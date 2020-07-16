#include <iostream>

#include "main.hpp"	
#include "APP_Control.hpp"
#include "CLK_Timer.hpp"
#include "IO_Entity.hpp"
#include "IO_GPIO.hpp"
#include "OS_Mutex.hpp"
#include "OS_Notification.hpp"
#include "OS_SharedMemory.hpp"
#include "OS_Thread.hpp"

#define STREAM_ID_SMARTPHONE_COMMAND 100
#define STREAM_ID_RASPBERRY_COMMAND 101
#define MOTOR_MAX_ABS_VOLT 6
#define REQUEST_COMMAND "CMD"
#define STATUS "ASK"
#define STATUS_OK "OK"
#define STATUS_NOT_OK "NOT_OK"
#define ENCODER_RESOLUTION 500.0
#define PULSE_ANGLE (float) 360.0/ENCODER_RESOLUTION
#define PC_UPDT_PERIOD 10
#define WHEEL_RADIUS 0.03 //m
#define WHEELBASE 0.20

float V_ref=0;
float teta=0;
float psidot=0;
float Vr_m=0;
float Vl_m=0;
float V_m=0;

OS::Mutex psi_mutex; 

const std::string
namespace DEBUG {

	void print_error(std::string context, std::string& error) {
#ifdef _DEBUG_
		std::cout << context << '\t' << error; 
#endif
	}
}


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


void irsensorCallback(number value, void* p){

	IO::Entity<IO::IR_SENSOR>* p_sensor = reinterpret_cast<IO::Entity<IO::IR_SENSOR>*>(p);
	float distance_temp;
	value *= 3.3;

	//LINEAR INTERPOLATION OF THE SENSOR'S VOLT/DISTANCE CURVE
	if(value>=2.5 && value<=3.1)
	{
		distance_temp= 20/3 *(4-value);
		p_sensor->setDistance(distance_temp);
	}
	else if(value>=1.4 && value<2.5);
	{
		distance_temp=10/-1.1 * (value-3.6);
		p_sensor->setDistance(distance_temp);
 	}
	else if(value>=0.9 && value<1.4)
	{
		distance_temp=20*(2.4-value);
		p_sensor->setDistance(distance_temp);
	}
	else if(value>=0.75 && value<=0.9)
	{
		distance_temp=200/3 * (1.35-value);
		p_sensor->setDistance(distance_temp);
	}
	else if(value>=0.6 && value<=0.75)
	{
		distance_temp=200/3 * (1.2-value);
		p_sensor->setDistance(distance_temp);
	}
	else if(value>=0.5 && value<=0.6)
	{
		distance_temp=100 * (1.1-value);
		p_sensor->setDistance(distance_temp);
	}
	else if(value>=0.45 && value<=0.5)
	{
		distance_temp=200 * (0.8-value);
		p_sensor->setDistance(distance_temp);
	}
	else if(value<0.45)
	{
		distance_temp=0.8;
		p_sensor->setDistance(distance_temp);
	}
}

void parseCallback(uint32_t buffer_size, uint8_t* buff_ptr) {
	
		psi_mutex.lock();
		
		teta = (float)*buff_ptr;
		V_ref = (float)*(buff_ptr+sizeof(float));
		V_ref=(V_ref/100)*6;
		teta=(teta/100)*0.5;

		psi_mutex.unlock();
}


