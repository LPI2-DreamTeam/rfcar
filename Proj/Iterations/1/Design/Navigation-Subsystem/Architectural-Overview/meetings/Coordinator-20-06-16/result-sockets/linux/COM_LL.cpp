#include "COM_LL.hpp"

#ifdef _LINUX_

#include <iostream>
#include <sstream>
#include <atomic>
#include <algorithm>
#include <thread>
#include <cerrno>

// AF_UNIX is used for communication between processes in the same machine efficiently
#define SERIAL_SOCKET_FAMILY	AF_UNIX
// SOCK_STREAM Provides sequenced, reliable, two-way, connection-based byte streams
#define SERIAL_SOCKET_TYPE		SOCK_STREAM

#define SERVERS_FOLDER		"/tmp/servers/"
#define SERVERS_PREFIX		"server"
#define DEFAULT_PORT		0

namespace COM {
	

	template<Protocol protocol, Role role>
	LL<protocol, role>::LL() {

	}

	template<Protocol protocol, Role role>
	LL<protocol, role >::~LL() {

	}
}



//////////////////////////////// SERIAL, CLIENT ////////////////////////////////

namespace COM {

	bool LL<SERIAL, CLIENT>::port_occupation[NO_AVAILABLE_PORTS + 1] = {0};
	
	LL<SERIAL, CLIENT>::LL(int32_t port) {

		this->port = port;

		if (port > 0 && port <= 5) {
			
			if (port_occupation[port] == true) {
				this->dead = true;
				last_error = INVALID_PORT;
				last_error_str = "The object was given an valid port number that was already taken\n";
			} else {
				this->dead = false;
				last_error = OK;
				last_error_str = "The object was correctly configured\n";
				port_occupation[port] = true;
			}
		} else {
			this->dead = true;
			last_error = INVALID_PORT;
			last_error_str = "The object was given an invalid port number\n";
		}

		this->connected = false;
		this->dead = false;

		this->connect_socket_fd = -1;
	}

	LL<SERIAL, CLIENT>::LL() : LL(DEFAULT_PORT) {

	}

	LL<SERIAL, CLIENT>::~LL() {

		
	}

	Error LL<SERIAL, CLIENT>::readStr(char * p_buff, uint32_t len) {

		std::unique_lock<std::mutex>lock(this->mutex.native());

		if (!this->connected) {
			last_error = NOT_CONNECTED;
			last_error_str = "ERROR[NOT_CONNECTED]: Not connected\n";

		} else if (this->dead) {
			last_error = DEAD;
			last_error_str = "ERROR[DEAD]: Object is dead\n";
			
		} else if (read(this->connect_socket_fd, p_buff, len) < 0) {
			last_error = READ_FAIL;
			last_error_str = "ERROR[READ_FAIL]: Failed reading from socket\n";
		
		} else {
			last_error = OK;
			last_error_str = "OK: Read string\n";
		}

		return last_error;
	}

	Error LL<SERIAL, CLIENT>::writeStr(const char * p_buff, uint32_t len) {
		
		std::unique_lock<std::mutex>lock(this->mutex.native());

		if (!this->connected) {
			last_error = NOT_CONNECTED;
			last_error_str = "ERROR[NOT_CONNECTED]: Not connected\n";

		} else if (this->dead) {
			last_error = DEAD;
			last_error_str = "ERROR[DEAD]: Object is dead\n";
			
		} else if (write(this->connect_socket_fd, p_buff, len) < 0) {
			last_error = WRITE_FAIL;
			last_error_str = "ERROR[WRITE_FAIL]: Failed writing to socket\n";
		
		} else {
			last_error = OK;
			last_error_str = "OK: Write string\n";
		}

		return last_error;
	}

	bool LL<SERIAL, CLIENT>::isConnected(void) {

		std::unique_lock<std::mutex>lock(this->mutex.native());

		return this->connected;
	}

	void LL<SERIAL, CLIENT>::kill(void) {

		std::unique_lock<std::mutex>lock(this->mutex.native());

		shutdown(this->connect_socket_fd, SHUT_RDWR);
		this->dead = true;
	}

	Error LL<SERIAL, CLIENT>::closeConnection(void) {

		std::unique_lock<std::mutex>lock(this->mutex.native());

		if (this->connected) {

			if (close(this->connect_socket_fd) < 0) {
				last_error = CLOSE_FAIL;
				last_error_str = "ERROR[CLOSE_FAIL]: Failed closing connection\n";
				return CLOSE_FAIL;
			}
		}

		this->connected = false;
		last_error = OK;
		last_error_str = "OK: CLose connection";

		return last_error;
	}

