#include "COM_LL.hpp"

#ifdef _LINUX_

#include <iostream>
#include <sstream>
#include <atomic>
#include <algorithm>
#include <thread>
#include <cerrno>
#include <unistd.h>

#include <termios.h> // Contains POSIX terminal control definitions

// AF_UNIX is used for communication between processes in the same machine efficiently
#define SERIAL_SOCKET_FAMILY	AF_UNIX
// SOCK_STREAM Provides sequenced, reliable, two-way, connection-based byte streams
#define SERIAL_SOCKET_TYPE		SOCK_STREAM
// Prefix for the serial communication servers
#define SERIAL_SERVERS_PREFIX	"serial"
// Default port to be assigned to a serial object when none is provided
#define SERIAL_DEFAULT_PORT		0

// AF_UNIX is used for communication between processes in the same machine efficiently
#define BLUETOOTH_SOCKET_FAMILY		AF_BLUETOOTH
// SOCK_STREAM Provides sequenced, reliable, two-way, connection-based byte streams
#define BLUETOOTH_SOCKET_TYPE		SOCK_STREAM
// Prefix for the serial communication servers
#define BLUETOOTH_SERVERS_PREFIX	"bluetooth"
// Default port to be assigned to a bluetooth object when none is provided
#define BLUETOOTH_DEFAULT_PORT		0

// Folder where the socket files are hosted
#define SERVERS_FOLDER		"/tmp/servers/"



namespace COM {
	

	template<Protocol protocol, Role role>
	LL<protocol, role>::LL() {

	}

	template<Protocol protocol, Role role>
	LL<protocol, role >::~LL() {

	}
}



//////////////////////////////// SERIAL, CLIENT ////////////////////////////////////////////////////////////////////////////////////

namespace COM {

	bool LL<SERIAL, CLIENT>::port_occupation[SERIAL_AVAILABLE_PORTS + 1] = {0};
	
	LL<SERIAL, CLIENT>::LL(int32_t port) {

		this->port = port;

		if (port > 0 && port <= SERIAL_AVAILABLE_PORTS) {
			
			if (port_occupation[port] == true) {
				this->dead = true;
				last_error = INVALID_PORT;
				last_error_str = "ERROR[INVALID_CONFIG]: The object was given an valid port number that was already taken\n";
			} else {
				this->dead = false;
				last_error = OK;
				last_error_str = "OK: The object was correctly configured\n";
				port_occupation[port] = true;
			}
		} else {
			this->dead = true;
			last_error = INVALID_PORT;
			last_error_str = "ERROR[INVALID_CONFIG]: The object was given an invalid port number\n";
		}

		this->connected = false;
		this->dead = false;

		this->connect_socket_fd = -1;
	}

	LL<SERIAL, CLIENT>::LL() : LL(SERIAL_DEFAULT_PORT) {

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

		std::string server_path = SERVERS_FOLDER SERIAL_SERVERS_PREFIX + std::to_string(this->port);
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





//////////////////////////////// SERIAL, SERVER ////////////////////////////////////////////////////////////////////////////////////

namespace COM {

	bool LL<SERIAL, SERVER>::port_occupation[SERIAL_AVAILABLE_PORTS + 1] = {0};
	
	LL<SERIAL, SERVER>::LL(int32_t port) {
		
		this->port = port;

		if (port > 0 && port <= SERIAL_AVAILABLE_PORTS) {
			
			if (port_occupation[port] == true) {
				this->dead = true;
				last_error = INVALID_PORT;
				last_error_str = "ERROR[INVALID_CONFIG]: The object was given an valid port number that was already taken\n";
			} else {
				this->dead = false;
				last_error = OK;
				last_error_str = "OK: The object was correctly configured\n";
				port_occupation[port] = true;
			}
		} else {

			this->dead = true;
			last_error = INVALID_PORT;
			last_error_str = "ERROR[INVALID_CONFIG]: The object was given an invalid port number\n";
		}
		
		this->connected = false;	
		this->listened = false;
		
	}

	LL<SERIAL, SERVER>::LL() : LL(SERIAL_DEFAULT_PORT) {
		
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

		std::string server_path = SERVERS_FOLDER SERIAL_SERVERS_PREFIX + std::to_string(this->port);
		std::string command = "mkdir -p " SERVERS_FOLDER;
		std::unique_lock<std::mutex>lock(this->mutex.native());

		// There is already a socket file descriptor atributed
		if (this->listen_socket_fd > 0) {
			last_error = ALREADY_OPEN;
			last_error_str = "ERROR[ALREADY_OPEN]: There is already a socket file descriptor atributed\n";
			return last_error;
		}

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
		last_error_str = "OK: Listening for connection\n";

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
			last_error_str = "ERROR[ACCEPT_FAIL]: Failed acceptance (invalid file descriptor)\n";
			return last_error;
		}

		// Signal connection established
		this->connected = true;
		last_error = OK;
		last_error_str = "OK: Acepted connection\n";

		return last_error;
	}


}


//////////////////////////////// BLUETOOTH, SERVER /////////////////////////////////////////////////////////////////////////////////

namespace COM {

	bool LL<BLUETOOTH, SERVER>::port_occupation[BLUETOOTH_AVAILABLE_PORTS + 1] = {0};
	
	LL<BLUETOOTH, SERVER>::LL(int32_t port) {
		
		this->port = port;

		if (port > 0 && port <= BLUETOOTH_AVAILABLE_PORTS) {
			
			if (port_occupation[port] == true) {
				this->dead = true;
				last_error = INVALID_PORT;
				last_error_str = "ERROR[INVALID_CONFIG]: The object was given an valid port number that was already taken\n";
			} else {
				this->dead = false;
				last_error = OK;
				last_error_str = "OK: The object was correctly configured\n";
				port_occupation[port] = true;
			}
		} else {

			this->dead = true;
			last_error = INVALID_PORT;
			last_error_str = "ERROR[INVALID_CONFIG]: The object was given an invalid port number\n";
		}
		
		this->connected = false;
		this->listened = false;
		
	}


