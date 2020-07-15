#include <iostream>
#include <string>
#include <list>
#include <utility>
#include <cstdio>
#include <map>

#include "main.hpp"
#include "Thread.hpp"
#include "WCOM_LL.hpp"

//void bluetoothSocketServerTest(OS::Thread*) {
//
//    WCOM::Error error;
//    std::string error_str;
//    WCOM::LL<WCOM::Protocol::BLUETOOTH, WCOM::Role::SERVER> server(4);
//    // char buffer[30] = "AT+PSWD=1234\r\n";
//    char buffer[30] = "AT+PSWD?\r\n";
//
//    std::cout << "FUN[socketServerTest]: STARTED\n";
//
//    // Create socket, bind and listen for connection
//    error = server.listenConnection();
//    server.getLastError(error_str);
//    std::cout << "SERVER   " << error_str;
//
//    if (error)
//        return;
//
//    // Accept connection
//    error = server.acceptConnection();	
//    server.getLastError(error_str);
//    std::cout << "SERVER   " << error_str;
//
//
//    // server.writeStr(buffer, sizeof(buffer));
//    // error = server.getLastError(error_str);
//    // std::cout << "SERVER   " << error_str;
//    memset(buffer, '\0', sizeof(buffer));
//
//    std::cout << "SERVER   Waiting..." << std::endl;
//
//    while (1) {
//
//        server.readStr(buffer, 30);
//		
//        if (server.getLastError(error_str) != WCOM::Error::OK) {
//            std::cout << "SERVER   " << error_str;
//            break;
//        }
//
//        if (std::string(buffer) != "") {
//            std::cout << "SERVER   " << error_str;
//            std::cout << "Received: " << buffer << std::endl;
//            server.writeStr(buffer, strlen(buffer));
//            server.getLastError(error_str);
//            std::cout << "SERVER   " << error_str;
//            break;
//        }
//    }
//
//    server.closeConnection();
//
//
//    return;
//}


void tcp_socketServerTest(OS::Thread*) {

    WCOM::Error error;
    std::string error_str;
    WCOM::LL<WCOM::Protocol::TCP, WCOM::Role::SERVER> server(1);
    char buffer[30];

    // Serves to demonstrate that a pointer to the generic class can be used to access methods methods from the specialized classes
    WCOM::LL<>* server_ptr = &server;
    WCOM::LL<WCOM::Protocol::TCP, WCOM::Role::SERVER>* server_ptr2 = \
        static_cast<WCOM::LL<WCOM::Protocol::TCP, WCOM::Role::SERVER>*>(server_ptr);

    std::cout << "FUN[socketServerTest]: STARTED\n";
	
    // Create socket, bind and listen for connection
    error = server_ptr2->listenConnection();
    server_ptr->getLastError(error_str);
    std::cout << "SERVER   " << error_str;

    if (error)
        return;

    // Accept connection
    error = server.acceptConnection();	
    server_ptr->getLastError(error_str);
    std::cout << "SERVER   " << error_str;

    // If there was an error during acceptance, return immediately.
    // The rest has been taked care of by the socket.
    if (error)
        return;


    while(1) {

        char temp_buffer[6];
		
        // Read string into temporary buffer to demonstrate how a message could be read continuously
        server_ptr->readStr(temp_buffer, 5);
        error = server_ptr->getLastError(error_str);
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


void tcp_socketClientTest(OS::Thread*) {

    WCOM::Error error;
    std::string error_str;
    WCOM::LL<WCOM::Protocol::TCP, WCOM::Role::CLIENT> client(1);
    WCOM::LL<>* client_ptr = &client;
	
    char buffer[30] = "This is a message\n";

    std::cout << "FUN[socketClientTest]: STARTED\n";
	
    // Complains about being asked to create 2 objects with the same port, as it should
    // WCOM::LL<WCOM::Protocol::TCP, WCOM::Role::CLIENT> client2(1);
    // client2.getLastError(error_str);
    // std::cout << error_str;

    // Open connection
    client.openConnection();
    error = client_ptr->getLastError(error_str);
    std::cout << "CLIENT   " << error_str;

    if (error)
        return;

    // Send message
    client_ptr->writeStr(buffer, sizeof(buffer));
    error = client_ptr->getLastError(error_str);
    std::cout << "CLIENT   " << error_str;
}



int main(int argc, char* argv[]) {

    using namespace std::chrono_literals;
    OS::Thread server_thread("Socket server test", tcp_socketServerTest);
    OS::Thread client_thread("Socket client test", tcp_socketClientTest);

    std::cout << "\n---------------------------\n";
    std::cout << "Working...\n\n";


    // Set the tasks running
    server_thread.run();
    // Wait for the server to configure. Anothr option would be to make 
    // the client try to establish a connection indefinitely.
    //std::this_thread::sleep_for(1s);
    //client_thread.run();

    // Synchronize the tasks
    server_thread.join();
    //client_thread.join();
    std::cout << "Joined!" << std::endl;

    return 0;
}
