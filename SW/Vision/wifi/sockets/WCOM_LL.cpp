#include "WCOM_LL.hpp"

#ifdef _LINUX_

#include <iostream>
#include <sstream>
#include <atomic>
#include <algorithm>
#include <thread>
#include <cerrno>
#include <unistd.h>

#include <termios.h> // Contains POSIX terminal control definitions


namespace WCOM {
	

    template<Protocol protocol, Role role>
    LL<protocol, role>::LL() {

    }

    template<Protocol protocol, Role role>
    LL<protocol, role >::~LL() {

    }
}



//////////////////////////////// TCP, CLIENT ////////////////////////////////////////////////////////////////////////////////////

 /* The sequence of events is:
 *  1. Create a TCP socket with the domain, type, and protocol.
 *  2. Connect to the remote server: connect is called by the client to connect to a server
 *     port.
 *  3. Send()/Recv() or Write()/Read() syscalls are used to send and receive data.
 *  6. Close the connection when the client disconnects and return to accept() mode
 *     to wait for new connect() calls from client
 *
 * Syscalls prototypes: The syscalls mentioned are present in pretty much
 * all *nix systems, like Linux systems and MacOS (FreeBSD). However, they differ in syntax,
 * namely referring to the constants.
 * The best way to use them is see the man pages for the syscalls on both systems.
 * So, for the server side, that will be run on the raspberry, the Linux version should be
 * used and in the client side, MacOS version should be used. This can be probably solved
 * easily with #ifdef/#ifnded/#define directives to get info from the system and change
 * the prototypes accordingly.
 * So the SYSCALLS prototypes for Linux version are:
 *
 *  - int Socket(int domain, int type, int protocol)
 *  	- domain = AF_INET (IPv4 communication)
 *  	- type = SOCK_STREAM(TCP), SOCK_DGRAM(UDP)
 *  	- protocol = 0
 *
 *  	The call returns a socket descriptor on success and -1 on an error.
 *
 *  - int Connect(int sockfd, (struct sockaddr *)&server_addr, socklen_t len)
 *  	- sockfd = a socket returned by socket()
 *  	- server_addr = a sockaddr_in struct pointer filled with all the remote server
 *        details and cast as a sockaddr struct pointer
 *      - len = size of the server_addr struct
 *
 *  - int send(int con_socket, const void *msg, size_t len, int flags)
 *  - int recv(int con_socket, const void *msg, size_t len, int flags) 
 *  	- con_socket = a connect socket
 *  	- msg = pointer to a buffer to send/receive data from/in
 *  	- len = size of the message buffer
 *  	- flags = 0 (for our purpose)
 *
 *  	The return value is the nr. of bytes actually sent/received.
 *
 *  - int close(int sockfd);
 *
 * UTILITY FUNCTIONS:
 * One must specially careful with the byte ordering (aka endianess), so one does not get
 * "translations" mistakes. In that sense, one must remember two things:
 * 	1. The network byte ordering is always big endian.
 * 	2. Before sending data packets to the network, one must always convert it to 
 *         network byte ordering.
 * For this purpose, we use the following utility functions:
 * + Byte Ordering:
 *  - Host Byte Order to Network Byte Order:
 *  	- short: htons();
 *  	- long:  htonl().
 *  - Network Byte Order to Host Byte Order:
 *  	- short: ntohs();
 *  	- long:  ntohl();

 * + IP Adress format:
 *  - ASCII dotted to Binary: inet_aton()
 *  - Binary to ASCII dotted: inet_ntoa()
 * 
 * + Padding
 *  - bzero: write zeroes to a byte string
 *  	bzero(void *s, size_t n);
 *
 *----------------------------------------------------------- */

namespace WCOM {

    bool LL<TCP, CLIENT>::port_occupation[TCP_AVAILABLE_PORTS + 1] = {0};
    
    LL<TCP, CLIENT>::LL(int32_t port) {
        this->connected = false;
        this->dead = false;
        this->connect_socket_fd = -1;
    }

    //LL<TCP, CLIENT>::LL() : LL(TCP_DEFAULT_PORT) {

    //}

    LL<TCP, CLIENT>::~LL() {

		
    }

