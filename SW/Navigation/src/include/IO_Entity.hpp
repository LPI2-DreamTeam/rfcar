#ifndef _IO_ENTITY_H
#define _IO_ENTITY_H

#include "IO_GPIO.hpp"

namespace IO {

	typedef enum Types_T {
		VIRTUAL, IR_SENSOR, MOTOR
	} Types;

	template <Types type = Types::VIRTUAL>
	class Entity {

	public: 

		Entity() {
			
		}

		~Entity() {

		}

		virtual void kill() {

		}

		virtual Error getLastError() {
			return OK;
		}
		
	};



	template <>
	class Entity<MOTOR>: public Entity<> {

	public:
		
		typedef enum Position_T {
			LEFT, RIGHT
		} Position;

	public: 

		Entity()  {

		}
			
		/**
		 * @param config
		 */
		Entity(Config* config, Entity<MOTOR>::Position position) {

		}

		~Entity() {

		}

		void kill() override {

		}

		Error getLastError() override {
			return OK;
		}

		float lastCalculatedAngle() {
			return angle;
		}
		
	private: 

		bool dead;
		Error last_error;

		float angle;
		float encoder_resolution;
		GPIO gpio_pwm;
		GPIO gpio_pulse_counter;
		Entity<MOTOR>::Position position;
	};


	
	template <>
	class Entity<IR_SENSOR>: public Entity<> {

	public:

		typedef enum Position_T {
			FRONT_LEFT, FRONT_CENTER, FRONT_RIGHT, RIGHT_FRONT, RIGHT_BACK, BACK_RIGHT, BACK_LEFT, LEFT_BACK, LEFT_FRONT
		} Position;

	public: 

		Entity() {

		}

		~Entity() {

		}

			
		/**
		 * @param config
		 * @param calc_distance
		 * @param position
		 */
		Entity(Config* config, Entity<IR_SENSOR>::Position position) : position(position) {

		}

		Error getLastError() override {
			return OK;
		}

		void kill() override {
			
		}

		float lastCalculatedDistance() {
			return distance;
		}

	private:

		bool dead;
		Error last_error; 

		float distance;
		GPIO gpio_adc;
		Entity<IR_SENSOR>::Position position;
	};

}

#endif //_IO_ENTITY_H