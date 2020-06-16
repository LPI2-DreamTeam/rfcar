
#define __MSVC__
#define _CRT_SECURE_NO_WARNINGS
#include "Socket.hpp"

#include <iostream>
#include <sstream>
#include <atomic>
#include <algorithm>
#include <thread>
#include <cerrno>


	namespace networking {

		uint8_t checksum_calc(void* pbuff, int len) {
			uint16_t result = 0;
			uint8_t ret;
			uint8_t* puint = (uint8_t*)pbuff;
			int i;
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
#ifdef __MSVC__
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")



		static std::atomic<int> soc_count = 0;
		static WSADATA wsaData;

		template<sock_type_t Soc>
		CSocket<Soc>::CSocket()
		{

			int iResult;
			if (++soc_count == 1) {
				iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
				if (iResult != 0) {
					printf("WSAStartup failed with error: %d\n", iResult);
					return;
				}
			}

		}


		template<sock_type_t Soc>
		CSocket<Soc>::~CSocket()
		{
			if (--soc_count == 0) {
				WSACleanup();
			}
		}

		CSocket<Client>::CSocket() {

			ZeroMemory(&this->hints, sizeof(struct addrinfo));
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;


			this->Connected = false;
			this->Shutdown = false;



		}

		CSocket<Client>::~CSocket() {


		}

		int CSocket<Client>::safe_read(char* p_buff, int len)
		{
			int ret = 0;
			std::unique_lock<std::mutex>lock(this->SockMutex);
			if (this->Connected == false) {
				this->ErrorMessage << __FUNCTION__ << ": not connected\n";
				return -ECONNREFUSED;
			}
			if (this->Shutdown) {
				this->ErrorMessage << __FUNCTION__ << ": shutdown initiated\n";
				return -ECONNABORTED;
			}
			ret = recv(this->ConnectSocket, p_buff, len, 0);
			if (ret < 0) {
				this->ErrorMessage << __FUNCTION__ << " error:" << std::strerror(ret) << "\n";
			}
			return ret;
		}

		int CSocket<Client>::safe_write(const char* p_buff, int len)
		{
			int ret = 0;
			std::unique_lock<std::mutex>lock(this->SockMutex);
			if (this->Connected == false) {
				this->ErrorMessage << __FUNCTION__ << ": not connected\n";
				return -ECONNREFUSED;
			}
			if (this->Shutdown) {
				this->ErrorMessage << __FUNCTION__ << ": shutdown initiated\n";
				return -ECONNABORTED;
			}
			ret = send(this->ConnectSocket, p_buff, len, 0);
			if (ret < 0) {
				this->ErrorMessage << __FUNCTION__ << " error:" << std::strerror(ret) << "\n";
			}
			return ret;
		}

		bool CSocket<Client>::kill(void)
		{
			this->Shutdown = true;
			shutdown(this->ConnectSocket, SD_BOTH);
			return Shutdown;
		}

		int CSocket<Client>::close_connection(void)
		{
			int iResult = -1;
			if (this->Connected) {
				iResult = shutdown(ConnectSocket, SD_BOTH);
				closesocket(ConnectSocket);

			}
			return iResult;
		}

		void CSocket<Client>::set_ip_address(std::string& ip)
		{
			this->IpAddress = ip;
		}

		void CSocket<Client>::set_sock_type(int type)
		{

			this->hints.ai_socktype = SOCK_STREAM;

		}

		void CSocket<Client>::set_sock_family(int soc_fam)
		{
			hints.ai_family = AF_UNSPEC;
		}

		void CSocket<Client>::set_sock_port(int port)
		{
			std::stringstream s;

			s << port;
			this->PortString = s.str();
		}

		bool CSocket<Client>::open_connection(void)
		{
			struct addrinfo* result;
			struct addrinfo* ptr;
			// Resolve the server address and port
			int iResult = getaddrinfo(this->IpAddress.c_str(), this->PortString.c_str(), &this->hints, &result);
			if (iResult != 0) {
				this->ErrorMessage << __FUNCTION__ << ":getaddrinfo failed with error: " << std::strerror(iResult) << "\n";

				return false;
			}


			// Attempt to connect to an address until one succeeds
			for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

				// Create a SOCKET for connecting to server
				this->ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
					ptr->ai_protocol);
				if (this->ConnectSocket == INVALID_SOCKET) {
					this->ErrorMessage << "socket failed with error: " << WSAGetLastError() << "\n";
					return false;
				}

				// Connect to server.
				iResult = connect(this->ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
				if (iResult == SOCKET_ERROR) {
					closesocket(this->ConnectSocket);
					this->ConnectSocket = INVALID_SOCKET;
					continue;
				}
				break;
			}

			freeaddrinfo(result);

			if (ConnectSocket == INVALID_SOCKET) {
				this->ErrorMessage << __FUNCTION__ << "Unable to connect to server!\n";

				return false;
			}

			this->Connected = true;

			return true;
		}

		bool CSocket<Client>::is_connected(void)
		{
			return this->Connected;
		}

		size_t CSocket<Client>::get_error_message(std::string& emsg)
		{
			size_t ret;
			emsg = this->ErrorMessage.str();
			ret = emsg.size();
			this->ErrorMessage = std::stringstream("");
			return ret;
		}




		CSocket<Server>::CSocket() {
			this->Connected = false;
			this->Shutdown = false;

			ZeroMemory(&this->hints, sizeof(struct addrinfo));
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;
			hints.ai_flags = AI_PASSIVE;
			this->ClientSocket = INVALID_SOCKET;
			this->ListenSocket = INVALID_SOCKET;
		}

		CSocket<Server>::~CSocket() {

		}


		int CSocket<Server>::safe_read(char* p_buff, int len) {
			int ret = 0;
			std::unique_lock<std::mutex>lock(this->SockMutex);

			if (this->Connected == false) {
				this->ErrorMessage << __FUNCTION__ << ": not connected\n";
				return -ECONNREFUSED;
			}
			if (this->Shutdown) {
				this->ErrorMessage << __FUNCTION__ << ": shutdown initiated\n";
				return -ECONNABORTED;
			}
			ret = recv(this->ClientSocket, p_buff, len, 0);
			if (ret <= 0) {
				this->Connected = false;
				if (ret < 0) {
					this->ErrorMessage << __FUNCTION__ << ": read error:" << std::strerror(ret) << "\n";
				}
			}
			return ret;
		}

		int CSocket<Server>::safe_write(const char* p_buff, int len) {
			int ret = 0;
			std::unique_lock<std::mutex>lock(this->SockMutex);

			if (this->Connected == false) {
				this->ErrorMessage << __FUNCTION__ << ": not connected\n";
				return -ECONNREFUSED;
			}
			if (this->Shutdown) {
				this->ErrorMessage << __FUNCTION__ << ": shutdown initiated\n";
				return -ECONNABORTED;
			}

			ret = send(this->ClientSocket, p_buff, len, 0);
			if (ret <= 0) {
				this->Connected = false;
				if (ret < 0)
					this->ErrorMessage << __FUNCTION__ << ": write error:" << std::strerror(ret) << "\n";

			}
			return ret;
		}

		bool CSocket<Server>::kill(void) {
			this->Shutdown = true;
			shutdown(this->ListenSocket, SD_BOTH);
			return false;
		}

		void CSocket<Server>::set_sock_type(int type) {
			this->hints.ai_socktype = type;
		}

		void CSocket<Server>::set_sock_family(int soc_fam) {
			this->hints.ai_family = soc_fam;
		}

		void CSocket<Server>::set_sock_port(int port) {
			std::stringstream s;
			s << port;
			this->PortString = s.str();
		}

		bool CSocket<Server>::listen_connection(void) {

			struct addrinfo* result = nullptr;
			// Resolve the server address and port
			int iResult = getaddrinfo(NULL, this->PortString.c_str(), &this->hints, &result);
			if (iResult != 0) {
				this->ErrorMessage << __FUNCTION__ << "getaddrinfo failed with error: " << iResult << "\n";

				return false;
			}

			// Create a SOCKET for connecting to server
			this->ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

			if (ListenSocket == INVALID_SOCKET) {
				this->ErrorMessage << __FUNCTION__ << "socket failed with error: " << WSAGetLastError() << "\n";
				freeaddrinfo(result);

				return false;
			}

			// Setup the TCP listening socket
			iResult = bind(this->ListenSocket, result->ai_addr, (int)result->ai_addrlen);
			if (iResult == SOCKET_ERROR) {
				this->ErrorMessage << __FUNCTION__ << "bind failed with error: " << WSAGetLastError() << "\n";
				freeaddrinfo(result);
				closesocket(ListenSocket);

				return false;
			}

			freeaddrinfo(result);

			iResult = listen(ListenSocket, SOMAXCONN);
			if (iResult == SOCKET_ERROR) {
				this->ErrorMessage << __FUNCTION__ << "listen failed with error: " << WSAGetLastError() << "\n";
				closesocket(ListenSocket);

				return false;
			}

			this->Connected = true;
			return true;

		}

		CSocket<Server>::CSocket(SOCKET client, struct addrinfo* p_server_addr)
		{
			this->ListenSocket = INVALID_SOCKET;
			this->Shutdown = false;

			std::copy_n((char*)p_server_addr, sizeof(p_server_addr), (char*)& this->hints);

			this->ClientSocket = client;

			this->Connected = true;

		}

		CSocket<Server>::CSocket(SOCKET client, SOCKADDR_IN* pclient_info)
		{
			this->ListenSocket = INVALID_SOCKET;
			this->Shutdown = false;
			PCSTR cip;
			char ipstr[INET6_ADDRSTRLEN];
			int infosize = sizeof(SOCKADDR_IN);
			this->ClientSocket = client;
			getpeername(ClientSocket, (struct sockaddr*)pclient_info, &infosize);

			cip = inet_ntop(pclient_info->sin_family, (const void*) & (pclient_info->sin_addr), (PSTR)ipstr, INET6_ADDRSTRLEN);

			this->ClientIp = std::string(cip);
			// this->PortString = std::to_string(pclient_info->sin_port);

			this->Connected = true;

		}
		std::shared_ptr<CSocket<Server>> CSocket<Server>::accept_connection(void) {

			SOCKET ClientSocket = INVALID_SOCKET;
			SOCKADDR_IN client_info;
			int infosize = sizeof(client_info);
			std::shared_ptr<CSocket> p_new = nullptr;
			if (this->Connected == false)
				return p_new;


			ClientSocket = accept(this->ListenSocket, (struct sockaddr*) & client_info, &infosize);

			if (ClientSocket == INVALID_SOCKET) {
				//LOG_MSG << "accept failed with error: " << WSAGetLastError() << "\n";
				closesocket(ListenSocket);

				this->Connected = false;
				return p_new;
			}
			// or get it from the socket itself at any time

			p_new = std::shared_ptr<CSocket<Server>>(new CSocket<Server>(ClientSocket, &client_info));
			this->ClientSocket = INVALID_SOCKET;
			return p_new;
		}

		int CSocket<Server>::close_connection(void) {
			int ret = 0;
			if (this->ListenSocket != INVALID_SOCKET) {
				shutdown(this->ListenSocket, SD_BOTH);
				ret = closesocket(this->ListenSocket);
			}
			if (this->ClientSocket != INVALID_SOCKET) {
				shutdown(this->ClientSocket, SD_BOTH);
				ret = closesocket(this->ClientSocket);
			}
			return ret;
		}

#else
		template<sock_type_t Soc>
		CSocket<Soc>::CSocket()
		{
		}

		template<sock_type_t Soc>
		CSocket<Soc>::~CSocket()
		{
		}



		CSocket<Client>::CSocket() {
			this->Connected = false;
			this->Shutdown = false;
			this->SockFamily = -1;
			this->SockType = -1;
			this->IpAddress = "";
		}

		CSocket<Client>::~CSocket()
		{
		}

		int CSocket<Client>::safe_read(char* p_buff, int len)
		{
			int ret;
			std::unique_lock<std::mutex>lock(this->SockMutex);
			if (this->Connected == false)
				return -ECONNREFUSED;

			ret = read(this->sock_fd, p_buff, len);
			if (ret < 0)
				perror("socket read");
			return ret;
		}

		int CSocket<Client>::safe_write(const char* p_buff, int len) {
			int ret;
			std::unique_lock<std::mutex>lock(this->SockMutex);
			if (this->Connected == false)
				return -ECONNREFUSED;

			ret = write(this->sock_fd, p_buff, len);
			if (ret < 0)
				perror("socket write");
			return ret;
		}


		bool CSocket<Client>::kill(void) {
			std::unique_lock<std::mutex>lock(this->SockMutex);
			this->Shutdown = true;
			//TODO:Kill

			return false;
		}

		int CSocket<Client>::close_connection(void) {
			int ret;
			if (this->Connected == false)
				return 0;
			ret = close(this->sock_fd);
			if (ret < 0) {
				perror("close failed");
			}
			this->Connected = false;
			return ret;
		}

		void CSocket<Client>::set_ip_address(std::string& ip) {
			this->IpAddress = ip;
		}

		void CSocket<Client>::set_sock_type(int type) {
			this->SockType = type;
		}

		void CSocket<Client>::set_sock_family(int soc_fam) {
			this->SockFamily = soc_fam;
		}

		void CSocket<Client>::set_sock_port(int port) {
			this->Sock_Port = port;
		}

		bool CSocket<Client>::open_connection(void) {

			int ret;

			if (this->sock_fd > 0) {
				return true;
			}


			this->sock_fd = socket(this->SockFamily, this->SockType, 0);
			if (this->sock_fd < 0) {
				perror("ERROR opening socket");
				return false;

			}


			std::fill_n((char*)& this->serv_addr, sizeof(this->serv_addr), 0);

			this->serv_addr.sin_family = this->SockFamily;
			serv_addr.sin_port = htons(this->Sock_Port);


			if (inet_pton(this->SockFamily, this->IpAddress.c_str(), &serv_addr.sin_addr) <= 0)
			{
				printf("\nInvalid address/ Address not supported \n");
				return false;
			}

			ret = connect(this->sock_fd, (struct sockaddr*) & this->serv_addr, sizeof(this->serv_addr));
			if (ret < 0) {
				perror("ERROR connecting");
				return false;
			}
			this->Connected = true;


			return this->Connected;

		}

		bool CSocket<Client>::is_connected(void) {
			std::unique_lock<std::mutex>lock(this->SockMutex);
			return this->Connected;
		}

		size_t CSocket<Client>::get_error_message(std::string& emsg)
		{
			size_t ret;
			emsg = this->ErrorMessage.str();
			ret = emsg.size();
			this->ErrorMessage = std::stringstream("");
			return ret;
		}

		CSocket<Server>::CSocket(int fd, sockaddr_in* p_server_addr) {

			std::copy_n((char*)p_server_addr, sizeof(p_server_addr), (char*)& this->serv_addr);
			this->sock_fd = fd;
			this->Connected = true;
		}

		CSocket<Server>::CSocket() {
			this->Connected = false;
			this->Shutdown = false;
			this->SockFamily = -1;
			this->SockType = -1;

			this->opt = 1;
		}

		int CSocket<Server>::safe_read(char* p_buff, int len) {
			int ret;
			std::unique_lock<std::mutex>lock(this->SockMutex);
			if (this->Connected == false)
				return -ECONNREFUSED;

			ret = read(this->sock_fd, p_buff, len);
			if (ret < 0)
				perror("socket write error");

			return ret;
		}

		int CSocket<Server>::safe_write(const char* p_buff, int len) {
			int ret;
			std::unique_lock<std::mutex>lock(this->SockMutex);
			if (this->Connected == false)
				return -ECONNREFUSED;

			ret = write(this->sock_fd, p_buff, len);
			if (ret < 0)
				perror("socket read error");
			return ret;
		}


		bool CSocket<Server>::kill(void) {
			std::unique_lock<std::mutex>lock(this->SockMutex);
			this->Shutdown = true;
			shutdown(this->sock_fd, SHUT_RDWR);
			return false;
		}

		void CSocket<Server>::set_sock_type(int type) {
			this->SockType = type;
		}

		void CSocket<Server>::set_sock_family(int soc_fam) {
			this->SockFamily = soc_fam;
		}

		void CSocket<Server>::set_sock_port(int port) {
			this->Sock_Port = port;
		}

		bool CSocket<Server>::listen_connection(void) {

			if (this->sock_fd > 0)
				return false;

			this->sock_fd = socket(this->SockFamily, this->SockType, 0);
			if (this->sock_fd < 0) {
				perror("ERROR opening socket");
				return false;
			}

			std::fill_n((char*)& this->serv_addr, sizeof(this->serv_addr), 0);

			this->serv_addr.sin_family = this->SockFamily;
			this->serv_addr.sin_addr.s_addr = INADDR_ANY;
			this->serv_addr.sin_port = htons(this->Sock_Port);

			// Forcefully attaching socket to the port 8080
			if (setsockopt(this->sock_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
			{
				perror("setsockopt");
				return false;
			}

			if (bind(this->sock_fd, (struct sockaddr*) & this->serv_addr, sizeof(this->serv_addr)) < 0)
			{
				perror("ERROR on binding");
				return false;
			}
			if (listen(this->sock_fd, 15) < 0)
			{
				perror("listen");
				return false;
			}

			this->Connected = true;
			return true;

		}

		std::shared_ptr<CSocket<Server>> CSocket<Server>::accept_connection(void) {

			struct sockaddr_in cli_addr;
			int ret;
			socklen_t clilen;
			std::shared_ptr<CSocket> p_new = nullptr;
			if (this->Connected == false)
				return p_new;

			clilen = sizeof(cli_addr);


			ret = accept(this->sock_fd, (struct sockaddr*) & cli_addr, &clilen);
			if (ret > 0) {
				p_new = std::shared_ptr<CSocket>(new CSocket(ret, &cli_addr));
			}
			else {
				//perror("accept");
			}

			return p_new;
		}

		int CSocket<Server>::close_connection(void) {
			int ret;
			if (this->Connected == false)
				return 0;
			ret = close(this->sock_fd);
			this->Connected = false;
			return ret;
		}

		CSocket<Server>::~CSocket()
		{

		}
#endif//__MSVC__



		bool CSocket<Server>::open_connection(void) {
			return this->Connected;
		}
		bool CSocket<Server>::is_connected(void) {
			std::unique_lock<std::mutex>lock(this->SockMutex);
			return this->Connected;
		}

		int CSocket<Server>::get_client_ip(char* s_ip)
		{
			this->ClientIp.copy(s_ip, INET6_ADDRSTRLEN);

			return 0;
		}

		int CSocket<Server>::get_connection_port(char* s_port)
		{
			return 0;
		}

		size_t CSocket<Server>::get_error_message(std::string& emsg)
		{
			size_t ret;
			emsg = this->ErrorMessage.str();
			ret = emsg.size();
			this->ErrorMessage = std::stringstream("");
			return ret;
		}
	}


