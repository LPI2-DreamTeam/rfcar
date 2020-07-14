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
	

int main(int argc, char* argv[]) {
	
	using namespace DEBUG;

	// IO::GPIO gpio0;
	// IO::Config gpio_config = {.update_period=500, .conv_cplt_callback=&convCpltCallback};

	// gpio0.configure(&gpio_config, IO::GPIO::OUTPUT_PWM);
	// gpio0.insertNewConversion((number){156});

	// begin = std::chrono::steady_clock::now();

	// gpio0.run();

	CLK::Timer timer0(&timeElapsedCallback, nullptr);

	begin = std::chrono::steady_clock::now();
	timer0.setCounter(500);
	timer0.setAutoReload(500);
	timer0.start();

	std::cin.get();

	timer0.stop();

	std::cout << "\nDone. Press ENTER to exit.\n";
	std::cin.get();
	return 0;
}