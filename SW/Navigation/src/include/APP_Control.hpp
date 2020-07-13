#ifndef _CONTROL_H
#define _CONTROL_H

#include "IO_Entity.hpp"
#include "COM_Manager.hpp"

#define SENSOR_COUNT	9
#define MOTOR_COUNT		2

namespace APP {

	class Control {
	public: 	// Public methods
		
		Control();
		
		~Control();
			
		void mainRoutine();
			
		void configRoutine();

	private: 	// Private members

		COM::Manager com_manager;
		IO::Entity<IO::Types::IR_SENSOR> sensors[SENSOR_COUNT];
		IO::Entity<IO::Types::MOTOR> motors[MOTOR_COUNT];

	private:	// Private methods	
		/**
		 * @param position
		 * @param config
		 */
		bool configIRSensor(IO::Entity<IO::Types::IR_SENSOR>::Position position, IO::Config* config);
			
		/**
		 * @param position
		 * @param config
		 */
		bool configMotor(IO::Entity<IO::Types::MOTOR>::Position position, IO::Config* config);
			
		bool configCOMRemoteVision();
			
		bool configCOMSmartphone();
	};
}

#endif //_CONTROL_H