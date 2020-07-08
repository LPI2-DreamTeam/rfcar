#include <iostream>

#include <thread>
#include <atomic>
#include <list>
#include <mutex>
#include <condition_variable>
#include <sstream>
#include <chrono>
#define N_PRODUCERS 4
//incrementar ate o processamento estar PRODUCE_LEN / N_PRODUCERS por thread
#define PRODUCE_LEN 10000
 //100000

#define TRIGGER_SIZE 10

std::list<int> SharedList;
std::mutex ListMutex;
std::condition_variable ListWait;
bool ThreadWaiting = false;

std::atomic<int> ThreadsRunning = 0;

//Virtual timer
std::atomic<bool> TimeRunning;
std::atomic<uint64_t> vtime_1us_as_1ms = 0;
void thread_timer_1ms(void) {
	using namespace std::chrono_literals;

	vtime_1us_as_1ms = 0;
	while (TimeRunning) {
		std::this_thread::sleep_for(1ms);
		vtime_1us_as_1ms++;
	}

}

//std::atomic<uint64_t> vtime_1us_as_100us = 0;
//void thread_timer_100us(void) {
//	using namespace std::chrono_literals;
//
//	vtime_1us_as_1ms = 0;
//	while (TimeRunning) {
//		std::this_thread::sleep_for(100us);
//		vtime_1us_as_100us++;
//	}
//
//}

void thread_run_producers(std::atomic<int>* p_counter) {

	int i;
	std::stringstream msg;
	int my_id = ThreadsRunning++;
	int my_counter = 0;

	msg << "[Thread" << my_id << "]:started....\n";
	std::cout << msg.str();
	msg.str("");

	while ((i = ++(*p_counter)) < PRODUCE_LEN) {

		ListMutex.lock();

		SharedList.push_back(i);
		my_counter++;
		if (ThreadWaiting && SharedList.size() > TRIGGER_SIZE)
			ListWait.notify_one();

		ListMutex.unlock();
	}

	msg << "[Thread" << my_id << "]:processed " << my_counter << " samples\n";
	std::cout << msg.str();

	ThreadsRunning--;

}


std::atomic<bool> Consume = true;
void thread_run_consumer(char* thread_name) {
	std::stringstream msg;
	int val;
	msg << "[" << thread_name << "]: results:[";
	while (Consume) {

		{
			std::unique_lock<std::mutex> Lock(ListMutex);
			while (SharedList.size() == 0) {
				ThreadWaiting = true;
				ListWait.wait(Lock, []() {return TimeRunning == false || SharedList.size() > TRIGGER_SIZE; });
				ThreadWaiting = false;
			}

		}

		ListMutex.lock();

		if (Consume || SharedList.size()) {
			while (SharedList.size()) {
				msg << ", ";
				val = SharedList.front();
				SharedList.pop_front();

				ListMutex.unlock();
				msg << val;
				ListMutex.lock();
			}
		}

		ListMutex.unlock();

	}

	msg << "]\n";
	std::cout << msg.str();
}
																			   

int notmain(int argc, char* argv[]) {
	int t;
	std::atomic<int> counter = 0;
	std::thread Thread_pool[N_PRODUCERS];
	TimeRunning = true;
	//create virtual timer
	std::thread vTime(thread_timer_1ms);
	//create consumer
	std::thread Consumer(thread_run_consumer, (char*)"VeryHardWorker");
	//create threads
	for (t = 0; t < N_PRODUCERS; t++)
		Thread_pool[t] = std::thread(thread_run_producers, &counter);

	//join at least on thread until they start
	Thread_pool->join();

	//wait for all threads before desroing

	for (t = 0; t < N_PRODUCERS; t++) {
		if (Thread_pool[t].joinable())
			Thread_pool[t].join();
	}



	Consume = false;



	//release if waiting just in case
	if (ThreadWaiting) {
		ListWait.notify_one();
	}

	//wait for consumer thread
	if (Consumer.joinable())
		Consumer.join();

	//wait for timer thread
	TimeRunning = false;
	vTime.join();
	std::cout << "[Main]: Job done! (" << counter << "," << vtime_1us_as_1ms << "[v_us])\n";


	while (1);
	return 0;
}