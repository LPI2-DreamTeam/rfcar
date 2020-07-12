#include "APP_Control.hpp"


namespace APP {

	void Control::Control() {

	}

	void Control::Operation1() {

	}

	void Control::mainRoutine() {

	}

	void Control::configRoutine() {

	}

	/**
	 * @param position
	 * @param config
	 * @return bool
	 */
	bool Control::configIRSensor(IO.IRSensor.Position position, IRSensorConfig* config) {
		return false;
	}

	/**
	 * @param position
	 * @param config
	 * @return bool
	 */
	bool Control::configMotor(IO.Motor.Position position, MotorConfig* config) {
		return false;
	}

	/**
	 * @return bool
	 */
	bool Control::configCOMRemoteVision() {
		return false;
	}

	/**
	 * @return bool
	 */
	bool Control::configCOMSmartphone() {
		return false;
	}
}