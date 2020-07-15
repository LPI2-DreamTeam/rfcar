void pcCallback(number value, void* p) {

	IO::Entity<IO::MOTOR>* p_motor = reinterpret_cast<IO::Entity<IO::MOTOR>*>(p);
	float last_angle = p_motor->inputShaftAngle();
	float angle;
	float omega;
	uint32_t pulses = value._uint32;

	angle=(last_angle + PULSE_ANGLE * pulses)%360;
	omega=(angle-last_angle)/PC_UPDT_PERIOD;
	V_m=omega*WHEEL_RADIUS;

	psi_mutex.lock();
	Vr_m= V_m + WHEELBASE/2 * omega;
	Vl_m= V_m - WHEELBASE/2 * omega;
	psi_mutex.unlock();
	p_motor->setShaftAngle(angle);
}