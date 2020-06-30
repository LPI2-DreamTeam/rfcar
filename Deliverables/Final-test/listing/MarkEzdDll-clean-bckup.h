#ifndef MARKEZDDLL_H
#define MARKEZDDLL_H

// Return Values
#define LMC1_ERR_SUCCESS           0        // Success
#define LMC1_ERR_EZCADRUN          1        // Find EZCAD running
#define LMC1_ERR_NOFINDCFGFILE     2        // Can not find EZCAD.CFG
#define LMC1_ERR_FAILEDOPEN        3        // Open LMC1 board failed
#define LMC1_ERR_NODEVICE          4        // Can not find valid lmc1 device
#define LMC1_ERR_HARDVER           5        // Lmc1's version is error.
#define LMC1_ERR_DEVCFG            6        // Can not find configuration files
#define LMC1_ERR_STOPSIGNAL        7        // Alarm signal
#define LMC1_ERR_USERSTOP          8        // User stops
#define LMC1_ERR_UNKNOW            9        // Unknown error
#define LMC1_ERR_OUTTIME           10        // Overtime 
#define LMC1_ERR_NOINITIAL         11        // Un-initialized 
#define LMC1_ERR_READFILE          12        // Read file error
#define LMC1_ERR_OWENWNDNULL       13        // Window handle is NULL
#define LMC1_ERR_NOFINDFONT        14        // Can not find designated font  
#define LMC1_ERR_PENNO             15        // Wrong pen number 
#define LMC1_ERR_NOTTEXT           16        // Object is not text 
#define LMC1_ERR_SAVEFILE          17        // Save file failed 
#define LMC1_ERR_NOFINDENT         18        // Can not find designated object
#define LMC1_ERR_STATUE            19        // Can not run the operation in current state

//INTENTION: initialize lmc1 control board
//DEFINITION: int lmc1_Initial(TCHAR* strEzCadPath, BOOL bTestMode, HWND hOwenWnd)
//	strEzCadPath:  the full path where ezcad2.exe exists
//	bTestMode  Whether in test mode or not
//	hOwenWnd:  The window that has the focus. It is used to check the user's stop messages.
//DESCRIPTION: you must first call lmc1_Initial before other function in program. 
//RETURN VALUE: common error code
typedef int (*LMC1_INITIAL)(TCHAR* strEzCadPath, BOOL bTestMode, HWND hOwenWnd);

//INTENTION: Close lmc1 board
//DEFINITION: int lmc1_Close(); 
//DESCRIPTION: you must call lmc1_Close to close the lmc1 board when exit program.
//RETURN VALUE: common error code
typedef int (*LMC1_CLOSE)();

//INTENTION: open the appointed ezd file, and clear all the object in database.
//DEFINITION: int lmc1_LoadEzdFile(TCHAR* strFileName);
//DESCRIPTION: this function can open an ezd file that was build by user as a template. 
//             User need not set process parameters, because they will be loaded in from the template file. 
//RETURN VALUE: common error code
typedef int (*LMC1_LOADEZDFILE)(TCHAR* strFileName);

//INTENTION: mark all the data in database
//DEFINITION: int lmc1_Mark(BOOL bFlyMark);
//	bFlyMark= TRUE  // mark on fly 
//DESCRIPTION: Begin to mark by calling this function after loading ezd file using lmc1_LoadEzdFile. The function will not return back until marking complete.
//RETURN VALUE: common error code
typedef int (*LMC1_MARK)(BOOL bFlyMark);

//INTENTION: mark the appointed named object in database
//DEFINITION: int lmc1_MarkEntity(TCHAR* strEntName);
//DESCRIPTION: after loading ezd file by lmc1_LoadEzdFile, user can use this function to mark the appointed object. 
//             The function will not return back until marking complete.
//RETURN VALUE: common error code
typedef int (*LMC1_MARKENTITY)(TCHAR* strEntName);

//INTENTION: read the input port of the lmc1
//DEFINITION: int lmc1_ReadPort(WORD& data);
//	data:   the data in input port
//DESCRIPTION: call lmc1_ReadPort to read the data from input ports 
//RETURN VALUE: common error code
typedef int (*LMC1_READPORT)(WORD& data);

//INTENTION: write data to output port on the lmc1
//DEFINITION: int lmc1_WritePort(WORD data);
//	data: the data to output ports
//DESCRIPTION: call lmc1_WritePort to write data to the output port 
//RETURN VALUE: common error code
typedef int (*LMC1_WRITEPORT)(WORD data);

