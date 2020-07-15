#ifndef COM_LL_H
#define COM_LL_H

#include "main.hpp"
#include "COM.hpp"
#include "OS_Mutex.hpp"

#define TCP_AVAILABLE_PORTS 5
#define BLUETOOTH_AVAILABLE_PORTS 4

#ifdef _LINUX_

#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory>
#include <fcntl.h>

// AF_UNIX is used for communication between processes in the same machine efficiently
#define TCP_SOCKET_FAMILY	AF_INET
// SOCK_STREAM Provides sequenced, reliable, two-way, connection-based byte streams
#define TCP_SOCKET_TYPE		SOCK_STREAM
// Prefix for the tcp communication servers
#define TCP_SERVERS_PREFIX	"tcp"
// Default port to be assigned to a tcp object when none is provided
#define TCP_DEFAULT_PORT		0

// AF_UNIX is used for communication between processes in the same machine efficiently
#define BLUETOOTH_SOCKET_FAMILY		AF_BLUETOOTH
// SOCK_STREAM Provides sequenced, reliable, two-way, connection-based byte streams
#define BLUETOOTH_SOCKET_TYPE		SOCK_STREAM
// Default port to be assigned to a bluetooth object when none is provided
#define BLUETOOTH_DEFAULT_PORT		0

// Folder where the socket files are hosted
#define SERVERS_FOLDER		"/tmp/servers/"

#endif		// _LINUX_

namespace COM {

//////////////////////////////// INTERFACE /////////////////////////////////////////////////////////////////////////////////////////

	/*! @brief LL class interface 
	*/
	template<Protocol protocol = INTERFACE, Role role = VIRTUAL>
	class LL {

	public:		// Public methods
		
		LL();
		virtual ~LL();
		
		/*! @brief Read a certain amount of characters from the buffer		
		*
		*	@param p_buff buffer to read from
		*	@param len number of characters to read
		*
		*	@return  General result code from the function's execution
		*/
		virtual Error readStr(char* p_buff, uint32_t len) = 0;
		
		/*! @brief Write a certain amount of characters to the buffer		
		*
		*	@param p_buff Buffer to write to
		*	@param len Number of characters to write
		*
		*	@return General result code from the function's execution
		*/
		
		virtual Error writeStr(const char* p_buff, uint32_t len) = 0;
		
		/*! @brief Verify connection state
		*/
		virtual bool isConnected(void) = 0;
		
		/*! @brief Disable message send and reception
		*/
		virtual void kill(void) = 0;

		/*! @brief Close connection
		*
		*	@return General result code from the function's execution
		*/
		virtual Error closeConnection(void) = 0;

		/*! @brief Get the result from the last function execution
		*	
		*	@param error_message Human-readable and more specific error message
		*
		*	@return General result code from the last function execution
		*/
		virtual Error getLastError(std::string& error_message) = 0;
		
		/*! @brief Get the result from the last function execution
		*
		*	@return General result code from the last function execution
		*/
		virtual Error getLastError() = 0;
	};



//////////////////////////////// TCP, CLIENT ////////////////////////////////////////////////////////////////////////////////////

	template <>
	class LL<TCP, CLIENT> : public LL<> {
	
	private:	// Private members

		static bool port_occupation[TCP_AVAILABLE_PORTS + 1];

		OS::Mutex mutex;	/*! Mutex meant to help keep functions of the same entity from executing at the same time*/
		bool connected;		/*! Connection state */
		bool dead;			/*! Whether shutdown() has been called or not */
		Error last_error;	/*! Result from the execution of the last function */
		std::string last_error_str;	/*! Result from the execution of the last function */
		int32_t port;		/*! Number that identifies the object in light of what hardware it is using. As such, it must be 
							*   unique. Must be between 1 and TCP_AVAILABLE_PORTS */

#ifdef _LINUX_
		int32_t connect_socket_fd = -1;
		int32_t sock_port = -1;
		struct sockaddr_in server_address;
#endif

	public:		// Public methods: override

		LL();
		~LL() override;

		Error readStr(char* p_buff, uint32_t len) override;
		Error writeStr(const char* p_buff, uint32_t len) override;
		bool isConnected(void) override;
		Error closeConnection(void) override;
		void kill(void) override;
		Error getLastError(std::string& error_message) override;
		Error getLastError() override;

