#include <iostream>
#include <string>
#include <list>
#include <utility>
#include <cstdio>
#include <map>

#include "main.hpp"
#include "Thread.hpp"
#include "COM_LL.hpp"

void producerFun(OS::Thread*) {

	return;
}



int main(int argc, char* argv[]) {

	OS::Thread p1("Producer 1", producerFun);
	OS::Thread p2("Producer 2", producerFun);
	OS::Thread p3("Producer 3", producerFun);
	COM::LL<COM::Protocol::SERIAL_CLIENT> serial1;

	serial1.openConnection("");

	std::cout << p1.name << ": " << p1.ownID() << std::endl;
	std::cout << p2.name << ": " << p2.ownID() << std::endl;
	std::cout << p3.name << ": " << p3.ownID() << std::endl;
	std::cout << "Working..." << std::endl;


	// Set the tasks running
	p1.run();
	p2.run();
	p3.run();

	// Synchronize the tasks
	p1.join();
	p2.join();
	p3.join();
	std::cout << "Joined!" << std::endl;

	while (1);
	return 0;
}