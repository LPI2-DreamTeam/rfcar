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
	}

	LL<SERIAL_CLIENT>::~LL() {


	}

	Error LL<SERIAL_CLIENT>::readStr(char * p_buff, uint32_t len) {

		std::unique_lock<std::mutex>lock(this->mutex.native());

		if (!this->connected) {
			last_error = NOT_CONNECTED;
			last_error_str = "ERROR[NOT_CONNECTED]: Not connected\n";

		} else if (this->dead) {
			last_error = DEAD;
			last_error_str = "ERROR[DEAD]: Connection shut down\n";
			
		} else if (read(this->sock_fd, p_buff, len) < 0) {
			last_error = READ_FAIL;
			last_error_str = "ERROR[READ_FAIL]: Failed reading from socket\n";
		
		} else {
			last_error = OK;
			last_error_str = "";
		}

		return last_error;
	}

	Error LL<SERIAL_CLIENT>::writeStr(const char * p_buff, uint32_t len) {
		
		std::unique_lock<std::mutex>lock(this->mutex.native());

		if (!this->connected) {
			last_error = NOT_CONNECTED;
			last_error_str = "ERROR[NOT_CONNECTED]: Not connected\n";

		} else if (this->dead) {
			last_error = DEAD;
			last_error_str = "ERROR[DEAD]: Connection shut down\n";
			
		} else if (write(this->sock_fd, p_buff, len) < 0) {
			last_error = WRITE_FAIL;
			last_error_str = "ERROR[WRITE_FAIL]: Failed writing to socket\n";
		
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
				last_error_str = "ERROR[CLOSE_FAIL]: Failed closing connection\n";
				return CLOSE_FAIL;
			}
		}

		this->connected = false;
		last_error = OK;
		last_error_str = "";

		return last_error;
	}

	bool LL<SERIAL_CLIENT>::openConnection(std::string port) {

		std::string server_path = "/tmp/servers/" + port.replace(port.begin(), port.end(), ' ', '_');
		std::unique_lock<std::mutex>lock(this->mutex.native());

		if (this->sock_fd > 0)
			return true;


		this->sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);

		if (this->sock_fd < 0) {
			last_error = OPEN_FAIL;
			last_error_str = "ERROR[OPEN_FAIL]: Failed to open socket\n";
			return false;
		}

		std::fill_n((char*)& this->serv_addr, sizeof(this->serv_addr), 0);

		this->serv_addr.sun_family = AF_UNIX;
		strcpy(this->serv_addr.sun_path, server_path.c_str());

		if (connect(this->sock_fd, (struct sockaddr*) & this->serv_addr, sizeof(this->serv_addr)) < 0) {
			last_error = OPEN_FAIL;
			last_error_str = "ERROR[OPEN_FAIL]: Failed connecting\n";
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
	
	LL<SERIAL_SERVER>::LL(std::string name) {
		
		this->connected = false;
		this->dead = false;
		this->opt = 1;

		this->name = name;
	}

	LL<SERIAL_SERVER>::LL() : LL("default server") {
		
		this->connected = false;
		this->dead = false;
		this->opt = 1;
	}

	LL<SERIAL_SERVER>::~LL() {


	}

	Error LL<SERIAL_SERVER>::readStr(char * p_buff, uint32_t len) {
		
		std::unique_lock<std::mutex>lock(this->mutex.native());

		if (!this->connected) {
			last_error = NOT_CONNECTED;
			last_error_str = "ERROR[NOT_CONNECTED]: Not connected\n";

		} else if (this->dead) {
			last_error = DEAD;
			last_error_str = "ERROR[DEAD]: Connection shut down\n";
			
		} else if (read(this->sock_fd, p_buff, len) < 0) {
			last_error = READ_FAIL;
			last_error_str = "ERROR[READ_FAIL]: Failed reading from socket\n";
		
		} else {
			last_error = OK;
			last_error_str = "";
		}

		return last_error;
	}

	Error LL<SERIAL_SERVER>::writeStr(const char * p_buff, uint32_t len) {
		
		std::unique_lock<std::mutex>lock(this->mutex.native());

		if (!this->connected) {
			last_error = NOT_CONNECTED;
			last_error_str = "ERROR[NOT_CONNECTED]: Not connected\n";

		} else if (this->dead) {
			last_error = DEAD;
			last_error_str = "ERROR[DEAD]: Connection shut down\n";
			
		} else if (write(this->sock_fd, p_buff, len) < 0) {
			last_error = WRITE_FAIL;
			last_error_str = "ERROR[WRITE_FAIL]: Failed writing to socket\n";
		
		} else {
			last_error = OK;
			last_error_str = "";
		}

		return last_error;
	}

	bool LL<SERIAL_SERVER>::isConnected(void)  {

		std::unique_lock<std::mutex>lock(this->mutex.native());

		return this->connected;
	}

	void LL<SERIAL_SERVER>::kill(void) {

		std::unique_lock<std::mutex>lock(this->mutex.native());

		shutdown(this->sock_fd, SHUT_RDWR);
		this->dead = true;
	}

	Error LL<SERIAL_SERVER>::closeConnection(void) {

		std::unique_lock<std::mutex>lock(this->mutex.native());

		if (this->connected) {

			if (close(this->sock_fd) < 0) {
				last_error = CLOSE_FAIL;
				last_error_str = "ERROR[CLOSE_FAIL]: Failed closing connection\n";
				return CLOSE_FAIL;
			}
		}

		this->connected = false;
		last_error = OK;
		last_error_str = "";

		return last_error;
	}

	Error LL<SERIAL_SERVER>::getLastError(std::string& error_message) {
		error_message = last_error_str;
		return last_error;
	}

	Error LL<SERIAL_SERVER>::getLastError() {
		return last_error;
	}

	Error LL<SERIAL_SERVER>::listenConnection(void) {

		std::string server_path = "/tmp/servers/" + this->name.replace(this->name.begin(), this->name.end(), ' ', '_');
		std::string command = "mkdir -p ";
		std::unique_lock<std::mutex>lock(this->mutex.native());

		
		// There is already a socket atributed
		if (this->sock_fd > 0)
			return last_error;

		// Request an endpoint for communication
		this->sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
		
		// If the socket fails to open
		if (this->sock_fd < 0) {
			last_error = OPEN_FAIL;
			last_error_str = "ERROR[OPEN_FAIL]: Failed to open socket";
			return last_error;
		}

		std::fill_n((char*)& this->serv_addr, sizeof(this->serv_addr), 0);
		
		command.append(server_path);
		system(command.c_str());

		this->serv_addr.sun_family = AF_UNIX;
		strcpy(this->serv_addr.sun_path, server_path.c_str());

		if (bind(this->sock_fd, (struct sockaddr*) & this->serv_addr, SUN_LEN(&this->serv_addr)) < 0) {
			last_error = OPEN_FAIL;
			last_error_str = "ERROR[OPEN_FAIL]: Failed to bind";
			return last_error;
		}

		if (listen(this->sock_fd, 5) < 0) {
			last_error = OPEN_FAIL;
			last_error_str = "ERROR[OPEN_FAIL]: Failed to listen";
			return last_error;
		}

		this->connected = true;
		last_error = OK;
		last_error_str = "";

		return last_error;
	}

	Error LL<SERIAL_SERVER>::acceptConnection(void) {

		socklen_t clilen;

		
		if (this->connected == false) {
			last_error = ACCEPT_FAIL;
			last_error_str = "ERROR[ACCEPT_FAIL]: Should listen before accepting";
			return last_error;
		}

		clilen = sizeof(this->serv_addr);

		this->sock_fd2 = accept(this->sock_fd, (struct sockaddr*) &this->serv_addr2, &clilen);
		
		if (this->sock_fd2 <= 0) {
			last_error = ACCEPT_FAIL;
			last_error_str = "ERROR[ACCEPT_FAIL]: Failed acceptance";
			return last_error;
		}

		last_error = OK;
		last_error_str = "";
		return last_error;
	}


}


#endif		// _LINUX_ 