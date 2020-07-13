#ifndef _IO_GPIO_H
#define _IO_GPIO_H

#include "IO.hpp"
#include "MEM_CircularList.hpp"

namespace IO {

	class GPIO {

		typedef enum Mode_T {
			INPUT_ANALOG, INPUT_PULSE_COUNTING, OUTPUT_PWM
		} Mode;

		typedef struct States_T {
			uint32_t assigned;
			uint32_t configured;
			uint32_t running;
		} States;
	
	public:		// Public static methods

		static GPIO* grabAvailableObject();
		static GPIO* releaseObject();

	private:	// Private static members

		static States global_states;

	public: 
		
		GPIO();
			
		/**
		 * @param config Configuraton structure
		 */
		Error configure(Config* config);
			
		bool run();
			
		/**
		 * @param value Value to be  inserted in the buffer
		 */
		Error insertNewConversion(number value);
			
		Error fetchLastConversion();
			
		bool isConfigured();
			
		bool isAssigned();
			
		bool isRunning();

	private: 

		Mode mode;
		uint32_t update_period_ms;
		ConvCpltCallback* conv_cplt_callback;
		MEM::CircularList<uint32_t> conversion_buffer;
	};

}

#endif //_IO_GPIO_H