void controlThread(OS::Thread* thread, void* arg) {

//////////////////////////////// Motor implementation example ///////////////////////////////////

	// Configure pwm to have an update rate of 2ms
	IO::Config config_pwm = {.update_period = 2, nullptr, nullptr};
	// Configure pulse counter to have a sample rate of 10ms
	IO ::Config config_pc = {.update_period = PC_UPDT_PERIOD, (IO::ConvCpltCallback*)&pcCallback, nullptr};

	IO::Entity<IO::MOTOR> motor_left(&config_pwm, &config_pc, IO::Entity<IO::MOTOR>::LEFT);
	IO::Entity<IO::MOTOR> motor_right(&config_pwm, &config_pc, IO::Entity<IO::MOTOR>::RIGHT);

	
/////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////// Sensor implementation ///////////////////////////////////

	// Configure sensor to have an update rate of 10ms
	IO::Config config_sensor = {.update_period = 10, (IO::ConvCpltCallback*)&irsensorCallback, nullptr};

	IO::Entity<IO::IR_SENSOR> sensor1(&config_sensor,FRONT_LEFT);
	IO::Entity<IO::IR_SENSOR> sensor2(&config_sensor,FRONT_CENTER);
	IO::Entity<IO::IR_SENSOR> sensor3(&config_sensor,FRONT_RIGHT);
	IO::Entity<IO::IR_SENSOR> sensor4(&config_sensor,RIGHT_FRONT);
	IO::Entity<IO::IR_SENSOR> sensor5(&config_sensor,RIGHT_BACK);
	IO::Entity<IO::IR_SENSOR> sensor6(&config_sensor,BACK_RIGHT);
	IO::Entity<IO::IR_SENSOR> sensor7(&config_sensor,BACK_LEFT);
	IO::Entity<IO::IR_SENSOR> sensor8(&config_sensor,LEFT_BACK);
	IO::Entity<IO::IR_SENSOR> sensor9(&config_sensor,LEFT_FRONT);

/////////////////////////////////////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////	CONFIGURE STREAMS	/////////////////////////////////////////////////////////////////
	//configure commands and request streams rapsberry and smartphone
	COM::Manager::CreateDevice Smartphone(COM::BLUETOOTH	,COM::CLIENT,4); 
	COM::Manager::CreateDevice Raspberry(COM::BLUETOOTH,COM::SERVER,1);	

	COM::Stream::Config config_stream_sp={&Smartpone,STREAM_ID_SMARTPHONE_COMMAND,(COM::Stream::ParserCallback*) &parseCallback(),COM::BIDIRECTIONAL};
	COM::Stream::Config config_stream_rasp={&Raspberry,STREAM_ID_RASPBERRY_COMMAND,(COM::Stream::ParserCallback*) &parseCallback(),COM::BIDIRECTIONAL};

	COM::Manager::createStream stream_sp(&config_stream_sp,5); 		
	COM::Manager::createStream stream_rasp(&config_stream_rasp,5);	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	char temp_buffer[50];
	float distances[9];
	float angles[2];

	float ref_theta;
	float ref_speed;

	float out_pwm[2];

//CONTROL VARS BEGIN
float nUvr=0,nUvl=0, nVx=0,nVy=0,Vr_m=0,Vl_m=0,Vr_minus1=0, Vl_minus1=0;
float Uvl=0,Uvr=0, Vr_ref=0, Vl_ref=0, Uvl_minus1=0, Uvr_minus1=0;
int vflag=0;

float L=WHEELBASE;
float Kp=0.05;
float Ki=0.1;
//CONTROL VARS END


	thread->sleepFor(5);
	thread->keepCurrentTimestamp();

	while(1) {

		vflag=0;

		//SEND CMD REQUEST TO SMARTPHONE
		COM::Manager::queueMSG(REQUEST_COMMAND,&stream_sp);

		//REQUESTS COMMAND TO RASPBERRY PI
		COM::Manager::queueMSG(REQUEST_COMMAND,&stream_rasp);

		//SLEEP FOR 38 MS
		thread->sleepUntilElapsed(38);

		//FETCHING IR SENSORS DISTANCES
		distances[0] = sensor1.inputDistance();
		distances[1] = sensor2.inputDistance();
		distances[2] = sensor3.inputDistance();
		distances[3] = sensor4.inputDistance();
		distances[4] = sensor5.inputDistance();
		distances[5] = sensor6.inputDistance();
		distances[6] = sensor7.inputDistance();
		distances[7] = sensor8.inputDistance();
		distances[8] = sensor9.inputDistance();

		for(i=0;i<9;i++){
			if(distances[i]<0.04/*40cm*/){
				distances[i]=1;
			}
		}
// attribution
		motor_left.inputshaftAngle();
		motor_right.inputshaftAngle();

		//SLEEP FOR 2 MS
		thread->sleepUntilElapsed(40);

		// CONTROL RULE
		psi_mutex.lock();
		for(i=0;i<9;i++)
   		 {
      		if(distances[i]==1 && ((psidot<=(i+1)*(3.14*2/9)) && (psidot>=i*(3.14*2/9))))
    	  	{
     			Vr_ref=0;
				V_ref=0;
        		Vl_ref=0;
				teta=0;
				vflag=1;
        	}
 	   }
		if(!vflag)
		{
		Vr_ref = V_ref + teta*V_ref/2;
    	Vl_ref = V_ref - teta*V_ref/2;
		}
		psi_mutex.unlock();


		Uvr = Uvr_minus1 + Kp*(Vr_minus1-Vr_m) + Ki*(Vr_ref-Vr_m);	//PID_r
    	Uvl = Uvl_minus1 + Kp*(Vl_minus1-Vl_m) + Ki*(Vl_ref-Vl_m);	//PID_l
		
		Uvl_minus1=Uvl;
		Uvr_minus1=Uvr;
		
		Vr_minus1=Vr_m;
		Vl_minus1=Vl_m;

		out_pwm[IO::LEFT] 	= (Uvl/MOTOR_MAX_ABS_VOLT)*100;
		out_pwm[IO::RIGHT]	= (Uvr/MOTOR_MAX_ABS_VOLT)*100;

		thread->sleepUntilElapsed(50);
		thread->keepCurrentTimestamp();

		motor_left.outputPulseWidth(out_pwm[IO::LEFT]);
		motor_right.outputPulseWidth(out_pwm[IO::RIGHT]);

		
	}
}

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

int main(int argc, char* argv[]) {
	
	using namespace DEBUG;

	OS::Thread controlThread("Control Thread", 
		controlThread, nullptr, OS::Thread::DONT_CARE, OS::Thread::HIGH);
	OS::Thread simulationThread("Simulation Thread", 
		simulationThread, nullptr, OS::Thread::DONT_CARE, OS::Thread::HIGH);

	simulationThread.run();
	controlThread.run();

	sisimulationThread.join();
	controlThread.join();

	std::cout << "\nDone. Press ENTER to exit.\n";
	std::cin.get();
	return 0;
}