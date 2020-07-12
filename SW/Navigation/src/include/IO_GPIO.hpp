#ifndef _IO_GPIO_H
#define _IO_GPIO_H

namespace IO {

	class GPIO {
		
	public: 
		
	GPIO();
		
	/**
	 * @param config Configuraton structure
	 */
	Error configure(GPIOConfig* config);
		
	bool run();
		
	/**
	 * @param value Value to be  inserted in the buffer
	 */
	Error insertNewConversion(number value);
		
	Error fetchLastConversion();
		
	bool isConfigured();
		
	bool isAssigned();
		
	bool isRunning();
		
	static GPIO* grabAvailableObject();
		
	static GPIO* releaseObject();

	private: 

		GPIOMode mode;
		uint32 update_period_ms;
		ConvCpltCallback* conv_cplt_callback;
		MEM.CircularList conversion_buffer;
		static IO.GPIO.States global_states;
	};

}

#endif //_IO_GPIO_H