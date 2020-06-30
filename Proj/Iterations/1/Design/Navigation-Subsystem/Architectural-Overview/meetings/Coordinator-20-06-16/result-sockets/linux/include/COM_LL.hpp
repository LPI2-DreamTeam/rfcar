#ifndef COM_LL_H
#define COM_LL_H

#include "main.hpp"
#include "Mutex.hpp"

#ifdef _LINUX_

#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory>

namespace COM {


	typedef enum Protocol_T {
		VIRTUAL=0, BLUETOOTH_SERVER, SERIAL_CLIENT, SERIAL_SERVER
	} Protocol;

	typedef enum Error_T {
		OK = 0, DEAD, NOT_CONNECTED, READ_FAIL, WRITE_FAIL, CLOSE_FAIL, OPEN_FAIL
	} Error;



	template<Protocol protocol = VIRTUAL>
	class LL {

		static uint32_t object_count;

	public:		// Public methods
		
		LL();
		virtual ~LL();
		virtual Error read_str(char* p_buff, uint32_t len) = 0;
		virtual Error write_str(const char* p_buff, uint32_t len) = 0;
		virtual bool isConnected(void) = 0;
		virtual void kill(void) = 0;
		virtual Error closeConnection(void) = 0;
		virtual Error getLastError(std::string& emsg) = 0;
		virtual Error getLastError() = 0;
	};


//////////////////////////////// SERIAL_CLIENT ////////////////////////////////

	template <>
	class LL<SERIAL_CLIENT> : public LL<> {
	
	private:	// Private members

		OS::Mutex mutex;
		bool connected;
		Error last_error;
		std::string last_error_str;
		std::string ip_address;
		bool dead;

#ifdef _LINUX_
		uint32_t sock_type;
		uint32_t sock_family;
		uint32_t sock_fd = -1;
		uint32_t sock_port = -1;
		struct sockaddr_in serv_addr;
#endif

	public:		// Public methods: override

		LL();
		~LL() override;

		Error read_str(char* p_buff, uint32_t len) override;
		Error write_str(const char* p_buff, uint32_t len) override;

		bool isConnected(void) override;
		Error closeConnection(void) override;

		void kill(void) override;

		Error getLastError(std::string& emsg) override;
		Error getLastError() override;

	public:		// Public methods: override

		bool openConnection(std::string port);

	};
	


//////////////////////////////// SERIAL_SERVER ////////////////////////////////

	template <>
	class LL<SERIAL_SERVER> : public LL<> {
	
	private:	// Private members

		OS::Mutex mutex;
		bool connected;
		Error last_error;
		std::string last_error_str;
		std::string ip_address;
		bool dead;

#ifdef _LINUX_
		uint32_t sock_type;
		uint32_t sock_family;
		uint32_t sock_fd = -1;
		uint32_t Sock_port = -1;
		struct sockaddr_in serv_addr;
		uint32_t opt;
#endif

	public:		// Public methods: override

		LL();
		~LL() override;

		Error read_str(char* p_buff, uint32_t len) override;
		Error write_str(const char* p_buff, uint32_t len) override;

		bool isConnected(void) override;
		Error closeConnection(void) override;

		void kill(void) override;

		Error getLastError(std::string& emsg) override;
		Error getLastError() override;

	public:		// Public methods: specific
		
		Error getClientIP(std::string& ip);
		Error getConnectionPort(std::string& port);
		bool listenConnection(void);
		void acceptConnection(void);
	};

}

#endif		// _LINUX_

#endif