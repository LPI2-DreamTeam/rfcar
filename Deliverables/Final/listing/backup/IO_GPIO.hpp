#ifndef _IO_GPIO_H
#define _IO_GPIO_H

#include "IO.hpp"
#include "MEM_CircularList.hpp"
#include "CLK_Timer.hpp"

#define IO_GPIO_MAX_OBJECT_COUNT	32
#define IO_MAX_BUFFER_SIZE			8

namespace IO {

	class GPIO {

	public:

		typedef enum Mode_T {
			INPUT_ANALOG, INPUT_PULSE_COUNTING, OUTPUT_PWM
		} Mode;

		typedef struct States_T {
			uint32_t assigned;
			uint32_t configured;
			uint32_t running;
		} States;
	
	private:		// Public static methods

		static uint32_t grabAvailableObject();
		static void releaseObject(GPIO* obj);
		static void markConfigured(GPIO* obj, bool value = true);
		static void markRunning(GPIO* obj, bool value = true);
		static void markAssigned(GPIO* obj, bool value = true);
		static bool isConfigured(GPIO* obj);
		static bool isRunning(GPIO* obj);
		static bool isAssigned(GPIO* obj);
		static void* timeElapsedCallback(void* arg);

	private:	// Private static members

		static IO::GPIO::States global_states;

	public: 
		
		GPIO();
			
		/**
		 * @param config Configuraton structure
		 */
		IO::Error configure(IO::Config* config, IO::GPIO::Mode mode);
			
		void run();
			
		/**
		 * @param value Value to be  inserted in the buffer
		 */
		IO::Error insertNewConversion(number value);
			
		IO::Error fetchLastConversions(number& value);
			
		bool isConfigured();
			
		bool isAssigned();
			
		bool isRunning();

		
	private: 

		IO::Error last_error;

		MEM::CircularList<number, IO_MAX_BUFFER_SIZE> conversion_buffer;
		CLK::Timer timer;
		
		IO::GPIO::Mode mode;
		uint32_t update_period_ms;
		IO::ConvCpltCallback* conv_cplt_callback;
		void* callback_arg;
		uint32_t id;

#ifdef _LINUX_
		uint32_t last_line_id;
		std::string filename;
#endif
	};

}

#endif //_IO_GPIO_H