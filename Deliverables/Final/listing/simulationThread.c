void simulationThread(OS::Thread* thread, void* arg) {

float psi=0,x=0,y=0, aux_x=0, aux_y=0, aux_psi=0;
	while(1) {

	thread->keepCurrentTimeStamp();

	psi_mutex.lock();
	psi=psidot*0.05+aux_psi;
	aux_psi=psi;
	nVx = V_m* cos(psi) - L/2 * V_ref/L * teta * sin(psi);
    nVy = V_m* sin(psi) - L/2 * V_ref/L * teta * cos(psi);
	psi_mutex.unlock();
	x=nVx*0.05 +aux_x;
	y=nVy*0.05+aux_y;
	aux_x=x;
	aux_y=y;
		
	psi_mutex.lock();
    psidot=V_m/L * teta;
	psi_mutex.unlock();

	thread->sleepUntilElapsed(10);
	}
}
