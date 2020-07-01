<<<<<<< Updated upstream
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
=======
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

	COM::Error error;
	std::string error_str;
	COM::LL<COM::Protocol::SERIAL_SERVER> server("Server1");
	char buffer[30];

	std::cout << "FUN[socketServerTest]: STARTED\n";
	
	// Create socket, bind and listen for connection
	error = server.listenConnection();
	server.getLastError(error_str);
	std::cout << "SERVER   " << error_str;

	if (error)
		return;

	// Accept connection
	error = server.acceptConnection();	
	server.getLastError(error_str);
	std::cout << "SERVER   " << error_str;

	// If there was an error during acceptance, return immediately.
	// The rest has been taked care of by the socket.
	if (error)
		return;


	while(1) {

		char temp_buffer[6];
		
		// Read string into temporary buffer to demonstrate how a message could be read continuously
		server.readStr(temp_buffer, 5);
		error = server.getLastError(error_str);
		std::cout << "SERVER   " << error_str;

		if (error)
			break;

		// Concatenate temporary string with permanent one
		strcat(buffer, (const char*)temp_buffer);

		// Check for reception of the message termination character
		if (strchr(buffer, '\n') != NULL) {
			// When finished, ptint the message and close the connection
			std::cout << "FUN[socketServerTest]: Message received: " << buffer;
			break;
		} 
	}

	server.closeConnection();
	return;
}


void socketClientTest(OS::Thread*) {

	COM::Error error;
	std::string error_str;
	COM::LL<COM::Protocol::SERIAL_CLIENT> client;
	char buffer[30] = "This is a message\n";

	std::cout << "FUN[socketClientTest]: STARTED\n";

	// Open connection
	client.openConnection("Server1");
	error = client.getLastError(error_str);
	std::cout << "CLIENT   " << error_str;

	if (error)
		return;

	// Send message
	client.writeStr(buffer, sizeof(buffer));
	error = client.getLastError(error_str);
	std::cout << "CLIENT   " << error_str;
}



int main(int argc, char* argv[]) {

	using namespace std::chrono_literals;
	OS::Thread server_thread("Socket server test", socketServerTest);
	OS::Thread client_thread("Socket client test", socketClientTest);

	std::cout << "\n---------------------------\n";
	std::cout << "Working...\n\n";


	// Set the tasks running
	server_thread.run();
	// Wait for the server to configure. Anothr option would be to make 
	// the client try to establish a connection indefinitely.
	std::this_thread::sleep_for(1s);
	client_thread.run();

	// Synchronize the tasks
	server_thread.join();
	client_thread.join();
	std::cout << "Joined!" << std::endl;

	return 0;
>>>>>>> Stashed changes
}