	bool LL<SERIAL, CLIENT>::openConnection() {

		std::string server_path = SERVERS_FOLDER SERVERS_PREFIX + std::to_string(this->port);
		std::unique_lock<std::mutex>lock(this->mutex.native());

		// Check if the socket is already open
		if (this->connect_socket_fd > 0) {
			last_error = ALREADY_OPEN;
			last_error_str = "ERROR[ALREADY_OPEN]: A connection with this server or another one is already open\n";
			return this->connected;
		}
	
		// Attempt to open socket
		this->connect_socket_fd = socket(SERIAL_SOCKET_FAMILY, SERIAL_SOCKET_TYPE, 0);

		// If this->connect_socket_fd == 1, it means that the socket could not be open and thus the file descriptor is invalid
		if (this->connect_socket_fd < 0) {
			last_error = OPEN_FAIL;
			last_error_str = "ERROR[OPEN_FAIL]: Failed to open socket\n";
			return this->connected;
		}

		std::fill_n((char*)& this->server_address, sizeof(this->server_address), 0);

		// Set server socket family and socket file path
		this->server_address.sun_family = SERIAL_SOCKET_FAMILY;
		strcpy(this->server_address.sun_path, server_path.c_str());

		// Attempt to establish a connection
		if (connect(this->connect_socket_fd, (struct sockaddr*) & this->server_address, sizeof(this->server_address)) < 0) {
			last_error = OPEN_FAIL;
			last_error_str = "ERROR[OPEN_FAIL]: Failed connecting\n";
			return this->connected;
		}

		this->connected = true;
		
		last_error = OK;
		last_error_str = "OK: Open connection\n";

		return this->connected;
	}

	Error LL<SERIAL, CLIENT>::getLastError(std::string& error_message) {
		error_message = last_error_str;
		return last_error;
	}

	Error LL<SERIAL, CLIENT>::getLastError() {
		return last_error;
	}
}



//////////////////////////////// SERIAL, SERVER ////////////////////////////////

namespace COM {

	bool LL<SERIAL, SERVER>::port_occupation[NO_AVAILABLE_PORTS + 1] = {0};
	
	LL<SERIAL, SERVER>::LL(int32_t port) {
		
		this->port = port;

		if (port > 0 && port <= 5) {
			
			if (port_occupation[port] == true) {
				this->dead = true;
				last_error = INVALID_PORT;
				last_error_str = "The object was given an valid port number that was already taken\n";
			} else {
				this->dead = false;
				last_error = OK;
				last_error_str = "The object was correctly configured\n";
				port_occupation[port] = true;
			}
		} else {

			this->dead = true;
			last_error = INVALID_PORT;
			last_error_str = "The object was given an invalid port number\n";
		}
		
		this->connected = false;	
		this->listen_socket_fd = -1;
		this->connect_socket_fd = -1;
		this->listened = false;
		
	}

	LL<SERIAL, SERVER>::LL() : LL(DEFAULT_PORT) {
		
	}

	LL<SERIAL, SERVER>::~LL() {

	}

	Error LL<SERIAL, SERVER>::readStr(char * p_buff, uint32_t len) {
		
		std::unique_lock<std::mutex>lock(this->mutex.native());

		if (!this->connected) {
			last_error = NOT_CONNECTED;
			last_error_str = "ERROR[NOT_CONNECTED]: Not connected\n";

		} else if (this->dead) {
			last_error = DEAD;
			last_error_str = "ERROR[DEAD]: Object is dead\n";
			
		} else if (read(this->connect_socket_fd, p_buff, len) < 0) {
			last_error = READ_FAIL;
			last_error_str = "ERROR[READ_FAIL]: Failed reading from socket\n";
		
		} else {
			last_error = OK;
			last_error_str = "OK: Read string\n";
		}

		return last_error;
	}

	Error LL<SERIAL, SERVER>::writeStr(const char * p_buff, uint32_t len) {
		
		std::unique_lock<std::mutex>lock(this->mutex.native());

		if (!this->connected) {
			last_error = NOT_CONNECTED;
			last_error_str = "ERROR[NOT_CONNECTED]: Not connected\n";

		} else if (this->dead) {
			last_error = DEAD;
			last_error_str = "ERROR[DEAD]: Object is dead\n";
			
		} else if (write(this->connect_socket_fd, p_buff, len) < 0) {
			last_error = WRITE_FAIL;
			last_error_str = "ERROR[WRITE_FAIL]: Failed writing to socket\n";
		
		} else {
			last_error = OK;
			last_error_str = "OK: Write string\n";
		}

		return last_error;
	}