//INTENTION: Get the preview picture of all the objects in database.
//DEFINITION: CBitmap* lmc1_GetPrevBitmap(HWND hwnd, int nBMPWidth, int nBMPHeight);
//	Hwnd: Window Handle that the preview shows in
//	nBMPWidth: the preview picture's width in pixel 
//	nBMPHeight: the preview picture's height in pixel
//DESCRIPTION: Call lmc1_GetPrevBitmap to get the preview picture of all the objects in database. It can be used to refresh the interface.
//RETURN VALUE: return picture's pointer if successful and NULL if faile
typedef  CBitmap* (*LMC1_GETPREVBITMAP)(HWND hwnd,int nBMPWIDTH,int nBMPHEIGHT);

//INTENTION: set parameter for device 
//DEFINITION: int lmc1_SetDevCfg( )
//DESCRIPTION: call lmc1_SetDevCfg, and then a window will be popup. User could set parameters of device in it.  
//RETURN VALUE: common error code
typedef int (*LMC1_SETDEVCFG)();

//INTENTION: Set the hatch parameter
//DEFINITION: int lmc1_SetHatchParam (
//	BOOL   bEnableContour,    //enable the contour of object to be marked
//	int    bEnableHatch1, //enable hatch NO. 1
//	int    nPenNo1, //set the pen of hatch NO. 1
//	int    nHatchAttrib1, //set the attribute of hatch NO. 1
//	double dHatchEdgeDist1, //set the distance between hatch line and contour of hatch NO. 1
//	double dHatchLineDist1, //set the distance between two line of hatch NO. 1 .
//	double dHatchStartOffset1, //set the start offset of hatch NO. 1
//	double dHatchEndOffset1, //set the end offset of hatch NO. 1
//	double dHatchAngle1, //set the hatch angle of hatch NO. 1
//	int    bEnableHatch2, //enable hatch1 NO.2
//	int    nPenNo2,
//	int    nHatchAttrib2,
//	double dHatchEdgeDist2, 
//	double dHatchLineDist2.
//	double dHatchStartOffset2,
//	double dHatchEndOffset2, 
//	double dHatchAngle2 
//	);
//nHatchAttrib1: attribute of hatch which is a combination of the following values:
//	const int HATCHATTRIB_ALLCALC = 0x01; //compute all object as one
//	const int HATCHATTRIB_BIDIR   = 0x08; // reciprocating hatch
//	const int HATCHATTRIB_EDGE    = 0x02; // re-mark the edge    
//	const int HATCHATTRIB_LOOP    = 0x10; // ring-like hatch 
//
//DESCRIPTION: call lmc1_SetHatchParam to set the parameters of hatch. The parameters will be used for the latter hatched object.
//RETURN VALUE: common error code
typedef int (*LMC1_SETHATCHPARAM)(BOOL   bEnableContour,					  
    int    bEnableHatch1,
    int    nPenNo1,
    int    nHatchAttrib1,	
    double dHatchEdgeDist1,
    double dHatchLineDist1,
    double dHatchStartOffset1,
    double dHatchEndOffset1,
    double dHatchAngle1,
    int    bEnableHatch2,
    int    nPenNo2,
    int    nHatchAttrib2,
    double dHatchEdgeDist2,
    double dHatchLineDist2,
    double dHatchStartOffset2,					  
    double dHatchEndOffset2,					  
    double dHatchAngle2);

//INTENTION: get the parameter of appointed pen
//DEFINITION: int lmc1_GetPenParam(
//	int      nPenNo,     // Pens NO. (0-255)
//	int&     nMarkLoop,     //mark times
//	double&   dMarkSpeed,     //speed of marking mm/s
//	double&   dPowerRatio,        // powerratio of laser (0-100%)    
//	double&   dCurrent,    //current of laser (A)
//	int&      nFreq,    // frequency of laser HZ
//	int&      nQPulseWidth,     //width of Q pulse (us)    
//	int&      nStartTC,    // Start delay (us)
//	int&      nLaserOffTC,    //delay before laser off (us)
//	int&      nEndTC,        // marking end delay (us)
//	int&      nPolyTC    ,    //delay for corner (us) 
//	double&   dJumpSpeed,     //speed of jump without laser (mm/s)
//	int&      nJumpPosTC,     //delay about jump position (us)
//	int&      nJumpDistTC,    //delay about the jump distance (us)    
//	double&   dEndComp,    // compensate for end (mm)
//	double&   dAccDist,    // distance of speed up (mm)    
//	double&   dPointTime,    //delay for point mark (ms)
//	BOOL&   bPulsePointMode,    //pulse for point mark mode
//	int&      nPulseNum,    //the number of pulse
//	double&   dFlySpeed    //speed of production line
//	); 
//DESCRIPTION: call lmc1_GetParam to get the parameter of appointed pen in database
//RETURN VALUE: common error code
typedef int (*LMC1_GETPENPARAM)(int      nPenNo,//(0-255)					 
					 int&      nMarkLoop,//
					 double&   dMarkSpeed,//mm/s
					 double&   dPowerRatio,//(0-100%)	
					 double&   dCurrent,//A
					 int&      nFreq,//HZ
					 //int&      nQPulseWidth,//Qus	
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
					 double&   dPointTime,// ms						 
					 BOOL&     bPulsePointMode,// 
					 int&      nPulseNum,//
					 double&   dFlySpeed);

