#include "IO_GPIO.hpp"

#ifdef _LINUX_

//#include <termios.h> // Contains POSIX terminal control definitions
#include <fstream>
#include <string>

#define IO_FOLDER	"io\\"

namespace IO {

	GPIO::States GPIO::global_states = {0, 0, 0};

	void* GPIO::timeElapsedCallback(void* ptr) {

		IO::GPIO* gpio_ptr = (IO::GPIO*)ptr;
		number conversion;

		// If mode is input
		if (gpio_ptr->mode < Mode::OUTPUT_PWM) {

			uint32_t line_id;
			std::ifstream conversions_file(gpio_ptr->filename, std::ios::binary | std::ios::in);

			do {
				// Move back in the file the right amount of characters to read the conversion ID and the conversion value 
				conversions_file.seekg(-(int32_t)(sizeof(line_id) + sizeof(conversion)), std::ios_base::end);

				conversions_file.read(reinterpret_cast<char*>(&line_id), sizeof(last_line_id));
				conversions_file.read(reinterpret_cast<char*>(&(conversion._uint8_arr)), sizeof(conversion));
			} while (line_id == gpio_ptr->last_line_id);

			conversions_file.close();

			gpio_ptr->last_line_id = line_id;
			gpio_ptr->insertNewConversion(conversion);
			// Else if mode is output
		} else {

			gpio_ptr->fetchLastConversions(conversion);

			std::ofstream conversions_file(gpio_ptr->filename, std::ios::binary | std::ios::out | std::ios::app);
			
			if (!conversions_file.is_open()) {
				gpio_ptr->last_error = IO::Error::FAIL_OUTPUT;
				return nullptr;
			}
			
			conversions_file.write(reinterpret_cast<char*>(&(gpio_ptr->last_line_id)), sizeof(gpio_ptr->last_line_id));
			conversions_file.write(reinterpret_cast<char*>(&(conversion)), sizeof(conversion));

			gpio_ptr->last_line_id += 1;
			conversions_file.close();
		}

		if (gpio_ptr->conv_cplt_callback != NULL)
			(*(gpio_ptr->conv_cplt_callback))(conversion, gpio_ptr->callback_arg);

		gpio_ptr->last_error = IO::OK;

		return nullptr;
	}

	GPIO::GPIO() : timer(&timeElapsedCallback, this) {

		this->id = GPIO::grabAvailableObject();

		if (id == IO_GPIO_MAX_OBJECT_COUNT) {
			this->last_error = OBJECT_UNAVAILABLE;

		} else {
			this->filename = std::string(IO_FOLDER) + std::string("gpio") + std::to_string(id) + ".bin";
			std::ofstream file(this->filename, std::ios::trunc);
			file.close();
			this->last_error = OK;
		}

	}


	Error GPIO::configure(Config* config, Mode mode) {
		
		this->mode = mode;

		if (config == nullptr) {
			last_error = NULL_CONFIG;
		
		} else {

			update_period_ms = config->update_period;
			conv_cplt_callback = config->conv_cplt_callback;

			markConfigured(this);
			last_error = OK;
		}

		return last_error;
	}


	void GPIO::run() {
		
		markRunning(this);
		timer.setCounter(update_period_ms);
		timer.setAutoReload(update_period_ms);
		timer.start();

		last_error = OK;
	}


	Error GPIO::insertNewConversion(number value) {
		
		if (mode < Mode::OUTPUT_PWM)
			this->last_error = INVALID_OPERATION;
			
		else if (this->conversion_buffer.push(value) != MEM::OK)
			last_error = BUFFER_FULL;

		else
			last_error = OK;
		
		return this->last_error;
	}


	Error GPIO::fetchLastConversions(number& value) {
	
		while (conversion_buffer.pop(&value) != MEM::EMPTY);
		last_error = OK;
		return this->last_error;
	}


	bool GPIO::isConfigured() {
		return isConfigured(this);
	}


	bool GPIO::isAssigned() {
		return isAssigned(this);
	}


	bool GPIO::isRunning() {
		return isRunning(this);
	}

	uint32_t GPIO::grabAvailableObject() {

		uint32_t i;

		for (i = 0; i < IO_GPIO_MAX_OBJECT_COUNT; i++) {
			if (((global_states.assigned >> i) & 1) == 0) {
				global_states.assigned |= 1U << i;
				break;
			}
		}
		
		return i;
	}

	void GPIO::releaseObject(GPIO* obj) {

		global_states.assigned &= ~(1U) << obj->id;
		global_states.configured &= ~(1U) << obj->id;
		global_states.running &= ~(1U) << obj->id;
	}

	void GPIO::markConfigured(GPIO* obj, bool value) {

		if (value)
			global_states.configured |= 1U << obj->id;

		else {
			global_states.configured &= ~(1U) << obj->id;
			global_states.running &= ~(1U) << obj->id;
		}
			
	}

	void GPIO::markRunning(GPIO* obj, bool value) {

		if (value)
			global_states.running |= 1U << obj->id;

		else
			global_states.running &= ~(1U) << obj->id;
	}

	void GPIO::markAssigned(GPIO* obj, bool value) {

		if (value)
			global_states.assigned |= 1U << obj->id;

		else
			global_states.assigned &= ~(1U) << obj->id;
	}

	bool GPIO::isConfigured(GPIO* obj) {
		return (global_states.configured>>obj->id) & 1U;
	}
	
	bool GPIO::isRunning(GPIO* obj) {
		return (global_states.running>>obj->id) & 1U;
	}

	bool GPIO::isAssigned(GPIO* obj) {
		return (global_states.assigned>>obj->id) & 1U;
	}

}

#endif