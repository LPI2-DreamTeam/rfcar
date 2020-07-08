#pragma once
#include <list>
#include <mutex>
#include <condition_variable>


namespace hal_asos {
	template <typename T>
	class SafeList
	{
		std::list<T> NativeList;
		std::mutex ListMutex;
		std::condition_variable ListCondition;
		bool WaitingData;
		bool Dead;
	public:
		SafeList()
		{
			this->WaitingData = false;
			this->Dead = false;
		}
		~SafeList()
		{
			while (this->NativeList.size() > 0)
				this->NativeList.pop_front();
		}
		size_t safe_push_back(T val)
		{
			std::unique_lock<std::mutex> lock(this->ListMutex);
			if (this->Dead == false) {
				this->NativeList.push_back(val);
				if (this->WaitingData)
					this->ListCondition.notify_one();
				return this->NativeList.size();
			}
			return -ENOBUFS;
		}
		size_t safe_pop_front(T* p_val)
		{
			std::unique_lock<std::mutex> lock(this->ListMutex);
			while (this->Dead == false && this->NativeList.size() == 0) {
				this->WaitingData = true;
				this->ListCondition.wait(lock, [this]() {return this->Dead || this->NativeList.size() > 0; });
				this->WaitingData = false;
			}
			if (this->Dead == false)
			{
				*p_val = this->NativeList.front();
				this->NativeList.pop_front();
				return this->NativeList.size();
			}
			return -ENODATA;
		}
		int safe_try_pop_front(T* p_val)
		{
			std::unique_lock<std::mutex> lock(this->ListMutex);
			if (this->Dead == false && this->NativeList.size()>0)
			{
				*p_val = this->NativeList.front();
				this->NativeList.pop_front();
				return 0;
			}
			return -ENODATA;
		}
		size_t size()
		{
			std::unique_lock<std::mutex> lock(this->ListMutex);
			if (this->Dead)
				return -ENOBUFS;
			return this->NativeList.size();
		}
		size_t kill()
		{
			std::unique_lock<std::mutex> lock(this->ListMutex);
			this->Dead = true;
			if (this->WaitingData)
				this->ListCondition.notify_all();
			return this->NativeList.size();
		}
	};

	template <typename T>
	class SafeList<std::shared_ptr<T>> {
		std::list<std::shared_ptr<T>> NativeList;
		std::mutex ListMutex;
		std::condition_variable ListCondition;
		bool WaitingData;
		bool Dead;
	public:
		SafeList() {
			this->WaitingData = false;
			this->Dead = false;

		}
		~SafeList() {
			while (this->NativeList.size() > 0)
				this->NativeList.pop_front();
		}
		size_t safe_push_back(std::shared_ptr<T>& p_val)
		{
			std::unique_lock<std::mutex> lock(this->ListMutex);
			if (this->Dead == false) {
				this->NativeList.push_back(p_val);
				if (this->WaitingData)
					this->ListCondition.notify_one();
				return this->NativeList.size();
			}
			return -ENOBUFS;
		}
		size_t safe_pop_front(std::shared_ptr<T>& p_val)
		{
			std::unique_lock<std::mutex> lock(this->ListMutex);
			while (this->Dead == false && this->NativeList.size() == 0) {
				this->WaitingData = true;
				this->ListCondition.wait(lock, [this]() {return this->Dead || this->NativeList.size() > 0; });
				this->WaitingData = false;
			}
			if (this->Dead == false)
			{
				p_val = this->NativeList.front();
				this->NativeList.pop_front();
				return this->NativeList.size();
			}
			return -ENODATA;
		}
		int safe_try_pop_front(std::shared_ptr<T>& p_val)
		{
			std::unique_lock<std::mutex> lock(this->ListMutex);
			if (this->Dead == false && this->NativeList.size()>0){
				p_val = this->NativeList.front();
				this->NativeList.pop_front();
				return 0;
			}
			return -ENODATA;
		}
		int safe_front(std::shared_ptr<T>& p_val) {
			std::unique_lock<std::mutex> lock(this->ListMutex);
			if (this->Dead == false && this->NativeList.size()>0) {
				p_val = this->NativeList.front();
				return 0;
			}
			return -ENODATA;
		}

		int safe_discard_front(void) {
			std::unique_lock<std::mutex> lock(this->ListMutex);
			if (this->Dead == false && this->NativeList.size()>0) {
				this->NativeList.pop_front();
				return 0;
			}
			return -ENODATA;
		}
		size_t size()
		{
			std::unique_lock<std::mutex> lock(this->ListMutex);
			if (this->Dead)
				return -ENOBUFS;
			return this->NativeList.size();
		}
		int kill()
		{
			std::unique_lock<std::mutex> lock(this->ListMutex);
			this->Dead = true;
			if (this->WaitingData)
				this->ListCondition.notify_all();
			return (int)this->NativeList.size();
		}
	};


	namespace test_units {
		extern void safe_list_simple(void);
		extern void safe_list_mx_thread(void);
	}
}