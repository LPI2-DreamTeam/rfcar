for(i=1;i<=9;i++)
    {
        if(sensor_buffer[i]==1 && ((psidot<=i*(3.14*2/9)) && (psidot>=(i-1)*(3.14*2/9)) && vflag==0))
        {
            Vr_ref=0;
						V_ref=0;
            Vl_ref=0;
						vflag=1;
        }
    }
		if(!vflag)
		{
		V_ref=1;
		Vr_ref = V_ref + teta*V_ref/2;
    Vl_ref = V_ref - teta*V_ref/2;
		}
		