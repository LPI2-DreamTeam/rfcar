#include <iostream>
#include <string>
#include <list>
#include <utility>
#include <cstdio>
#include <map>

#include "main.hpp"
#include "Thread.hpp"
#include "COM_LL.hpp"

void socketServerTest(OS::Thread*) {

	std::string error_str;
	COM::LL<COM::Protocol::SERIAL_SERVER> server("Server 1");
	char buffer[30];

	std::cout << "Started socketServerTest\n";

	if (server.listenConnection() != COM::Error::OK) {
		server.getLastError(error_str);
		std::cout << "ERROR\n";
		std::cout << error_str;
		return;
	}

	std::cout << "NO ERROR\n";

	if (server.acceptConnection() != COM::Error::OK) {
		server.getLastError(error_str);
		std::cout << error_str;
		server.closeConnection();
		return;
	}

	std::cout << "SERVER: connected\n";

	while(1) {

		if (server.readStr(buffer, 50) != COM::Error::OK) {
			if (buffer[0] != '\0') {
				std::cout << "SERVER: ";
				std::cout << buffer;
				server.closeConnection();
				return;
			}
		} else {
			server.getLastError(error_str);
			std::cout << error_str;
			server.closeConnection();
			return;
		}
	}
}


void socketClientTest(OS::Thread*) {

	std::string error_str;
	COM::LL<COM::Protocol::SERIAL_CLIENT> client;
	char buffer[30] = "This is a message";

	std::cout << "Started socketClientTest\n";

	if (client.openConnection("Server 1") != COM::Error::OK) {
		client.getLastError(error_str);
		std::cout << error_str;
		return;
	}

	std::cout << "CLIENT: connected\n";

	if (client.writeStr(buffer, sizeof(buffer)) != COM::Error::OK) {
		client.getLastError(error_str);
		std::cout << error_str;
		return;
	}

	std::cout << "CLIENT: sent message\n";
}



int main(int argc, char* argv[]) {

	using namespace std::chrono_literals;
	OS::Thread server_thread("Socket server test", socketServerTest);
	OS::Thread client_thread("Socket client test", socketClientTest);

	std::cout << server_thread.name << ": " << server_thread.ownID() << std::endl;
	std::cout << client_thread.name << ": " << client_thread.ownID() << std::endl;
	std::cout << "Working..." << std::endl;


	// Set the tasks running
	server_thread.run();
	std::this_thread::sleep_for(1s);
	client_thread.run();

	// Synchronize the tasks
	server_thread.join();
	client_thread.join();
	std::cout << "Joined!" << std::endl;

	while (1);
	return 0;
}