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