#include <iostream>

#include "main.hpp"	
#include "CLK_Timer.hpp"
#include "IO_Entity.hpp"
#include "IO_GPIO.hpp"
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

std::chrono::steady_clock::time_point begin;

// IO::ConvCpltCallback convCpltCallback = [](number value) {

// 	std::cout << "Time difference = " << 
// 		std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin).count() << 
// 		"[µs]" << std::endl;
// };	

void* timeElapsedCallback (void* arg) {

	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count() <<
		"[ms]" << std::endl;

	return nullptr;
};	

IO::GPIO gpio0;

void convCpltCallback(number num, void* param) {
	
	static int i = 0;

	gpio0.insertNewConversion({100});

	if (i++ == 3)
		exit(0);

}
#define _LINUX_
	
int main(int argc, char* argv[]) {
	
	using namespace DEBUG;

	number num1, num2, num3;
	num1._uint32 = 150;
	num2._uint32 = 200;
	num3._uint32 = 250;

	// IO::ConvCpltCallback* temp = (*convCpltCallback)(num1, nullptr);

	
	IO::Config gpio_config = {500, (IO::ConvCpltCallback*)&convCpltCallback, &gpio0};

	gpio0.configure(&gpio_config, IO::GPIO::OUTPUT_PWM);
	gpio0.insertNewConversion(num1);
	gpio0.insertNewConversion(num2);
	gpio0.insertNewConversion(num3);



	begin = std::chrono::steady_clock::now();

	gpio0.run();

	//CLK::Timer timer0(&timeElapsedCallback, nullptr);

	//begin = std::chrono::steady_clock::now();
	//timer0.setCounter(500);
	//timer0.setAutoReload(500);
	//timer0.start();

	//std::cin.get();

	//timer0.stop();

	std::cout << "\nDone. Press ENTER to exit.\n";
	std::cin.get();
	return 0;
}