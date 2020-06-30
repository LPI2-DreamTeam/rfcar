#include "COM_LL.hpp"

#ifdef _LINUX_

#include <iostream>
#include <sstream>
#include <atomic>
#include <algorithm>
#include <thread>
#include <cerrno>


#define LOCALHOST		"127.0.0.1"
#define DEFAULT_PORT	"27015"

namespace COM {

	uint8_t checksum_calc(void* pbuff, uint32_t len) {
		uint16_t result = 0;
		uint8_t ret;
		uint8_t* puint = (uint8_t*)pbuff;
		uint32_t i;
		for (i = 0; i < len; i++) {
			result += puint[i];
			if (result > 0xffU) {
				result = (result >> 8) + (result & 0xffU);
			}
		}

		ret = (uint8_t)result;
		ret = ~ret;

		return ret;
	}

	template<Protocol protocol>
	uint32_t LL<protocol>::object_count = 0;


	template<Protocol protocol>
	LL<protocol>::LL() {}

	template<Protocol protocol>
	LL<protocol>::~LL() {

	}
}



//////////////////////////////// SERIAL_CLIENT ////////////////////////////////

namespace COM {
	
	LL<SERIAL_CLIENT>::LL() {

		this->connected = false;
		this->dead = false;
		this->sock_family = -1;
		this->sock_type = -1;
		this->ip_address = "";
	}

	LL<SERIAL_CLIENT>::~LL() {


	}

	Error LL<SERIAL_CLIENT>::read_str(char * p_buff, uint32_t len) {

		std::unique_lock<std::mutex>lock(this->mutex.native());

		if (!this->connected) {
			last_error = NOT_CONNECTED;
			last_error_str = "Not connected\n";

		} else if (this->dead) {
			last_error = DEAD;
			last_error_str = "Connection shut down\n";
			
		} else if (read(this->sock_fd, p_buff, len) < 0) {
			last_error = READ_FAIL;
			last_error_str = "Failed reading from socket\n";
		
		} else {
			last_error = OK;
			last_error_str = "";
		}

		return last_error;
	}

	Error LL<SERIAL_CLIENT>::write_str(const char * p_buff, uint32_t len) {
		
		std::unique_lock<std::mutex>lock(this->mutex.native());

		if (!this->connected) {
			last_error = NOT_CONNECTED;
			last_error_str = "Not connected\n";

		} else if (this->dead) {
			last_error = DEAD;
			last_error_str = "Connection shut down\n";
			
		} else if (write(this->sock_fd, p_buff, len) < 0) {
			last_error = WRITE_FAIL;
			last_error_str = "Failed writing to socket\n";
		
		} else {
			last_error = OK;
			last_error_str = "";
		}

		return last_error;
	}

	bool LL<SERIAL_CLIENT>::isConnected(void) {

		std::unique_lock<std::mutex>lock(this->mutex.native());

		return this->connected;
	}

	void LL<SERIAL_CLIENT>::kill(void) {

		std::unique_lock<std::mutex>lock(this->mutex.native());

		shutdown(this->sock_fd, SHUT_RDWR);
		this->dead = true;
	}

	Error LL<SERIAL_CLIENT>::closeConnection(void) {

		std::unique_lock<std::mutex>lock(this->mutex.native());

		if (this->connected) {

			if (close(this->sock_fd) < 0) {
				last_error = CLOSE_FAIL;
				last_error_str = "Failed closing connection\n";
				return CLOSE_FAIL;
			}
		}

		this->connected = false;
		last_error = OK;
		last_error_str = "";

		return last_error;
	}

	bool LL<SERIAL_CLIENT>::openConnection(std::string port) {

		std::unique_lock<std::mutex>lock(this->mutex.native());

		if (this->sock_fd > 0)
			return true;


		this->sock_fd = socket(this->sock_family, this->sock_type, 0);

		if (this->sock_fd < 0) {
			last_error = OPEN_FAIL;
			last_error_str = "Failed to open socket\n";
			return false;
		}

		std::fill_n((char*)& this->serv_addr, sizeof(this->serv_addr), 0);

		this->serv_addr.sin_family = this->sock_family;
		serv_addr.sin_port = htons(this->sock_port);


		if (inet_pton(this->sock_family, this->ip_address.c_str(), &serv_addr.sin_addr) <= 0)
		{
			last_error = OPEN_FAIL;
			last_error_str = "Invalid address/ Address not supported \n";
			return false;
		}


		if (connect(this->sock_fd, (struct sockaddr*) & this->serv_addr, sizeof(this->serv_addr)) < 0) {
			last_error_str = "Failed connecting\n";
			return false;
		}

		this->connected = true;
		
		return this->connected;
	}

	Error LL<SERIAL_CLIENT>::getLastError(std::string& error_message) {
		error_message = last_error_str;
		return last_error;
	}

	Error LL<SERIAL_CLIENT>::getLastError() {
		return last_error;
	}
}



//////////////////////////////// SERIAL_SERVER ////////////////////////////////

namespace COM {
	
	LL<SERIAL_SERVER>::LL() {


	}

	LL<SERIAL_SERVER>::~LL() {


	}

	Error LL<SERIAL_SERVER>::read_str(char * p_buff, uint32_t len) {
		return OK;
	}

	Error LL<SERIAL_SERVER>::write_str(const char * p_buff, uint32_t len) {
		return OK;
	}

	bool LL<SERIAL_SERVER>::isConnected(void) {
		return false;
	}

	void LL<SERIAL_SERVER>::kill(void) {
		return;
	}

	Error LL<SERIAL_SERVER>::closeConnection(void) {
		return OK;
	}

	Error LL<SERIAL_SERVER>::getLastError(std::string & emsg) {
		return OK;
	}

	Error LL<SERIAL_CLIENT>::getLastError() {
		return last_error;
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

	void LL<SERIAL_SERVER>::acceptConnection(void) {

		
	}


}


#endif		// _LINUX_ 