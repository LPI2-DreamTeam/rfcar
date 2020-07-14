#include <iostream>

#include "main.hpp"	
#include "APP_Control.hpp"
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


void pcCallback(number value, void* p) {

	IO::Entity<IO::MOTOR>* p_motor = reinterpret_cast<IO::Entity<IO::MOTOR>*>(p);
	float angle;
	uint32_t pulses = value._uint32;

	// calculo do angulo

	p_motor->setShaftAngle(angle);
}

void pwmCallback(number value, void* p) {

}


void controlThread(OS::Thread* thread, void* arg) {

//////////////////////////////// Motor implementation example ///////////////////////////////////

	// Configure pwm to have an update rate of 2ms
	IO::Config config_pwm = {.update_period = 2, (IO::ConvCpltCallback*)&pwmCallback, nullptr};
	// Configure pulse counter to have a sample rate of 10ms
	IO::Config config_pc = {.update_period = 10, (IO::ConvCpltCallback*)&pcCallback, nullptr};

	IO::Entity<IO::MOTOR> motor_left(&config_pwm, &config_pc, IO::Entity<IO::MOTOR>::LEFT);
	IO::Entity<IO::MOTOR> motor_right(&config_pwm, &config_pc, IO::Entity<IO::MOTOR>::RIGHT);
	
/////////////////////////////////////////////////////////////////////////////////////////////////

	// motor_left.outputPulseWidth();
	motor_left.inputShaftAngle();

	// config ir sensors
	// config motors
	// config comms

	while(1) {

	}
}

void simulationThread(OS::Thread* thread, void* arg) {

	while(1) {

	}
}

int main(int argc, char* argv[]) {
	
	using namespace DEBUG;

	OS::Thread controlThread("Control Thread", 
		controlThread, nullptr, OS::Thread::DONT_CARE, OS::Thread::HIGH);
	OS::Thread simulationThread("Simulation Thread", 
		simulationThread, nullptr, OS::Thread::DONT_CARE, OS::Thread::HIGH);


	std::cin.get();


	std::cout << "\nDone. Press ENTER to exit.\n";
	std::cin.get();
	return 0;
}