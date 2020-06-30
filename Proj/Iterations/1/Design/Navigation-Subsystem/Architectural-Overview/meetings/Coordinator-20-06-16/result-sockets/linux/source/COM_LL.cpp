#define _CRT_SECURE_NO_WARNINGS
#include "COM_LL.hpp"

#ifdef __WINDOWS__

#define LOCALHOST		"127.0.0.1"
#define DEFAULT_PORT	"27015"

namespace COM {

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")



	template<Protocol protocol>
	uint32_t LL<protocol>::object_count = 0;


	static WSADATA wsaData;

	template<Protocol protocol>
	LL<protocol>::LL() {

		int startup_result=0;

		if (++object_count == 1) {
			startup_result = WSAStartup(MAKEWORD(2, 2), &wsaData);
			if (startup_result != 0) {
				printf("WSAStartup failed with error: %d\n", startup_result);
				return;
			}
		}
	}

	template<Protocol protocol>
	LL<protocol>::~LL() {

	}
}



//////////////////////////////// SERIAL_CLIENT ////////////////////////////////

namespace COM {
	
	LL<SERIAL_CLIENT>::LL() {

		this->hints.ai_socktype = SOCK_STREAM;
		this->hints.ai_family = AF_UNSPEC;
		this->hints.ai_protocol = IPPROTO_TCP;
		this->ip_address = LOCALHOST;
		this->port = DEFAULT_PORT;
		this->connect_socket = INVALID_SOCKET;

		ZeroMemory(&this->hints, sizeof(struct addrinfo));
	}

	LL<SERIAL_CLIENT>::~LL() {


	}

	Error LL<SERIAL_CLIENT>::read(char * p_buff, int len) {
		return OK;
	}

	Error LL<SERIAL_CLIENT>::write(const char * p_buff, int len) {
		return OK;
	}

	bool LL<SERIAL_CLIENT>::isConnected(void) {
		return false;
	}

	bool LL<SERIAL_CLIENT>::kill(void) {
		return false;
	}

	Error LL<SERIAL_CLIENT>::closeConnection(void) {
		return OK;
	}

	bool LL<SERIAL_CLIENT>::openConnection(std::string port) {
		
		struct addrinfo* result;
		struct addrinfo* ptr;
		uint32_t request_result;
		
		// Resolve the server address and port
		request_result = getaddrinfo(this->ip_address.c_str(), this->port.c_str(), &this->hints, &result);
		
		if (request_result != 0)
			return false;

	
		// Attempt to connect to an address until one succeeds
		for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

			// Create a SOCKET for connecting to server
			this->connect_socket = socket(ptr->ai_family, ptr->ai_socktype,
				ptr->ai_protocol);

			if (this->connect_socket == INVALID_SOCKET)
				return false;

			// Connect to server.
			request_result = connect(this->connect_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (request_result == SOCKET_ERROR) {
				closesocket(this->connect_socket);
				this->connect_socket = INVALID_SOCKET;
				continue;
			}
			break;
		}

		freeaddrinfo(result);

		if (connect_socket == INVALID_SOCKET)
			return false;

		this->connected = true;

		return true;
	}

	Error LL<SERIAL_CLIENT>::getLastError(std::string & emsg) {
		return OK;
	}
}



//////////////////////////////// SERIAL_SERVER ////////////////////////////////

namespace COM {
	
	LL<SERIAL_SERVER>::LL() {

		this->hints.ai_socktype = SOCK_STREAM;
		this->hints.ai_family = AF_UNSPEC;
		this->hints.ai_protocol = IPPROTO_TCP;
		this->ip_address = LOCALHOST;
		this->port = DEFAULT_PORT;
		this->client_socket = INVALID_SOCKET;
		this->listen_socket = INVALID_SOCKET;

		ZeroMemory(&this->hints, sizeof(struct addrinfo));
	}

	LL<SERIAL_SERVER>::~LL() {


	}

	Error LL<SERIAL_SERVER>::read(char * p_buff, int len) {
		return OK;
	}

	Error LL<SERIAL_SERVER>::write(const char * p_buff, int len) {
		return OK;
	}

	bool LL<SERIAL_SERVER>::isConnected(void) {
		return false;
	}

	bool LL<SERIAL_SERVER>::kill(void) {
		return false;
	}

	Error LL<SERIAL_SERVER>::closeConnection(void) {
		return OK;
	}

	Error LL<SERIAL_SERVER>::getLastError(std::string & emsg) {
		return OK;
	}

	Error LL<SERIAL_SERVER>::getClientIP(std::string & ip) {
		return Error();
	}

	Error LL<SERIAL_SERVER>::getConnectionPort(std::string & port) {
		return Error();
	}

	bool LL<SERIAL_SERVER>::listenConnection(void) {
		return false;
	}

	void LL<SERIAL_SERVER>::acceptConnection(void) {}


}


#endif