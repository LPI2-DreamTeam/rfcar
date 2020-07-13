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

	private:	// Private static members

		static States global_states;

	public: 
		
		GPIO();
			
		/**
		 * @param config Configuraton structure
		 */
		Error configure(IO::Config* config, IO::GPIO::Mode mode);
			
		void run();
			
		/**
		 * @param value Value to be  inserted in the buffer
		 */
		Error insertNewConversion(number value);
			
		Error fetchLastConversions(number& value);
			
		bool isConfigured();
			
		bool isAssigned();
			
		bool isRunning();

		void kill();

		void timeElapsedCallback(void*);

	private: 

#ifdef _LINUX_
		uint32_t last_line_id;
		std::string filename;
#endif
		CLK::Timer timer;
		IO::Error last_error;
		uint32_t id;
		IO::GPIO::Mode mode;
		uint32_t update_period_ms;
		ConvCpltCallback* conv_cplt_callback;
		MEM::CircularList<number, IO_MAX_BUFFER_SIZE> conversion_buffer;

	};

}

#endif //_IO_GPIO_H