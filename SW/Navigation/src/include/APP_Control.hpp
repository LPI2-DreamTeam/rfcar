#ifndef _CONTROL_H
#define _CONTROL_H

namespace APP {

	class Control {
	public: 	// Public methods
		
		void Control();
			
		void Operation1();
			
		void mainRoutine();
			
		void configRoutine();

	private: 	// Private members

		COM com_manager;
		IO.IRSensor[] ir_sensors;
		IO.Motor[] motors;

	private:	// Private methods	
		/**
		 * @param position
		 * @param config
		 */
		bool configIRSensor(IO.IRSensor.Position position, IRSensorConfig* config);
			
		/**
		 * @param position
		 * @param config
		 */
		bool configMotor(IO.Motor.Position position, MotorConfig* config);
			
		bool configCOMRemoteVision();
			
		bool configCOMSmartphone();
	};
}

#endif //_CONTROL_H