	public:		// Public methods: specific

		/*! @brief Open a connection in the established port
		*
		*	@return Connection state
		*/
		bool openConnection(void);

		LL(int32_t port);

	};
	



//////////////////////////////// TCP, SERVER ////////////////////////////////////////////////////////////////////////////////////

	template <>
	class LL<TCP, SERVER> : public LL<> {
	
	private:	// Private members

		static bool port_occupation[TCP_AVAILABLE_PORTS + 1];

		OS::Mutex mutex;	/*! Mutex meant to help keep functions of the same entity from executing at the same time*/
		bool connected;		/*! Connection state */
		Error last_error;	/*! Result from the execution of the last function */
		std::string last_error_str;	/*! Result from the execution of the last function */
		bool dead;			/*! True kill() has been called or the object was poorly configured*/
		bool listened;		/*! Whether or not listenConnection() has been called */
		uint32_t port;		/*! Number that identifies the object in light of what hardware it is using. As such, it must be 
							*   unique. Must be between 1 and TCP_AVAILABLE_PORTS */


#ifdef _LINUX_
		int32_t connect_socket_fd = -1;	/*! File descriptor for the connection socket */
		int32_t listen_socket_fd = -1;	/*! File descriptor for the listening socket */
		int32_t sock_port = -1;			/*! Socket port */
		struct sockaddr_in connect_serv_addr;	/*! Address of the socket used during communications */
		struct sockaddr_in listen_serv_addr;	/*! Address of the socket used for listening */
#endif

	public:		// Public methods: override

		LL();
		~LL() override;

		Error readStr(char* p_buff, uint32_t len) override;
		Error writeStr(const char* p_buff, uint32_t len) override;
		bool isConnected(void) override;
		Error closeConnection(void) override;
		void kill(void) override;
		Error getLastError(std::string& error_message) override;
		Error getLastError() override;

	public:		// Public methods: specific
		
		LL(int32_t port);

		/*! @brief Listen for a connection
		*	
		*	@return General result code from the function's execution
		*/
		Error listenConnection(void);

		/*! @brief Accept connection
		*	
		*	@return General result code from the function's execution
		*/
		Error acceptConnection(void);
	};




//////////////////////////////// BLUETOOTH, CLIENT /////////////////////////////////////////////////////////////////////////////////

//	template <>
//	class LL<BLUETOOTH, SERVER> : public LL<> {
//	
//	private:	// Private members
//
//		static bool port_occupation[BLUETOOTH_AVAILABLE_PORTS + 1];
//
//		OS::Mutex mutex;	/*! Mutex meant to help keep functions of the same entity from executing at the same time*/
//		bool connected;		/*! Connection state */
//		Error last_error;	/*! Result from the execution of the last function */
//		std::string last_error_str;	/*! Result from the execution of the last function */
//		bool dead;			/*! True kill() has been called or the object was poorly configured*/
//		bool listened;			/*! Whether or not listenConnection() has been called */
//		
//		uint32_t port;		/*! Number that identifies the object in light of what hardware it is using. As such, it must be 
//							*   unique. Must be between 1 and TCP_AVAILABLE_PORTS */
//		
//
//#ifdef _LINUX_
//		int32_t tcp_fd = -1;	/*! File descriptor for the socket */
//#endif
//
//	public:		// Public methods: override
//
//		LL() : LL(BLUETOOTH_DEFAULT_PORT) {}
//		~LL() override;
//
//		Error readStr(char* p_buff, uint32_t len) override;
//		Error writeStr(const char* p_buff, uint32_t len) override;
//		bool isConnected(void) override;
//		Error closeConnection(void) override;
//		void kill(void) override;
//		Error getLastError(std::string& error_message) override;
//		Error getLastError() override;
//
//	public:		// Public methods: specific
//		
//		LL(int32_t port);
//
//		/*! @brief Listen for a connection
//		*	
//		*	@return General result code from the function's execution
//		*/
//		Error listenConnection(void);
//
//		/*! @brief Accept connection
//		*	
//		*	@return General result code from the function's execution
//		*/
//		Error acceptConnection(void);
//	};
//	

}

#endif
