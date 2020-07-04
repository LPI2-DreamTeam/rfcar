#ifndef MARKEZDDLL_H
#define MARKEZDDLL_H

// Return Values
#define LMC1_ERR_SUCCESS        0   // Success
#define LMC1_ERR_EZCADRUN       1   // Find EZCAD running
#define LMC1_ERR_NOFINDCFGFILE  2   // Can not find EZCAD.CFG
#define LMC1_ERR_FAILEDOPEN     3   // Open LMC1 board failed
#define LMC1_ERR_NODEVICE       4   // Can not find valid lmc1 device
#define LMC1_ERR_HARDVER        5   // Lmc1's version is error.
#define LMC1_ERR_DEVCFG         6   // Can not find configuration files
#define LMC1_ERR_STOPSIGNAL     7   // Alarm signal
#define LMC1_ERR_USERSTOP       8   // User stops
#define LMC1_ERR_UNKNOW         9   // Unknown error
#define LMC1_ERR_OUTTIME        10  // Overtime 
#define LMC1_ERR_NOINITIAL      11  // Un-initialized 
#define LMC1_ERR_READFILE       12  // Read file error
#define LMC1_ERR_OWENWNDNULL    13  // Window handle is NULL
#define LMC1_ERR_NOFINDFONT     14  // Can not find designated font  
#define LMC1_ERR_PENNO          15  // Wrong pen number 
#define LMC1_ERR_NOTTEXT        16  // Object is not text 
#define LMC1_ERR_SAVEFILE       17  // Save file failed 
#define LMC1_ERR_NOFINDENT      18  // Can not find designated object
#define LMC1_ERR_STATUE         19  // Can not run operation in current state

//INTENTION: initialize lmc1 control board
typedef int (*LMC1_INITIAL)(TCHAR* strEzCadPath, BOOL bTestMode, HWND hOwenWnd);

//INTENTION: Close lmc1 board
typedef int (*LMC1_CLOSE)();

//INTENTION: open the appointed ezd file, and clear all the object in database.
typedef int (*LMC1_LOADEZDFILE)(TCHAR* strFileName);

//INTENTION: mark all the data in database
typedef int (*LMC1_MARK)(BOOL bFlyMark);

//INTENTION: mark the appointed named object in database
typedef int (*LMC1_MARKENTITY)(TCHAR* strEntName);

//INTENTION: read the input port of the lmc1
typedef int (*LMC1_READPORT)(WORD& data);

//INTENTION: write data to output port on the lmc1
typedef int (*LMC1_WRITEPORT)(WORD data);

//INTENTION: Get the preview picture of all the objects in database.
typedef  CBitmap* (*LMC1_GETPREVBITMAP)(HWND hwnd,int nBMPWIDTH,int nBMPHEIGHT);

//INTENTION: get the parameter of appointed pen
typedef int (*LMC1_GETPENPARAM)(int nPenNo,//(0-255)
				int&      nMarkLoop,//
				double&   dMarkSpeed,//mm/s
				double&   dPowerRatio,//(0-100%)	
				double&   dCurrent,//A
				int&      nFreq,//HZ
				double&      dQPulseWidth,//Qus	
				int&      nStartTC,//us
				int&      nLaserOffTC,//us 
				int&      nEndTC,//us
				int&      nPolyTC,//us   //	
				double&   dJumpSpeed, //mm/s
				int&      nJumpPosTC, //us
				int&      nJumpDistTC,//us	
				double&   dEndComp,//mm
				double&   dAccDist,//mm	
				double&   dPointTime,//ms
				BOOL&     bPulsePointMode,// 
				int&      nPulseNum,//
				double&   dFlySpeed);

//INTENTION: Set the pen parameter
typedef int (*LMC1_SETPENPARAM)(int nPenNo,//(0-255)
				int      nMarkLoop,//
				double   dMarkSpeed,//mm/s
				double   dPowerRatio,//(0-100%)	
				double   dCurrent,//A
				int      nFreq,//HZ
				//int      nQPulseWidth,//Qus	
				double      dQPulseWidth,//Qus
				int      nStartTC,//us
				int      nLaserOffTC,//us 
				int      nEndTC,//us
				int      nPolyTC,//us   //	
				double   dJumpSpeed, //mm/s
				int      nJumpPosTC, //us
				int      nJumpDistTC,//us	
				double   dEndComp,//mm
				double   dAccDist,//mm	
				double   dPointTime,// ms
				BOOL     bPulsePointMode,// 
				int      nPulseNum,
				double   dFlySpeed);//

//INTENTION: clear all object in database
typedef int (*LMC1_CLEARENTLIB)();  

//INTENTION: add the appointed file into database.
typedef int  (*LMC1_ADDFILETOLIB)(TCHAR* pFileName,	//
				  TCHAR* pEntName,//
				  double dPosX, //x
				  double dPosY, //y
				  double dPosZ, //z
				  int    nAlign,//08
				  double dRatio,//
				  int nPenNo,//
				  BOOL bHatchFile);// ezd


//INTENTION: add new curve object into database.
typedef int (*LMC1_ADDCURVETOLIB)(double ptBuf[][2],//
					   int  ptNum,//
					   TCHAR* pEntName,//
					   int nPenNo,//
					   int bHatch);//

//INTENTION: enable and reset the coordinate of extend axis
typedef double (*LMC1_RESET)(BOOL bEnAxis0 , BOOL bEnAxis1);

//INTENTION: save all objects in database to the appointed .ezd file.
typedef int (*LMC1_SAVEENTLIBTOFILE)(TCHAR* strFileName);

//INTENTION: move object appointed distance
typedef int  (*LMC1_MOVEENT)(TCHAR* pEntName,
			     double dMovex,
			     double dMovey);

//INTENTION: get the total number of objects in database.
typedef  int (*LMC1_GETENTITYCOUNT)();

//INTENTION: get the name of the object that has appointed serial number
typedef int (*LMC1_GETENTITYNAME)(int nEntityIndex,TCHAR  szEntName[256]);

#endif