	bool LL<SERIAL, SERVER>::isConnected(void)  {

		std::unique_lock<std::mutex>lock(this->mutex.native());

		return this->connected;
	}

	void LL<SERIAL, SERVER>::kill(void) {

		std::unique_lock<std::mutex>lock(this->mutex.native());

		shutdown(this->listen_socket_fd, SHUT_RDWR);
		this->dead = true;
	}

	Error LL<SERIAL, SERVER>::closeConnection(void) {

		std::unique_lock<std::mutex>lock(this->mutex.native());

		if (this->connected) {

			if (close(this->listen_socket_fd) < 0) {
				last_error = CLOSE_FAIL;
				last_error_str = "ERROR[CLOSE_FAIL]: Failed closing connection\n";
				return CLOSE_FAIL;
			}
		}

		this->connected = false;
		last_error = OK;
		last_error_str = "OK: Close connection\n";

		return last_error;
	}

	Error LL<SERIAL, SERVER>::getLastError(std::string& error_message) {

		error_message = last_error_str;
		return last_error;
	}

	Error LL<SERIAL, SERVER>::getLastError() {

		return last_error;
	}

	Error LL<SERIAL, SERVER>::listenConnection(void) {

		std::string server_path = SERVERS_FOLDER SERVERS_PREFIX + std::to_string(this->port);
		std::string command = "mkdir -p " SERVERS_FOLDER;
		std::unique_lock<std::mutex>lock(this->mutex.native());

		// There is already a socket atributed
		if (this->listen_socket_fd > 0)
			return last_error;

		// Request an endpoint for communication
		this->listen_socket_fd = socket(SERIAL_SOCKET_FAMILY, SERIAL_SOCKET_TYPE, 0);
		
		// If the socket fails to open
		if (this->listen_socket_fd < 0) {
			last_error = OPEN_FAIL;
			last_error_str = "ERROR[OPEN_FAIL]: Failed to open socket\n";
			return last_error;
		}

		std::fill_n((char*)& this->listen_serv_addr, sizeof(this->listen_serv_addr), 0);
		
		system(command.c_str());

		this->listen_serv_addr.sun_family = SERIAL_SOCKET_FAMILY;
		strcpy(this->listen_serv_addr.sun_path, server_path.c_str());

		// Path should be unlinked before a bind() call
		// https://stackoverflow.com/questions/17451971/getting-address-already-in-use-error-using-unix-socket
		unlink(server_path.c_str());
		int opt = 1;
		if (setsockopt(this->listen_socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
			last_error = OPEN_FAIL;
			last_error_str = "ERROR[OPEN_FAIL]: Failed to set socket options\n";
			return last_error;
		}

		// Assign the address specified by addr to the socket referred to by the file descriptor sockfd
		if (bind(this->listen_socket_fd, (struct sockaddr*) & this->listen_serv_addr, SUN_LEN(&this->listen_serv_addr)) < 0) {
			
			// Error feedback
			last_error = OPEN_FAIL;
			last_error_str = "ERROR[OPEN_FAIL]: Failed to bind\n";
			return last_error;
		}

		if (listen(this->listen_socket_fd, 5) < 0) {

			// Undo binding. Not absolutely necessary as it is done before the 'bind' instruction already.
			unlink(server_path.c_str());

			// Error feedback
			last_error = OPEN_FAIL;
			last_error_str = "ERROR[OPEN_FAIL]: Failed to listen\n";
			return last_error;
		}

		listened = true;
		last_error = OK;
		last_error_str = "OK: Listen for connection\n";

		return last_error;
	}

	Error LL<SERIAL, SERVER>::acceptConnection(void) {

		socklen_t clilen;

		// Make sure the listen() operation has been executed
		if (this->listened == false) {
			last_error = ACCEPT_FAIL;
			last_error_str = "ERROR[ACCEPT_FAIL]: Should listen before accepting\n";
			return last_error;
		}

		clilen = sizeof(this->listen_serv_addr);

		// Accept connection. This blocks the thread's execution until it returns.
		this->connect_socket_fd = accept(this->listen_socket_fd, (struct sockaddr*) &this->connect_serv_addr, &clilen);

		// Check if accept() returned a valid file descriptor
		if (this->connect_socket_fd <= 0) {
			last_error = ACCEPT_FAIL;
			last_error_str = "ERROR[ACCEPT_FAIL]: Failed acceptance\n";
			return last_error;
		}

		// Signal connection established
		this->connected = true;
		last_error = OK;
		last_error_str = "OK: Accept connection\n";

		return last_error;
	}


}


#endif		// _LINUX_ 