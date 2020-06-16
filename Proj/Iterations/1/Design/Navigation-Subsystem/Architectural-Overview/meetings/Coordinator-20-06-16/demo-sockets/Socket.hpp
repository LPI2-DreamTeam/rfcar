#pragma once
#include <sstream>
#ifdef __MSVC__

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#else
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory>

#endif //__MSVC__



#include<mutex>

namespace networking {

	uint8_t checksum_calc(void* pbuff, int len);

	typedef  enum socket_qualifier { SockInterface = 0, Server, Client } sock_type_t;

	template <sock_type_t Soc = SockInterface>
	class CSocket
	{

	public:
		CSocket();
		virtual ~CSocket();
		virtual int safe_read(char* p_buff, int len) = 0;
		virtual int safe_write(const char* p_buff, int len) = 0;
		virtual bool is_connected(void) = 0;
		virtual bool kill(void) = 0;
		virtual  int close_connection(void) = 0;
		virtual bool open_connection(void) = 0;
		virtual size_t get_error_message(std::string& emsg) = 0;
	};


	template <>
	class CSocket<Client> : public CSocket<> {
		std::mutex SockMutex;
		bool Connected;
		std::string IpAddress;
		bool Shutdown;
		std::stringstream ErrorMessage;
#ifdef __MSVC__

		std::string PortString;


		SOCKET ConnectSocket;

		struct addrinfo hints;
#else
		int SockType;
		int SockFamily;
		int sock_fd = -1;
		int Sock_Port = -1;
		struct sockaddr_in serv_addr;
#endif //__MSVC__
	public:
		CSocket();
		~CSocket()override;
		int safe_read(char* p_buff, int len)override;
		int safe_write(const char* p_buff, int len)override;
		bool is_connected(void)override;
		bool kill(void)override;
		int close_connection(void)override;
		void set_ip_address(std::string& ip);
		void set_sock_type(int type);
		void set_sock_family(int soc_fam);
		void set_sock_port(int port);
		bool open_connection(void) override;
		//const char* error_message(void)override;
		size_t get_error_message(std::string& emsg)override;
	};

	template<>
	class CSocket<Server> :public CSocket<> {
		std::mutex SockMutex;
		bool Connected;
		bool Shutdown;
		std::string IpAddress;
		std::string ClientIp;
		std::stringstream ErrorMessage;

#ifdef __MSVC__

		std::string PortString;


		SOCKET ListenSocket;
		SOCKET ClientSocket;
		struct addrinfo hints;
		CSocket(SOCKET client, struct addrinfo* p_server_addr);
		CSocket(SOCKET client, SOCKADDR_IN* client_info);
#else
		int SockType;
		int SockFamily;
		int sock_fd = -1;
		int Sock_Port = -1;
		struct sockaddr_in serv_addr;
		int opt;

		CSocket(int fd, struct sockaddr_in* p_server_addr);
#endif //__MSVC__









	public:

		CSocket();

		~CSocket() override;

		int safe_read(char* p_buff, int len)override;
		int safe_write(const char* p_buff, int len)override;

		bool is_connected(void)override;
		bool kill(void)override;
		void set_sock_type(int type);

		void set_sock_family(int soc_fam);

		void set_sock_port(int port);

		bool open_connection(void)override;

		bool listen_connection(void);

		std::shared_ptr<CSocket<Server>> accept_connection(void);

		int close_connection(void)override;
		int get_client_ip(char* s_ip);
		int get_connection_port(char* s_port);
		//const char* error_message(void) override;
		size_t get_error_message(std::string& emsg)override;
	};

}

