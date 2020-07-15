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
const string REQUEST_COMMAND ="CMD"
const string STATUS = "ASK"
const string STATUS_OK = "OK"
const string STATUS_NOT_OK = "NOT_OK"

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
	float angle;
	uint32_t pulses = value._uint32;

	// calculo do angulo

	p_motor->setShaftAngle(angle);
}

void pwmCallback(number value, void* p) {

}

void irsensorCallback(number value, void* p){

}

void parseCallback(){
	value *=3.3;
	for(int i=0;i<9;i++)
	if()
}


void controlThread(OS::Thread* thread, void* arg) {

//////////////////////////////// Motor implementation example ///////////////////////////////////

	// Configure pwm to have an update rate of 2ms
	IO::Config config_pwm = {.update_period = 2, (IO::ConvCpltCallback*)&pwmCallback, nullptr};
	// Configure pulse counter to have a sample rate of 10ms
	IO ::Config config_pc = {.update_period = 10, (IO::ConvCpltCallback*)&pcCallback, nullptr};

	IO::Entity<IO::MOTOR> motor_left(&config_pwm, &config_pc, IO::Entity<IO::MOTOR>::LEFT);
	IO::Entity<IO::MOTOR> motor_right(&config_pwm, &config_pc, IO::Entity<IO::MOTOR>::RIGHT);

	
/////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////// Sensor implementation example ///////////////////////////////////

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

	//motor_left.outputPulseWidth();
	//motor_left.inputShaftAngle();

	// config ir sensors -> done
	// config motors ->done
	// config comms
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//configure device smartphone ?
	//configure device raspberry ?
	//configure commands and request streams rapsberry and smartphone
	COM::Manager::CreateDevice Smartphone(COM::BLUETOOTH	,COM::CLIENT,4); 	//??
	COM::Manager::CreateDevice Raspberry(COM::BLUETOOTH,COM::SERVER,1);	//??

	COM::Stream::Config config_stream_sp={&Smartpone,STREAM_ID_SMARTPHONE_COMMAND,(COM::Stream::ParserCallback*) &parseCallback(),COM::BIDIRECTIONAL};
	COM::Stream::Config config_stream_rasp={&Raspberry,STREAM_ID_RASPBERRY_COMMAND,(COM::Stream::ParserCallback*) &parseCallback(),COM::BIDIRECTIONAL};

	COM::Manager::createStream stream_sp(&config_stream_sp,5); 		// LAST NUMBER UNKNOWN ASKS FOR  CLK::Time polling_rate_read TIME BETWEEN VERIFICATION
	COM::Manager::createStream stream_rasp(&config_stream_rasp,5);	// LAST NUMBER UNKNOWN ASKS FOR  CLK::Time polling_rate_read TIME BETWEEN VERIFICATION
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//queueMsg falta size, to be updated

/*
	//REQUESTS COMMAND TO SMARTPHONE
	COM::Manager::queueMSG(REQUEST_COMMAND,&stream_sp);

	uint32_t size = COM::Manager::fetchLastMsg(&stream_sp) == 0;
	
	//WAITS FOR COMMAND
	if(size == 0) {
		// 
	}

	//REQUESTS COMMAND TO RASPBERRY PI
	COM::Manager::queueMSG(REQUEST_COMMAND,&stream_rasp);

	//WAITS FOR COMMAND
	while(COM::Manager::fetchLastMsg(&stream_rasp) ==0);

	//STATUS FEEDBACK SMARTPHONE
	COM::Manager::queueMSG(STATUS_OK,&stream_sp);

	//WAITS FOR ACKNOWLEDGEMENT
	while(COM::Manager::fetchLastMsg(&stream_sp) ==0);

	//STATUS FEEDBACK RASPBERRY PI
	COM::Manager::queueMSG(STATUS_OK,&stream_rasp);

	//WAITS FOR ACKNOWLEDGEMENT
	while(COM::Manager::fetchLastMsg(&stream_rasp) ==0);
*/

	char temp_buffer[50];
	float distances[9];
	float angles[2];

	float ref_theta;
	float ref_speed;

	float out_pwm[2];
	float Vr;
	float Vl;

//CONTROL VARS BEGIN
float nUvr=0,nUvl=0,psi=0,psidot=0, nVx=0,nVy=0,Vr_m=0,Vl_m=0,Vr_minus1=0, Vl_minus1=0,x=0,y=0;
float Uvl=0,Uvr=0,norm=0, counter=0, V_m=0, Vr_ref=0, Vl_ref=0, Uvl_minus1=0, Uvr_minus1=0;
float aux_x=0,aux_y=0, aux_psi=0;
int i=0, vflag=0;

float V_ref;
float teta;

float L=0.20;
float Kp=0.05;
float Ki=0.1;
//CONTROL VARS END
	while(1) {
		vflag=0;
		thread->keepCurrentTimestamp();

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
		// calculate wheel speed
		Vr_m=0;
		Vl_m=0;

		//SLEEP FOR 2 MS
		thread->sleepUntilElapsed(40);

		
		if (COM::Manager::fetchLastMsg(&stream_sp) != 0) {
			teta 	= (float)stream_sp;
			V_ref 	= (float)(stream_sp+sizeof(float));
			V_ref=(V_ref/100)*6;
			teta=(teta/100)*0.5;
		}

		// CONTROL RULE

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

		Uvr = Uvr_minus1 + Kp*(Vr_minus1-Vr_m) + Ki*(Vr_ref-Vr_m);	//PID_r
    	Uvl = Uvl_minus1 + Kp*(Vl_minus1-Vl_m) + Ki*(Vl_ref-Vl_m);	//PID_l
		
		Uvl_minus1=Uvl;
		Uvr_minus1=Uvr;
		
		Vr_minus1=Vr_m;
		Vl_minus1=Vl_m;

		out_pwm[IO::LEFT] 	= (Uvl/MOTOR_MAX_ABS_VOLT)*100;
		out_pwm[IO::RIGHT]	= (Uvr/MOTOR_MAX_ABS_VOLT)*100;

		thread->sleepUntilElapsed(50);

		motor_left.outputPulseWidth(out_pwm[IO::LEFT]);
		motor_right.outputPulseWidth(out_pwm[IO::RIGHT]);

	}
}

void simulationThread(OS::Thread* thread, void* arg) {

	while(1) {
	psi=psidot*0.05+aux_psi;
	aux_psi=psi;

	/*V_m= Uvr/2 + Uvl/2;*/ // CHECK WITH RAM
	nVx = V_m* cos(psi) - L/2 * V_ref/L * teta * sin(psi);
    nVy = V_m* sin(psi) - L/2 * V_ref/L * teta * cos(psi);
	x=nVx*0.05 +aux_x;
	y=nVy*0.05+aux_y;
	aux_x=x;
	aux_y=y;
		
		
	//norm=sqrt(pow(nVx,2)+pow(nVy,2));
    psidot=V_m/L * teta;
    //Vr_m = norm + teta*V_m/2;
    //Vl_m = norm - teta*V_m/2;
	}
}

int main(int argc, char* argv[]) {
	
	using namespace DEBUG;

	OS::Thread controlThread("Control Thread", 
		controlThread, nullptr, OS::Thread::DONT_CARE, OS::Thread::HIGH);
	OS::Thread simulationThread("Simulation Thread", 
		simulationThread, nullptr, OS::Thread::DONT_CARE, OS::Thread::HIGH);


	std::cin.get();


	std::cout << "\nDone. Press ENTER to exit.\n";
	std::cin.get();
	return 0;
}