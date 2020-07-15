
void parseCallback(uint32_t buffer_size, uint8_t* buff_ptr) {
	
		psi_mutex.lock();
		
		teta = (float)*buff_ptr;
		V_ref = (float)*(buff_ptr+sizeof(float));
		V_ref=(V_ref/100)*6;
		teta=(teta/100)*0.5;

		psi_mutex.unlock();
}