    Error LL<TCP, CLIENT>::readStr(char * p_buff, uint32_t len) {

        std::unique_lock<std::mutex>lock(this->mutex.native());

    /* Clear buffer for reception*/
        std::fill_n((char*)& p_buff, len, 0);
   
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

    Error LL<TCP, CLIENT>::writeStr(const char * p_buff, uint32_t len) {
		
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

    bool LL<TCP, CLIENT>::isConnected(void) {

        std::unique_lock<std::mutex>lock(this->mutex.native());

        return this->connected;
    }

    void LL<TCP, CLIENT>::kill(void) {

        std::unique_lock<std::mutex>lock(this->mutex.native());

        shutdown(this->connect_socket_fd, SHUT_RDWR);
        this->dead = true;
    }

    Error LL<TCP, CLIENT>::closeConnection(void) {

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

    bool LL<TCP, CLIENT>::Connect(const std::string &addr, int port) {
        
    /* Lock */
        std::unique_lock<std::mutex>lock(this->mutex.native());

        this->port = -1;
        if (port > 0 && port <= TCP_AVAILABLE_PORTS) {
            if (port_occupation[port] == true) {
                this->dead = true;
                last_error = INVALID_PORT;
                last_error_str = "ERROR[INVALID_CONFIG]: The object was given an valid port number that was already taken\n";
                return false;
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
            return false;
        }

// Check if the socket is already open
        if ( this->connected ) {
            last_error = ALREADY_OPEN;
            last_error_str = "ERROR[ALREADY_OPEN]: A connection with this server or another one is already open\n";
            return this->connected;
        }

        /* Store port */
        this->port = port;

/* Try to obtain address */
    // gethostbyname() version
    // takes a host name in the internet as an argument and returns a ptr to a
    // hostent containing information about that host. If the structure is NULL,
    // the system could not locate a host with this name.
	struct hostent *server = gethostbyname(addr.c_str()); // this is a pointer to a struct of type hostent, which defines a host computer in the internet.
        if(server == NULL){
            last_error = INVALID_ADDR;
            last_error_str = "ERROR[UNKOWN SERVER]: IP addr not recognized\n";
            return this->connected;
        }


        // Attempt to open socket
        this->connect_socket_fd = socket(TCP_SOCKET_FAMILY, TCP_SOCKET_TYPE, 0);

        // If this->connect_socket_fd == -1, it means that the socket could not be open and thus the file descriptor is invalid
        if (this->connect_socket_fd < 0) {
            last_error = OPEN_FAIL;
            last_error_str = "ERROR[OPEN_FAIL]: Failed to open socket\n";
            return this->connected;
        }

    // Filling the serv_addr struct. Because the field server->h_addr
    // is a char string we use the function:
    // 	void bcopy( char *s1, char *s2, int length);
    // which copies length bytes from s1 to s2
	//bzero( (char* ) &this->server_address, sizeof(this->server_address) );
        std::fill_n((char*)& this->server_address,
                    sizeof(this->server_address), 0);
	this->server_address.sin_family = TCP_SOCKET_FAMILY; //AF_INET; IPv4 fam
        // watch out for endianess
	this->server_address.sin_port = htons(this->port);



    // 2. Attempt to establish a connection: connect to remote server
        if (connect(this->connect_socket_fd,
                    (struct sockaddr*) &this->server_address,
                    sizeof(this->server_address)) < 0) {
            last_error = OPEN_FAIL;
            last_error_str = "ERROR[OPEN_FAIL]: Failed connecting\n";
            return this->connected;
        }

        this->connected = true;
		
        last_error = OK;
        last_error_str = "OK: Open connection\n";

        return this->connected;
    }

    Error LL<TCP, CLIENT>::getLastError(std::string& error_message) {
        error_message = last_error_str;
        return last_error;
    }

    Error LL<TCP, CLIENT>::getLastError() {
        return last_error;
    }
    Error LL<TCP, CLIENT>::getAddr(std::string &serverAddr) {
        if(this->connected)
            serverAddr = inet_ntoa(this->server_address.sin_addr);
        else
            serverAddr = "";
        return INVALID_ADDR;
    }
    int LL<TCP, CLIENT>::getPort() {
        if(this->connected)
            return this->port;
        else
            return -1;
        return INVALID_PORT;
    }
}





//////////////////////////////// TCP, SERVER ////////////////////////////////////////////////////////////////////////////////////

 /* The sequence of events is:
 *  1. Create a TCP socket with the domain, type, and protocol.
 *  2. Bind the TCP socket to a specific port and IP address
 *  3. Listen (wait) for a connection: the server process calls listen to tell the kernel
 *     to initialize a wait queue of connections for this socket.
 *  4. Accept a new connection: Accept is called by the server process to accept new
 *     connections from new clients trying to connect to the server.
 *  5. Send()/Recv() or Write()/Read() syscalls are used to send and receive data.
 *  6. Close the connection when the client disconnects and return to accept() mode
 *     to wait for new connect() calls from client
 *
 *
 *  There should also be a graceful way of restarting the server, since it will be run
 *  on the raspberry pi as a daemon, so it will be forever in accept mode. Probably a
 *  time-out to restart the service if it hangs, can be a solution.
 *
 * Syscalls prototypes: The syscalls mentioned are present in pretty much
 * all *nix systems, like Linux systems and MacOS (FreeBSD). However, they differ in syntax,
 * namely referring to the constants.
 * The best way to use them is see the man pages for the syscalls on both systems.
 * So, for the server side, that will be run on the raspberry, the Linux version should be
 * used and in the client side, MacOS version should be used. This can be probably solved
 * easily with #ifdef/#ifnded/#define directives to get info from the system and change
 * the prototypes accordingly.
 * So the SYSCALLS prototypes for Linux version are:
 *
 *  - int Socket(int domain, int type, int protocol)
 *  	- domain = AF_INET (IPv4 communication)
 *  	- type = SOCK_STREAM(TCP), SOCK_DGRAM(UDP)
 *  	- protocol = 0
 *
 *  	The call returns a socket descriptor on success and -1 on an error.
 *
 *  - int Bind(int sockfd, struct sockaddr *my_addr, socklen_t addrlen)
 *  	- sockfd = socket descriptor returned by socket()
 *  	- my_addr = pointer to a valid sockaddr_in structure cast as a sockaddr * pointer
 *  	- addrlen = length of the sockaddr_in structure
 *
 *  - int Listen(int sockfd, int backlog)
 *	- sockfd = socket descriptor returned by socket()
 *	- backlog = maximun length of the pending connections queue
 *
 *  - int Accept(int socket, (struct sockaddr *)&client, socklen_t *client_len)
 *  	- socket = socket in the listen state
 *  	- client = will hold the new client's information when accept returns
 *  	- client_len = pointer to size of the client structure
 *
 *  - int send(int con_socket, const void *msg, size_t len, int flags)
 *  - int recv(int con_socket, const void *msg, size_t len, int flags) 
 *  	- con_socket = a connect socket
 *  	- msg = pointer to a buffer to send/receive data from/in
 *  	- len = size of the message buffer
 *  	- flags = 0 (for our purpose)
 *
 *  	The return value is the nr. of bytes actually sent/received.
 *
 *  - int close(int sockfd);
 *
 * UTILITY FUNCTIONS:
 * One must specially careful with the byte ordering (aka endianess), so one does not get
 * "translations" mistakes. In that sense, one must remember two things:
 * 	1. The network byte ordering is always big endian.
 * 	2. Before sending data packets to the network, one must always convert it to 
 *         network byte ordering.
 * For this purpose, we use the following utility functions:
 * + Byte Ordering:
 *  - Host Byte Order to Network Byte Order:
 *  	- short: htons();
 *  	- long:  htonl().
 *  - Network Byte Order to Host Byte Order:
 *  	- short: ntohs();
 *  	- long:  ntohl();

 * + IP Adress format:
 *  - ASCII dotted to Binary: inet_aton()
 *  - Binary to ASCII dotted: inet_ntoa()
 * 
 * + Padding
 *  - bzero: write zeroes to a byte string
 *  	bzero(void *s, size_t n);
 *
 *----------------------------------------------------------- */
namespace WCOM {

    bool LL<TCP, SERVER>::port_occupation[TCP_AVAILABLE_PORTS + 1] = {0};
	
    LL<TCP, SERVER>::LL(int32_t port) {
		
        this->port = -1;
        if (port > 0 && port <= TCP_AVAILABLE_PORTS) {
			
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
		
        this->port = port;
        this->connected = false;	
        this->listened = false;
    }

    LL<TCP, SERVER>::LL() : LL(TCP_DEFAULT_PORT) {
		
    }

    LL<TCP, SERVER>::~LL() {

    }

    Error LL<TCP, SERVER>::readStr(char * p_buff, uint32_t len) {
		
        std::unique_lock<std::mutex>lock(this->mutex.native());

    /* Clear buffer for reception*/
        std::fill_n((char*)& p_buff, len, 0);

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

    Error LL<TCP, SERVER>::writeStr(const char * p_buff, uint32_t len) {
		
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

    bool LL<TCP, SERVER>::isConnected(void)  {

        std::unique_lock<std::mutex>lock(this->mutex.native());

        return this->connected;
    }

    void LL<TCP, SERVER>::kill(void) {

        std::unique_lock<std::mutex>lock(this->mutex.native());

        shutdown(this->listen_socket_fd, SHUT_RDWR);
        this->dead = true;
    }

    Error LL<TCP, SERVER>::closeConnection(void) {

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

    Error LL<TCP, SERVER>::getLastError(std::string& error_message) {

        error_message = last_error_str;
        return last_error;
    }

    Error LL<TCP, SERVER>::getLastError() {

        return last_error;
    }

    Error LL<TCP, SERVER>::listenConnection(void) {

        std::unique_lock<std::mutex>lock(this->mutex.native());

        // There is already a socket file descriptor atributed
        if (this->listen_socket_fd > 0) {
            last_error = ALREADY_OPEN;
            last_error_str = "ERROR[ALREADY_OPEN]: There is already a socket file descriptor atributed\n";
            return last_error;
        }

        // Request an endpoint for communication
        this->listen_socket_fd = socket(TCP_SOCKET_FAMILY, TCP_SOCKET_TYPE, 0);
	
        // If the socket fails to open
        if (this->listen_socket_fd < 0) {
            last_error = OPEN_FAIL;
            last_error_str = "ERROR[OPEN_FAIL]: Failed to open socket\n";
            return last_error;
        }

/* Reset array for addr */
        std::fill_n((char*)& this->listen_serv_addr,
                    sizeof(this->listen_serv_addr), 0);
		
	this->listen_serv_addr.sin_family = TCP_SOCKET_FAMILY; // IPv4 family
	this->listen_serv_addr.sin_port = htons(this->port); // Host TO Network Short
	this->listen_serv_addr.sin_addr.s_addr = INADDR_ANY; // listen to all interfaces (0.0.0.0) on the designated port

        // Path should be unlinked before a bind() call
        // https://stackoverflow.com/questions/17451971/getting-address-already-in-use-error-using-unix-socket
//        unlink(server_path.c_str());
//        int opt = 1;
//        if (setsockopt(this->listen_socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
//            last_error = OPEN_FAIL;
//            last_error_str = "ERROR[OPEN_FAIL]: Failed to set socket options\n";
//            return last_error;
//        }

        unsigned int sockaddr_len = sizeof(struct sockaddr_in);
        // Assign the address specified by addr to the socket referred to by the file descriptor sockfd
        if (bind(this->listen_socket_fd,
                 (struct sockaddr*) & this->listen_serv_addr,
                 sockaddr_len) < 0) {
			
            // Error feedback
            last_error = OPEN_FAIL;
            last_error_str = "ERROR[OPEN_FAIL]: Failed to bind\n"
                + std::string( strerror(errno) );
            return last_error;
        }

#define TCP_MAX_CLIENTS 5
        if (listen(this->listen_socket_fd, TCP_MAX_CLIENTS) < 0) {

            // Undo binding. Not absolutely necessary as it is done before the 'bind' instruction already.
//            unlink(server_path.c_str());

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

    Error LL<TCP, SERVER>::acceptConnection(void) {

        std::unique_lock<std::mutex>lock(this->mutex.native());

        socklen_t clilen;

        // Make sure the listen() operation has been executed
        if (this->listened == false) {
            last_error = ACCEPT_FAIL;
            last_error_str = "ERROR[ACCEPT_FAIL]: Should listen before accepting\n";
            return last_error;
        }

        clilen = sizeof(this->listen_serv_addr);
		
        // Accept connection. This blocks the thread's execution until it returns.
        this->connect_socket_fd =
            accept(this->listen_socket_fd,
                   (struct sockaddr*) &this->connect_serv_addr,
                   &clilen);

        // Check if accept() returned a valid file descriptor
        if (this->connect_socket_fd < 0) {
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

    Error LL<TCP, SERVER>::getAddr(std::string &serverAddr) {
        if(this->connected)
            serverAddr = inet_ntoa(this->connect_serv_addr.sin_addr);
        else
            serverAddr = "";
        return INVALID_ADDR;
    }
    int LL<TCP, SERVER>::getPort() {
        if(this->connected)
            return this->port;
        else
            return -1;
        return INVALID_PORT;
    }


}


#endif		// _LINUX_ 