//INTENTION: Set the pen parameter
//DEFINITION: int lmc1_SetPenParam(
//	int      nPenNo,     // Pens NO. (0-255)
//	int&      nMarkLoop,     //mark times
//	double&   dMarkSpeed,     //speed of marking mm/s
//	double&   dPowerRatio,    // powerratio of laser (0-100%)    
//	double&   dCurrent,    //current of laser (A)
//	int&      nFreq,    // frequency of laser HZ
//	int&      nQPulseWidth,     //width of Q pulse (us)    
//	int&      nStartTC,    // Start delay (us)
//	int&      nLaserOffTC,        //delay before laser off (us)
//	int&      nEndTC,        // marking end delay (us)
//	int&      nPolyTC,        //delay for corner (us)
//	double&   dJumpSpeed,     //speed of jump without laser (mm/s)
//	int&      nJumpPosTC,        //delay about jump position (us)
//	int&      nJumpDistTC,    //delay about the jump distance (us)    
//	double&   dEndComp,        //compensate for end (mm)
//	double&   dAccDist,    // distance of speed up (mm)    
//	double&   dPointTime,    //delay for point mark (ms) 
//	BOOL&   bPulsePointMode,    //pulse for point mark mode
//	int&      nPulseNum,    //the number of pulse
//	double&   dFlySpeed    //speed of production line
//	);
//DESCRIPTION: call lmc1_SetPenParam to set the parameters of appointed pen in database 
//RETURN VALUE: common error code
typedef int (*LMC1_SETPENPARAM)(int      nPenNo,//(0-255)					 
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
//DEFINITION: int lmc1_ClearEntLib();            
//DESCRIPTION: call lmc1_ClearEntLib to clear all objects in database.
//RETURN VALUE: common error code
//
typedef int (*LMC1_CLEARENTLIB)();  

//INTENTION: add the appointed file into database.
//DEFINITIONint lmc1_AddFileToLib(
//	TCHAR* pFileName,    //filename
//	TCHAR* pEntName,        // name of the file object
//	double dPosX,     // X coordinate of left-bottom point 
//	double dPosY,     // Y coordinate of left-bottom point
//	double dPosZ,    // Z coordinate of the file object
//	int    nAlign,    // align way 08
//	double dRatio,    // scaling ratio                  
//	int nPenNo,    //the pen number of the file object
//	BOOL bHatchFile     // hatch the file object or not
//	);        
//DESCRIPTION: call lmc1_AddFileLib to add new file object into database. The following file formats are supported : 
// - ezd, dxf dst, plt, ai, bmp, jpg, tga, png, gif, tiff
//RETURN VALUE: common error code
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
//DEFINITION: int lmc1_AddCurveToLib(
//	double ptBuf[][2],    //array of the curve vertex
//	int  ptNum,    //number of the curve vertex 
//	TCHAR* pEntName,     //name of the curve object
//	int nPenNo,     //the pen number of curve object 
//	int bHatch    //hatch the curve object or not
//	);
//DESCRIPTION: call lmc1_AddCurveToLib to add curve object into database.
//RETURN VALUE: common error code
typedef int (*LMC1_ADDCURVETOLIB)(double ptBuf[][2],//
					   int  ptNum,//
					   TCHAR* pEntName,//
					   int nPenNo,//
					   int bHatch);//

//INTENTION: set parameter for rotation transform
//DEFINITIONint lmc1_SetRotateParam(double dCenterX, double dCenterY, double dRotateAng)            
//	dCenterX:  X coordinate of rotate center
//	dCenterY:  Y coordinate of rotate center 
//	dRotateAng:  rotation angle (in radian)
//DESCRIPTION: call lmc1_SetRotateParam to set the parameter of rotation transform. All of the objects in database are rotated around the appointed center. 
//RETURN VALUE: common error code
typedef void (*LMC1_SETROTATEPARAM)(double dCenterX,double dCenterY,double dRotateAng);

//INTENTION: move the extended axis to appointed position.
//DEFINITION:int lmc1_AxisMoveTo(int axis,double GoalPos)
//	axis     axis number  0 = axis 0  1 = axis 1
//	GoalPos: absolute coordinate of position
//DESCRIPTION: call lmc1_AxisMoveTo to move extended axis to the appointed absolute coordinate position. 
//             The moving speed is the biggest speed set in parameter.
//RETURN VALUE: common error code
typedef int    (*LMC2_AXISMOVETO)(int axis,double GoalPos);

//INTENTION: calibrate the origin of extended axis
//DEFINITIONint lmc1_AxisCorrectOrigin(int axis)
//	axis     axis number  0 = axis 0  1 = axis 1
//DESCRIPTION: call lmc1_AxisCorrectOrigin to calibrate the origin of extended axis automatically
//RETURN VALUE: common error code
typedef int    (*LMC1_AXISCORRECTORIGIN)(int axis);

//INTENTION: get the coordinate of extended axis.
//DEFINITION: int lmc1_GetAxisCoor(int axis)
//	axis:     axis number  0 = axis 0  1 = axis 1
//DESCRIPTION: call lmc1_GetAxisCoor to get the coordinate of extended axis. 
//RETURN VALUE:  coordinate of appointed extended axis
typedef double (*LMC2_GETAXISCOOR)(int axis);

//: axis       0 = 0  1 = 1
//          nGoalPos  
typedef int   (*LMC1_AXISMOVETOPULSE)(int axis,int nGoalPos);

//
//: axis  0 = 0  1 = 1
typedef int (*LMC1_GETAXISCOORPULSE)(int axis);

//INTENTION: enable and reset the coordinate of extend axis
//DEFINITION: int lmc1_Reset(BOOL bEnAxis0 , BOOL bEnAxis1)
//	bEnAxis0    enable extended axis 0 or not
//	bEnAxis1    enable extended axis 1 or not
//DESCRIPTION: Before calling any other function about extended axis, you must call lmc1_Reset first to enable the appointed axis. 
//             When the extended axis moves to the limited position, lmc1_Reset can be called to reset the coordinate.  
//RETURN VALUE: common error code
typedef double (*LMC1_RESET)(BOOL bEnAxis0 , BOOL bEnAxis1);

//INTENTION: save all objects in database to the appointed .ezd file.
//DEFINITION: int lmc1_SaveEntLibToFile(TCHAR* strFileName)
//	strFileName    name of ezd file
//DESCRIPTION: save all objects in database to the appointed ezd file.
//RETURN VALUE: common error code
typedef int (*LMC1_SAVEENTLIBTOFILE)(TCHAR* strFileName);

//INTENTION: move object appointed distance
//DEFINITION: int lmc1_GetEntSize(TCHAR* pEntName, 
//						double dMovex,
//								double dMovey)
//	pEntName: name of object
//	dMovex: the X distance of object moving
//	dMovey: the Y distance of object moving
//
//DESCRIPTION: move object appointed distance
//RETURN VALUE: common error code
typedef int  (*LMC1_MOVEENT)(TCHAR* pEntName,//
								double dMovex,
								double dMovey);

//(dCenxdCeny)  dScaleX=X  dScaleY=Y
typedef int  (*LMC1_SCALEENT)(TCHAR* pEntName,//
				   double dCenx,
				   double dCeny,
				   double dScaleX,
				   double dScaleY);

//(dCenxdCeny)  bMirrorX=TRUE X  bMirrorY=TRUE Y
typedef  int  (*LMC1_MIRRORENT)(TCHAR* pEntName,//
				   double dCenx,
				   double dCeny,
				   BOOL   bMirrorX,
				   BOOL   bMirrorY);

//(dCenxdCeny)  dAngle=()
typedef  int  (*LMC1_ROTATEENT)(TCHAR* pEntName,//
				   double dCenx,
				   double dCeny,
				   double dAngle);

//INTENTION: mark the contour using indicated red light
//DEFINITION: int lmc1_RedLightMark();
//DESCRIPTION: mark the contour using indicated red light
//RETURN VALUE: common error code
typedef int  (*LMC1_REDLIGHTMARK)();//

//INTENTION: get the total number of objects in database.
//DEFINITION: int lmc1_GetEntityCount()
//DESCRIPTION: get the total number of objects in database.
//RETURN VALUE:  Total count of object in database
typedef  int (*LMC1_GETENTITYCOUNT)();

//INTENTION: get the name of the object that has appointed serial number
//DEFINITION: int lmc1_GetEntityName(int nEntityIndex,
//								TCHAR szEntName[256])
//NEntityIndex: appoint the serial number 0(total number-1). The total objects count can be got by lmc1_GetEntityCount.
//	szEntName: name of appointed object
//DESCRIPTION: get the name of the object that has appointed serial number
//RETURN VALUE: common error code
typedef int (*LMC1_GETENTITYNAME)(int nEntityIndex,TCHAR  szEntName[256]);

//ID
typedef WORD (*LMC1_GETCLIENTID)();

//
typedef int  (*LMC1nt_DELETEENT)(TCHAR* pEntName);

//
typedef int (*LMC1_COPYENT)(TCHAR* strEntName,TCHAR* strNewEntName);

//strNewName
typedef int (*LMC1_CHANGEENTNAME)(TCHAR* strEntName,TCHAR* strNewName);

#endif

