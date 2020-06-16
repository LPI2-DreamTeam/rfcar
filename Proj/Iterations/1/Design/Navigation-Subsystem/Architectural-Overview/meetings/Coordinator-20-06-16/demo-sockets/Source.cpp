#include <iostream>
#include <chrono>
#define __MSVC__
#include "Socket.hpp"

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"



int socket_client_test(void)
{
	using namespace networking;

	char message[] = "this is a test";;
	char ip[] = "127.0.0.1";
	std::string Ip = "127.0.0.1";

	char* sendbuf = message;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	int iResult;
	CSocket<Client>C1;

	C1.set_sock_family(AF_UNSPEC);
	C1.set_sock_port(SOCK_STREAM);
	C1.set_ip_address(Ip);
	C1.set_sock_port(27015);
	C1.open_connection();

	iResult = C1.safe_write(message, sizeof(message));
	printf("<Client>Bytes Sent: %ld\n", iResult);

	do {

		iResult = C1.safe_read(recvbuf, recvbuflen);
		if (iResult > 0)
			printf("<Client>Bytes received: %d\n", iResult);
		else if (iResult == 0)
			printf("<Client>Connection closed\n");
		else
			printf("<Client>recv failed with error: %d\n", WSAGetLastError());

	} while (iResult > 0);

	C1.close_connection();

	return 0;
}

int socket_server_test(void)
{
	using namespace networking;

	int iResult;
	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	CSocket<Server>Cs;
	std::shared_ptr<CSocket<Server>>p_Soc;

	Cs.set_sock_port(27015);
	Cs.set_sock_family(AF_INET);
	Cs.set_sock_type(SOCK_STREAM);
	Cs.listen_connection();

	p_Soc = Cs.accept_connection();

	Cs.close_connection();


	iResult = p_Soc->safe_read(recvbuf, recvbuflen);
	if (iResult > 0) {
		std::cout << "<Server>Bytes received: " << iResult << std::endl;

		std::cout << "<Server>Message was:" << recvbuf << std::endl;

		// Echo the buffer back to the sender
		iSendResult = p_Soc->safe_write(recvbuf, iResult);
		if (iSendResult == SOCKET_ERROR) {
			std::cout << "<Server>send failed with error: " << WSAGetLastError() << std::endl;
			p_Soc->close_connection();

			return 1;
		}
		std::cout << "<Server>Bytes sent: " << iSendResult << std::endl;
	}
	else if (iResult == 0)
		std::cout << "<Server>Connection closing...\n";
	else {
		std::cout << "<Server>recv failed with error:" << WSAGetLastError() << std::endl;
		p_Soc->close_connection();
		return 1;
	}

	//} while (iResult > 0);

	p_Soc->close_connection();

	return 0;
}


int main(int argc, char** argv) {

	using namespace std::chrono_literals;
	std::thread TServer(socket_server_test);
	

	std::this_thread::sleep_for(2s);
	socket_client_test();

	if (TServer.joinable())
		TServer.join();


	return 0;
}