#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "MEM.hpp"

#ifdef _LINUX_
#include <list>
#include <mutex>
#include <condition_variable>

#endif

namespace MEM {

	template <typename T>
	class LinkedList {

	private:
		 
#ifdef _LINUX_
		std::mutex list_mutex;					/*! Mutex for use with std::unique_lock in managing accesses and operations */
		std::condition_variable list_condition;	/*! Condition variable for use in waits (see waiting_data: bool) */
#endif

		std::list<T> list;		/* Native list */

		Error last_error;	/*! Result from the execution of the last function */
		bool waiting_data;	/*! True when pop() or popBulk() are waiting for an element in BlockingMode::BLOCKING */
		bool dead;			/*! Once dead, the list may no longer be used. Accessing or operating upon it will yield Error::DEAD */


		/*! @return Whether or not the list is empty
		*/
		inline bool _isEmpty() {
			return list.size() == 0;
		}


		/*! @return Whether or not the list is full
		*/
		inline bool _isFull() {
			return list.size() == UINT32_MAX;
		}


		/*! @return Number of elements present in the list
		*/
		inline uint32_t _size() {
			return list.size();
		}


		/*! @return Number of spaces left in the list
		*/
		inline uint32_t _available() {
			return UINT32_MAX - list.size();
		}

	public:

		LinkedList() {
			this->waiting_data = false;
		}


		~LinkedList() {
			this->empty();
		}


		/*! @brief Push one element into the list
		*
		*	@param value Value to be stored  in the list
		*	@param position Where to insert the value
		*					- When position = BACK it will be inserted in the first position of the list
		*					- When position = FRONT it will be inserted after the last position of the list
		*
		*	@return Result of the operation
		*/
		Error push(T& value, Position position = BACK) {
#ifdef DEBUG
			auto _full_ = _isFull();
			auto _empty_ = _isEmpty();
			auto _size_ = _size();
			auto _available_ = _available();
#endif

			std::unique_lock<std::mutex> lock(this->list_mutex);

			if (this->dead) {
				last_error = DEAD;

			} else if (_isFull()) {
				last_error = FULL;

			} else {

				// Insert the element into the list
				auto it = position == BACK ? list.end() : list.begin();
				list.insert(it, value);

				// Notify any thread that might be waiting for an object to be inserted into the empty list
				if (this->waiting_data)
					this->list_condition.notify_one();

				last_error = OK;
			}

			return last_error;
		}



		/*! @brief Pop one element from the list
		*
		*	@param value Value of the element to be removed from the list
		*	@param blocking Whether the function operating mode is BLOCKING or NON_BLOCKING
		*					- In BLOCKING mode, whenever the list is empty, it waits for there to be something in the list and
		*					pops the first element.
		*					- In NON_BLOCKING mode, whenever the list is empty, it returns halfway through the operation,
		*					returning Error::EMPTY
		*
		*	@return Result of the operation
		*/
		Error pop(T& value, BlockingMode blocking = BlockingMode::NON_BLOCKING) {
#ifdef DEBUG
			auto _full_ = _isFull();
			auto _empty_ = _isEmpty();
			auto _size_ = _size();
			auto _available_ = _available();
#endif

			std::unique_lock<std::mutex> lock(this->list_mutex);

			if (this->dead) {
				last_error = DEAD;
				return DEAD;

			} else if (_isEmpty()) {

				// If in BlockingMode::NON_BLOCKING, give up immediately
				if (!blocking) {
					last_error = EMPTY;
					return EMPTY;
				}

				// If in BlockingMode::BLOCKING, wait until something appears in the list
				while (_isEmpty()) {
					this->waiting_data = true;
					this->list_condition.wait(lock, [this]() {return this->dead || !_isEmpty(); });
					this->waiting_data = false;
				}

			}

			// Take element from the list
			list.remove(value);

			last_error = OK;

			return last_error;
		}


		/*! @brief Get the number of elements in the list
		*
		*	@return The number of elements in the list
		*/
		uint32_t size() {

			std::unique_lock<std::mutex> lock(this->list_mutex);

			if (this->dead)
				last_error = DEAD;
			else
				last_error = OK;

			return _size();
		}


		/*! @brief Break the functioning of the list
		*
		*	Once dead, the list may no longer be used. Accessing or operating upon it will yield Error::DEAD
		*/
		void kill() {

			std::unique_lock<std::mutex> lock(this->list_mutex);

			this->dead = true;
			if (this->waiting_data)
				this->list_condition.notify_all();

			last_error = OK;
		}


		/*! @brief Empty the list
		*/
		inline void empty() {
			std::unique_lock<std::mutex> lock(this->list_mutex);
			list.clear();
			last_error = OK;
		}


		/*! @brief Get the result from the last function execution
		*
		*	@return General result code from the last function execution
		*/
		inline Error getLastError() {
			std::unique_lock<std::mutex> lock(this->list_mutex);
			return last_error;
		}
	};

}

#endif