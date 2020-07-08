#include "SafeList.hpp"
#include <atomic>
#include <sstream>
#include <iostream>
#include <thread>
namespace hal_asos {

	namespace test_units {



		void safe_list_simple(void)
		{
			int index;
            size_t ret =0;
            index = (int) ret;
			SafeList<int> L0;
			int buffer[5];

			SafeList<std::shared_ptr<int>>L1;
			std::shared_ptr<int>p_int;

			for (index = 0; index < 5; index++)
				L0.safe_push_back(index);

			ret = L0.size();


			for (index = 0; index < 5; index++) {
				L0.safe_pop_front(buffer + index);
			}

			L0.kill();
			L0.size();

			for (index = 0; index < 5; index++) {
				p_int = std::make_shared<int>();
				*p_int = index;
				L1.safe_push_back(p_int);
			}


			ret = L1.size();

			for(index = 5-1; index >=0; index--){
				L1.safe_pop_front(p_int);
				buffer[index] = *p_int;
			}

			L1.kill();
			L1.size();

		}

#define SAFELIST_NLEN 1024
		void* t_run( void* arg)
		{
			static std::atomic<int> Cntr = 0;
			static std::atomic<int> StateId = 0;
			std::mutex cnt_mutex;
			SafeList<int>*plist = (SafeList<int>*)arg;
			std::stringstream s;
			int local_count = 0;
			int local_id;
			
			local_id= StateId++;

			while (Cntr < SAFELIST_NLEN) {
					
				plist->safe_push_back(Cntr++);
				local_count++;
			}
			s << "Thread(" << local_id << ") processed numbers:" << local_count << "\n";
			std::cout << s.str();
			return nullptr;
		}

		void safe_list_mx_thread(void)
		{
			SafeList<int> llist;
			std::thread pool[5];
			int local_buff[SAFELIST_NLEN];
			std::stringstream l;
			int *pint;
			int i;
			size_t len;
			for (i = 0; i < 5; i++) {
				pool[i] = std::thread(t_run, &llist);
			}

			for (i = 0; i < 5; i++) {
				if(pool[i].joinable())
					pool[i].join();
			}
			
			len = llist.size();
			for (pint = local_buff, i=0; (unsigned)i <len; i++,pint++) {
				llist.safe_pop_front(pint);
				l << *pint<< " ";
			}
			l << "\n";
			std::cout << l.str();

		}

	}
}