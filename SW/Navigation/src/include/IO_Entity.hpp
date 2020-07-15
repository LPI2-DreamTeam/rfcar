 #ifndef _IO_ENTITY_H
#define _IO_ENTITY_H

#include "IO_GPIO.hpp"

namespace IO {

	typedef enum Types_T {
		VIRTUAL=0, IR_SENSOR, MOTOR
	} Types;

	template <Types type = Types::VIRTUAL>
	class Entity {

	public: 

		~Entity() {

		}

		virtual Error getLastError() {
			return OK;
		}
		
	};



	template <>
	class Entity<MOTOR>: public Entity<> {

	public:
		
		typedef enum Position_T {
			LEFT=0, RIGHT
		} Position;

	public:


		Entity(IO::Config* config_pwm = nullptr, IO::Config* config_pc = nullptr,IO::Entity<MOTOR>::Position position = IO::Entity<MOTOR>::Position::RIGHT) {

			this->position = position;

			if (config_pwm == nullptr || config_pc == nullptr) {
				this->last_error = NULL_CONFIG;
			
			} else {
				
				config_pwm->callback_arg = this;
				config_pc->callback_arg = this;

				this->gpio_pwm.configure(config_pwm, GPIO::OUTPUT_PWM);
				this->gpio_pwm.configure(config_pc, GPIO::INPUT_PULSE_COUNTING);
				this->last_error = OK;
			}

		}

		IO::Error getLastError() override {
			return last_error;
		}

		float inputShaftAngle() {
			this->last_error = OK;
			return this->shaft_angle;
		}

		IO::Error setShaftAngle(float newAngle) {
			
			if (newAngle >= 0) {
				this->shaft_angle = newAngle;
				this->last_error = OK;
			
			} else {
				this->last_error = INVALID_SIGN;
			}

			return this->last_error;
		}

		void outputPulseWidth(float value) {
			
			number val;
			val._float = value;

			// Error treatment
			this->gpio_pwm.insertNewConversion(val);
		}
		
	private: 

		IO::Error last_error;

		IO::Entity<MOTOR>::Position position;
		IO::GPIO gpio_pwm;
		IO::GPIO gpio_pulse_counter;

		float shaft_angle;
		
	};


	
	template <>
	class Entity<IR_SENSOR>: public Entity<> {

	public:

		typedef enum Position_T {
			FRONT_LEFT=0, FRONT_CENTER, FRONT_RIGHT, RIGHT_FRONT, RIGHT_BACK, BACK_RIGHT, BACK_LEFT, LEFT_BACK, LEFT_FRONT
		} Position;

	public: 

		~Entity() {

		}

			
		/**
		 * @param config
		 * @param calc_distance
		 * @param position
		 */
		Entity(IO::Config* config, IO::Entity<IR_SENSOR>::Position position) {
			this->position = position;
		}

		IO::Error getLastError() override {
			last_error = OK;
			return last_error;
		}


		float inputDistance() {
			last_error = OK;
			return distance;
		}

		IO::Error setDistance () {
			
			
		}

	private:

		IO::Error last_error; 

		float distance;
		IO::GPIO gpio_adc;
		IO::Entity<IR_SENSOR>::Position position;
	};

}

#endif //_IO_ENTITY_H