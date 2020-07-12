#ifndef _IO_ENTITY_H
#define _IO_ENTITY_H

namespace IO {

	template <Entity::Types type = VIRTUAL>
	class Entity {

		namespace Motor {
			typedef enum Position_T {
				LEFT, RIGHT
			} Position;
		}

		namespace IRSensor {

			typedef enum Position_T {
				FRONT_LEFT, FRONT_CENTER, FRONT_RIGHT, RIGHT_FRONT, RIGHT_BACK, BACK_RIGHT, BACK_LEFT, LEFT_BACK, LEFT_FRONT
			} Position;
		}

	public: 

		Entity();

		Error getLastError() virtual;
		
	};





	template <>
	class Entity<MOTOR>: public Entity<> {
	
	public: 
			
		/**
		 * @param config
		 */
		Entity(Config* config);

		float lastCalculatedAngle() {
			return angle;
		}
		
	private: 

		float angle;
		float encoder_resolution;
		GPIO gpio_pwm;
		GPIO gpio_pulse_counter;
		Error last_error;
		const Motor::Position position;
	};


	
	template <>
	class Entity<IR_SENSOR>: public Entity<> {

	public: 
			
		/**
		 * @param config
		 * @param calc_distance
		 * @param position
		 */
		void IRSensor(Config* config, ConvCpltCallback* calc_distance, Entity.IRSensor.Position position);

		float lastCalculatedDistance() {
			return distance;
		}

	private:
	
		float distance;
		GPIO gpio_adc;
		Error last_error; 
		const IRSensor::Position position;
	};

}

#endif //_IO_ENTITY_H