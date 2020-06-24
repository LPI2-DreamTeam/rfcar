// MISSING FUNCTIONALITY:
// - A SharedMemory::addThreads() function that can receives a list of threads to be authorized all at once (eliminates the need to
// use SharedMemory::addThread() for each thread we want to authorize). It could also receive a thread pool.
// - A thread pool
// - A way to assign an anonymous function as a secondary trigger for breaking the notification waits, as in line 92 of example.cpp

// FUTURE CORRECTIONS:
// - Some inlined functions have come to take too big of a size to be inline anymore.
// - Some types and enumerations could be declared inside classes to reaffirm their scope.
// - Since all the code would be shared between platforms, the platform-specific functions in Thread could be platform-agnostic 
// and simplty not do anything in certain platforms
// - There is redundancy in SharedMemory: each SharedMemory object has an 'authorized_thread' variable and its own 'mutex' variable 
// has an 'owner'. These serve the same purpose and, although not clashing with each other at any point, it is a redundant use of 
// computation and memory.

#include <iostream>
#include <string>
#include <list>
#include <utility>
#include <cstdio>
#include <map>

#include "main.h"
#include "SharedMemory.h"
#include "Notification.h"


#define LIST_LEN		250
#define SIZE_THRESHOLD	3

using ThreadIDIntPair = std::pair<ThreadID, uint32_t>;
using ThreadIDIntPairList = std::list<ThreadIDIntPair>;
using IntList = std::list<uint32_t>;

static uint32_t g_iterator;

// List of g_iterator values produced by the producer threads.
// It is published to by the producer threads and consumed by the consumer threads. 
// Its initial and final size is 0. It grows as it is published to and shrinks as it is consumed.
static IntList g_global_list;
static SharedMemory<IntList> g_shared_global_list(&g_global_list);

// List of all the g_iterator values and the ids of the threads that did work on them.
// This list allows us to map which producer thread published each g_iterator value and which consumer thread consumed it.
// It is ever-growing and its final size is equal to LIST_LEN*2
static ThreadIDIntPairList g_work_list;
static SharedMemory<ThreadIDIntPairList> g_shared_work_list(&g_work_list);

// Notification signaling that the threahlohd has been reached and the consumer threads may resume work.
static Notification threshold_reached;


bool consume;

void producerFun(Thread* thread) {

	printf("Thread '%s' started\n", thread->name);

	consume = true;

	while (g_iterator < LIST_LEN) {
		thread->keepCurrentTimestamp();

		g_shared_work_list.grab();
		g_shared_global_list.grab();

		// If a producer thread is stuck waiting for authorization to access the memory while another is publishing (LIST_LEN - 1),
		// it will 
		if (g_iterator >= LIST_LEN) {
			g_shared_global_list.release();
			g_shared_work_list.release();
			break;
		}

		IntList* global_list = g_shared_global_list.data();
		ThreadIDIntPairList* work_list = g_shared_work_list.data();

		work_list->push_back(ThreadIDIntPair(thread->ownID(), g_iterator));
		global_list->push_back(g_iterator);

		g_iterator += 1;

		if (global_list->size() > SIZE_THRESHOLD)
			threshold_reached.notifyOne();

		g_shared_global_list.release();
		g_shared_work_list.release();

		// Sleep helps with effort distribution between producer and consumer threads
		thread->sleepUntilElapsed(20);
	}	

	threshold_reached.notifyOne();
	consume = false;
}

void consumerFun(Thread* thread) {

	printf("Thread '%s' started\n", thread->name);

	while (!consume);

	while (consume) {

		threshold_reached.wait();

		g_shared_work_list.grab();
		g_shared_global_list.grab();

		IntList* global_list = g_shared_global_list.data();
		ThreadIDIntPairList* work_list = g_shared_work_list.data();

		while (global_list->size()) {
			work_list->push_back(ThreadIDIntPair(thread->ownID(), global_list->front()));
			g_shared_global_list.data()->pop_front();
		}

		g_shared_global_list.release ();
		g_shared_work_list.release();
	}
}

int main(int argc, char* argv[]) {

	Thread p1("Producer 1", producerFun);
	Thread p2("Producer 2", producerFun);
	Thread p3("Producer 3", producerFun);
	Thread c1("Consumer 1", consumerFun);
	Thread c2("Consumer 2", consumerFun);

	std::cout << p1.name << ": " << p1.ownID() << std::endl;
	std::cout << p2.name << ": " << p2.ownID() << std::endl;
	std::cout << p3.name << ": " << p3.ownID() << std::endl;
	std::cout << "Working..." << std::endl;

	// Prepare the iterator
	g_iterator = 0;

	// Add threads to the list of threads who can access and change the lists
	g_shared_work_list.addThread(&p1);
	g_shared_work_list.addThread(&p2);
	g_shared_work_list.addThread(&p3);
	g_shared_work_list.addThread(&c1);
	g_shared_work_list.addThread(&c2);

	g_shared_global_list.addThread(&p1);
	g_shared_global_list.addThread(&p2);
	g_shared_global_list.addThread(&p3);
	g_shared_global_list.addThread(&c1);
	g_shared_global_list.addThread(&c2);

	// Set the tasks running
	p1.run();
	p2.run();
	p3.run();
	c1.run();
	c2.run();


/////////////////////////////////// TESTING ///////////////////////////////////
	std::map<ThreadID, std::pair<std::string, uint32_t>> id_name_map;

	id_name_map.insert({ p1.ownID(), std::pair<std::string, uint32_t>("P1", 0) } );
	id_name_map.insert({ p2.ownID(), std::pair<std::string, uint32_t>("P2", 0) });
	id_name_map.insert({ p3.ownID(), std::pair<std::string, uint32_t>("P3", 0) });
	id_name_map.insert({ c1.ownID(), std::pair<std::string, uint32_t>("C1", 0) });
	id_name_map.insert({ c2.ownID(), std::pair<std::string, uint32_t>("C2", 0) });
///////////////////////////////////////////////////////////////////////////////

	// Synchronize the tasks
	p1.join();
	p2.join();
	p3.join();
	c1.join();
	c2.join();


/////////////////////////////////// TESTING ///////////////////////////////////
	for (auto pair : g_work_list) {
		
		auto it = id_name_map.find(pair.first);
		std::cout << std::get<0>(it->second) << ": " << pair.second << std::endl;

		it->second = std::pair<std::string, uint32_t>(std::get<0>(it->second), 1 + std::get<1>(it->second));
	}

	std::cout << std::endl << "Total count: " << std::endl;

	for (auto pair : id_name_map) {
		std::cout << pair.second.first << ": " << pair.second.second << std::endl;
	}
///////////////////////////////////////////////////////////////////////////////
	
	while (1);
	return 0;
}