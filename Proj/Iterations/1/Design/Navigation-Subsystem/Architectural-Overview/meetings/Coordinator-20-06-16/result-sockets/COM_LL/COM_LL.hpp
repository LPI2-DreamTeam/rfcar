#ifndef COM_LL_H
#define COM_LL_H

#include "main.hpp"

#ifdef __WINDOWS__

#include <WinSock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#include <string>
#include <iostream>

#endif

#include "Mutex.hpp"

namespace COM {


	typedef enum Protocol_T {
		VIRTUAL=0, BLUETOOTH_SERVER, SERIAL_CLIENT, SERIAL_SERVER
	} Protocol;

	typedef enum Error_T {
		OK = 0, INVALID_PROTOCOL, SERIAL_OPEN_CONNECTION_FAIL
	} Error;



	template<Protocol protocol = VIRTUAL>
	class LL {

		static uint32_t object_count;

	public:		// Public methods
		
		LL();
		virtual ~LL();
		virtual Error read(char* p_buff, int len) = 0;
		virtual Error write(const char* p_buff, int len) = 0;
		virtual bool isConnected(void) = 0;
		virtual bool kill(void) = 0;
		virtual Error closeConnection(void) = 0;
		virtual Error getLastError(std::string& emsg) = 0;
	};


//////////////////////////////// SERIAL_CLIENT ////////////////////////////////

	template <>
	class LL<SERIAL_CLIENT> : public LL<> {
	
	private:	// Private members

		OS::Mutex mutex;
		bool connected;
		Error last_error;
		std::string ip_address;

#ifdef __WINDOWS__
		std::string port;
		SOCKET connect_socket;
		struct addrinfo hints;
#endif

	public:		// Public methods: override

		LL();
		~LL() override;

		Error read(char* p_buff, int len) override;
		Error write(const char* p_buff, int len) override;

		bool isConnected(void) override;
		Error closeConnection(void) override;

		bool kill(void) override;

		Error getLastError(std::string& emsg) override;


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
		std::string ip_address;

#ifdef __WINDOWS__
		std::string port;
		SOCKET client_socket;
		SOCKET listen_socket;
		struct addrinfo hints;
#endif

	public:		// Public methods: override

		LL();
		~LL() override;

		Error read(char* p_buff, int len) override;
		Error write(const char* p_buff, int len) override;

		bool isConnected(void) override;
		Error closeConnection(void) override;

		bool kill(void) override;

		Error getLastError(std::string& emsg) override;
	
	public:		// Public methods: specific
		
		Error getClientIP(std::string& ip);
		Error getConnectionPort(std::string& port);
		bool listenConnection(void);
		void acceptConnection(void);
	};

}


#endif