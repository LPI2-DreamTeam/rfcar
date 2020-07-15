#include <iostream>

#include "main.hpp"
#include "COM_LL.hpp"
#include "MEM_CircularList.hpp"
#include "OS_Mutex.hpp"
#include "OS_Notification.hpp"
#include "OS_SharedMemory.hpp"
#include "OS_Thread.hpp"

namespace DEBUG {

	void print_error(std::string context, std::string& error) {
#ifdef _DEBUG_
		std::cout << context << '\t' << error; 
#endif
	}
}

int main(int argc, char* argv[]) {
	
	using namespace DEBUG;

	// Must be the same as chosen GUEST port (example: 4 for COM4(/dev/ttyS3))
	COM::LL<COM::Protocol::BLUETOOTH, COM::Role::SERVER>bt(4);	
	char buffer[30];
	std::string error_str;

	bt.listenConnection();
	bt.getLastError(error_str);
	print_error("BT", error_str);
	
	bt.acceptConnection();
	bt.getLastError(error_str);
	print_error("BT", error_str);

	memset(buffer, '\0', sizeof(buffer));

	loop {
		bt.readStr(buffer, 29);
		
		if (bt.getLastError(error_str) != COM::Error::OK) {
			print_error("BT", error_str);
			break;
		}

		if (strlen(buffer) != 0) {
			print_error("BT", error_str);
			error_str = "Message received: " + std::string(buffer);
			print_error("BT", error_str);
			bt.writeStr(buffer, strlen(buffer));
			bt.getLastError(error_str);
			print_error("BT", error_str);
			break;
		}
	}

	std::cout << "\nDone. Press ENTER to exit.\n";
	std::cin.get();
	return 0;
}