	LL<BLUETOOTH, SERVER>::~LL() {

	}

	Error LL<BLUETOOTH, SERVER>::readStr(char * p_buff, uint32_t len) {
		
		std::unique_lock<std::mutex>lock(this->mutex.native());

		if (!this->connected) {
			last_error = NOT_CONNECTED;
			last_error_str = "ERROR[NOT_CONNECTED]: Not connected\n";

		} else if (this->dead) {
			last_error = DEAD;
			last_error_str = "ERROR[DEAD]: Object is dead\n";
			
		} else if (read(this->serial_fd, p_buff, len) < 0) {
			last_error = READ_FAIL;
			last_error_str = "ERROR[READ_FAIL]: Failed reading from socket\n";
		
		} else {
			last_error = OK;
			last_error_str = "OK: Read string\n";
		}

		return last_error;
	}

	Error LL<BLUETOOTH, SERVER>::writeStr(const char * p_buff, uint32_t len) {

		std::unique_lock<std::mutex>lock(this->mutex.native());

		if (!this->connected) {
			last_error = NOT_CONNECTED;
			last_error_str = "ERROR[NOT_CONNECTED]: Not connected\n";

		} else if (this->dead) {
			last_error = DEAD;
			last_error_str = "ERROR[DEAD]: Object is dead\n";
			
		} else if (write(this->serial_fd, p_buff, len) < 0) {
			last_error = WRITE_FAIL;
			last_error_str = "ERROR[WRITE_FAIL]: Failed writing to socket\n";
		
		} else {
			last_error = OK;
			last_error_str = "OK: Write string\n";
		}

		return last_error;
	}

	bool LL<BLUETOOTH, SERVER>::isConnected(void)  {

		std::unique_lock<std::mutex>lock(this->mutex.native());

		return this->connected;
	}

	void LL<BLUETOOTH, SERVER>::kill(void) {

		std::unique_lock<std::mutex>lock(this->mutex.native());

		shutdown(this->serial_fd, SHUT_RDWR);
		this->dead = true;
	}

	Error LL<BLUETOOTH, SERVER>::closeConnection(void) {

		std::unique_lock<std::mutex>lock(this->mutex.native());

		if (this->connected) {

			if (close(this->serial_fd) < 0) {
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

	Error LL<BLUETOOTH, SERVER>::getLastError(std::string& error_message) {

		error_message = last_error_str;
		return last_error;
	}

	Error LL<BLUETOOTH, SERVER>::getLastError() {

		return last_error;
	}

	Error LL<BLUETOOTH, SERVER>::listenConnection(void) {

		std::unique_lock<std::mutex>lock(this->mutex.native());
		std::string serial_port = "/dev/ttyS" + std::to_string(port - 1);

		this->serial_fd = open(serial_port.c_str(), O_RDWR);

		// Check for errors
		if (serial_fd < 0) {
			last_error = OPEN_FAIL;
			last_error_str = "ERROR[OPEN_FAIL]: Failed to open " + serial_port + "\n";
			return last_error;
		}

		// Create new termios struct, we call it 'tty' for convention
		struct termios tty;
		memset(&tty, 0, sizeof tty);

		// Read in existing settings, and handle any error
		if(tcgetattr(serial_fd, &tty) != 0) {
			last_error = OPEN_FAIL;
			last_error_str = "ERROR[OPEN_FAIL]: Failed to get port attributes\n";
			return last_error;
		}

		tty.c_cflag &= ~PARENB; // Disable parity
		tty.c_cflag &= ~CSTOPB; // One stop bit 
		tty.c_cflag |= CS8; 	// 8 bits per bytetty.c_cflag |= CS8; // 8 bits per byte
		tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control
		tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines

		tty.c_lflag &= ~ICANON; // DIsable canonical mode (not wait for \n)
		tty.c_lflag &= ~ECHO; // Disable echo
		tty.c_lflag &= ~ECHOE; // Disable erasure
		tty.c_lflag &= ~ECHONL; // Disable new-line echo
		tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP

		tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
		tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
		
		tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
		tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

		tty.c_cc[VTIME] = 0; 
		tty.c_cc[VMIN] = 0;

		// Set in/out baud rate to be 115200
		cfsetispeed(&tty, B115200);
		cfsetospeed(&tty, B115200);

		// Save tty settings, also checking for error
		if (tcsetattr(serial_fd, TCSANOW, &tty) != 0) {
			last_error = OPEN_FAIL;
			last_error_str = "ERROR[OPEN_FAIL]: Failed to Set desired port attributes\n";
			return last_error;
		}

		listened = true;
		last_error = OK;
		last_error_str = "OK: Listening for connection\n";

		return last_error;
		
	}

	Error LL<BLUETOOTH, SERVER>::acceptConnection(void) {

		std::unique_lock<std::mutex>lock(this->mutex.native());

		// Make sure the listen() operation has been executed
		if (this->listened == false) {
			last_error = ACCEPT_FAIL;
			last_error_str = "ERROR[ACCEPT_FAIL]: Should listen before accepting\n";
			return last_error;
		}

		// HANDSHAKE

		// Signal connection established
		this->connected = true;
		last_error = OK;
		last_error_str = "OK: Acepted connection\n";

		return last_error;
	}



}


#endif		// _LINUX_ 