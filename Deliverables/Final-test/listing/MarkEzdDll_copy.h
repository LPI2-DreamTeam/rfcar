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
//DESCRIPTION: you must first call lmc1­_Initial before other function in program. 
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
//	int      nPenNo,     // Pen’s NO. (0-255)
//	int&     nMarkLoop,     //mark times
//	double&   dMarkSpeed,     //speed of marking mm/s
//	double&   dPowerRatio,        // power ratio of laser (0-100%)    
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
typedef int (*LMC1_GETPENPARAM)(int      nPenNo,//ÒªÉèÖÃµÄ±ÊºÅ(0-255)					 
					 int&      nMarkLoop,//¼Ó¹¤´ÎÊý
					 double&   dMarkSpeed,//±ê¿Ì´ÎÊýmm/s
					 double&   dPowerRatio,//¹¦ÂÊ°Ù·Ö±È(0-100%)	
					 double&   dCurrent,//µçÁ÷A
					 int&      nFreq,//ÆµÂÊHZ
					 //int&      nQPulseWidth,//QÂö³å¿í¶Èus	
					 double&      dQPulseWidth,//QÂö³å¿í¶Èus	
					 int&      nStartTC,//¿ªÊ¼ÑÓÊ±us
					 int&      nLaserOffTC,//¼¤¹â¹Ø±ÕÑÓÊ±us 
					 int&      nEndTC,//½áÊøÑÓÊ±us
					 int&      nPolyTC,//¹Õ½ÇÑÓÊ±us   //	
					 double&   dJumpSpeed, //Ìø×ªËÙ¶Èmm/s
					 int&      nJumpPosTC, //Ìø×ªÎ»ÖÃÑÓÊ±us
					 int&      nJumpDistTC,//Ìø×ª¾àÀëÑÓÊ±us	
					 double&   dEndComp,//Ä©µã²¹³¥mm
					 double&   dAccDist,//¼ÓËÙ¾àÀëmm	
					 double&   dPointTime,//´òµãÑÓÊ± ms						 
					 BOOL&     bPulsePointMode,//Âö³åµãÄ£Ê½ 
					 int&      nPulseNum,//Âö³åµãÊýÄ¿
					 double&   dFlySpeed);


typedef int (*LMC1_GETPENPARAM2)(int       nPenNo,//ÒªÉèÖÃµÄ±ÊºÅ(0-255)						 
					  int&      nMarkLoop,//¼Ó¹¤´ÎÊý
					  double&   dMarkSpeed,//±ê¿Ì´ÎÊýmm/s
					  double&   dPowerRatio,//¹¦ÂÊ°Ù·Ö±È(0-100%)	
					  double&   dCurrent,//µçÁ÷A
					  int&      nFreq,//ÆµÂÊHZ
					  double&      dQPulseWidth,//QÂö³å¿í¶Èus	
					  int&      nStartTC,//¿ªÊ¼ÑÓÊ±us
					  int&      nLaserOffTC,//¼¤¹â¹Ø±ÕÑÓÊ±us 
					  int&      nEndTC,//½áÊøÑÓÊ±us
					  int&      nPolyTC,//¹Õ½ÇÑÓÊ±us   //	
					  double&   dJumpSpeed, //Ìø×ªËÙ¶Èmm/s
					  int&      nJumpPosTC, //Ìø×ªÎ»ÖÃÑÓÊ±us
					  int&      nJumpDistTC,//Ìø×ª¾àÀëÑÓÊ±us	
					  double&   dPointTime,//´òµãÑÓÊ± ms		
					  int& nSpiWave,
					  BOOL&     bWobbleMode,
					  double&   bWobbleDiameter,
					  double&   bWobbleDist);

//INTENTION: Set the pan parameter
//DEFINITION: int lmc1_SetPenParam(
//	int      nPenNo,     // Pen’s NO. (0-255)
//	int&      nMarkLoop,     //mark times
//	double&   dMarkSpeed,     //speed of marking mm/s
//	double&   dPowerRatio,    // power ratio of laser (0-100%)    
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
//ÉèÖÃÖ¸¶¨±ÊºÅ¶ÔÓ¦µÄ¼Ó¹¤²ÎÊý
typedef int (*LMC1_SETPENPARAM)(int      nPenNo,//ÒªÉèÖÃµÄ±ÊºÅ(0-255)					 
					 int      nMarkLoop,//¼Ó¹¤´ÎÊý
					 double   dMarkSpeed,//±ê¿Ì´ÎÊýmm/s
					 double   dPowerRatio,//¹¦ÂÊ°Ù·Ö±È(0-100%)	
					 double   dCurrent,//µçÁ÷A
					 int      nFreq,//ÆµÂÊHZ
					 //int      nQPulseWidth,//QÂö³å¿í¶Èus	
					 double      dQPulseWidth,//QÂö³å¿í¶Èus
					 int      nStartTC,//¿ªÊ¼ÑÓÊ±us
					 int      nLaserOffTC,//¼¤¹â¹Ø±ÕÑÓÊ±us 
					 int      nEndTC,//½áÊøÑÓÊ±us
					 int      nPolyTC,//¹Õ½ÇÑÓÊ±us   //	
					 double   dJumpSpeed, //Ìø×ªËÙ¶Èmm/s
					 int      nJumpPosTC, //Ìø×ªÎ»ÖÃÑÓÊ±us
					 int      nJumpDistTC,//Ìø×ª¾àÀëÑÓÊ±us	
					 double   dEndComp,//Ä©µã²¹³¥mm
					 double   dAccDist,//¼ÓËÙ¾àÀëmm	
					 double   dPointTime,//´òµãÑÓÊ± ms						 
					 BOOL     bPulsePointMode,//Âö³åµãÄ£Ê½ 
					 int      nPulseNum,
					 double   dFlySpeed);//Âö³åµãÊýÄ¿


//ÉèÖÃÖ¸¶¨±ÊºÅ¶ÔÓ¦µÄ¼Ó¹¤²ÎÊý
typedef int (*LMC1_SETPENPARAM2)(int      nPenNo,//ÒªÉèÖÃµÄ±ÊºÅ(0-255)					 
					  int      nMarkLoop,//¼Ó¹¤´ÎÊý
					  double   dMarkSpeed,//±ê¿Ì´ÎÊýmm/s
					  double   dPowerRatio,//¹¦ÂÊ°Ù·Ö±È(0-100%)	
					  double   dCurrent,//µçÁ÷A
					  int      nFreq,//ÆµÂÊHZ
					  double      dQPulseWidth,//QÂö³å¿í¶Èus	
					  int      nStartTC,//¿ªÊ¼ÑÓÊ±us
					  int      nLaserOffTC,//¼¤¹â¹Ø±ÕÑÓÊ±us 
					  int      nEndTC,//½áÊøÑÓÊ±us
					  int      nPolyTC,//¹Õ½ÇÑÓÊ±us   //	
					  double   dJumpSpeed, //Ìø×ªËÙ¶Èmm/s
					  int      nJumpPosTC, //Ìø×ªÎ»ÖÃÑÓÊ±us
					  int      nJumpDistTC,//Ìø×ª¾àÀëÑÓÊ±us	
					  int nSpiWave,
					  BOOL     bWobbleMode,
					  double   bWobbleDiameter,
					  double   bWobbleDist);


//INTENTION: clear all object in database
//DEFINITION: int lmc1_ClearEntLib();            
//DESCRIPTION: call lmc1_ClearEntLib to clear all objects in database.
//RETURN VALUE: common error code
//Çå³ý¶ÔÏó¿âÀïËùÓÐÊý¾Ý
typedef int (*LMC1_CLEARENTLIB)();  


//INTENTION: add new text object into database.
//DEFINITION: int lmc1_AddTextToLib(
//	TCHAR* pStr, 
//	TCHAR* pEntName, 
//	double dPosX, 
//	double dPosY, 
//	double dPosZ, 
//	int    nAlign
//	double dTextRotateAngle,
//	int nPenNo, 
//	BOOL bHatchText        //hatch the text object or not.
//	);
//	pStr:      the character string
//	pEntName: the name of character string object
//	dPosX    left-bottom point’s X coordinate of the character string object 
//	dPosY    left-bottom point’s Y coordinate of the character string object 
//	dPosZ    Z coordinate of the character string object
//	nAlign    align way 0－8
//			//meaning of the align way:
//			//   6 ---  5 ---  4
//			//   |             |
//			//   |             |
//			//   7      8      3
//			//   |             |
//			//   |             |
//			//   0 ---- 1 ---- 2
//	dTextRotateAngle  rotation angle (in radian) that the character string object rotates around base point.
//	nPenNo          the number of pen to mark text  
//	bHatchText       hatch the text object or not
//DESCRIPTION: call lmc1_AddTextToLib to add  new text object into database.
//RETURN VALUE: common error code
//¼ÓÈëÐÂÎÄ±¾µ½Êý¾Ý¿âÖÐ
typedef int (*LMC1_ADDTEXTTOLIB)(TCHAR* pStr,//ÒªÌí¼ÓµÄ×Ö·û´®
								  TCHAR* pEntName,//×Ö·û´®¶ÔÏóÃû³Æ
					  double dPosX,//×Ö·û´®µÄ×óÏÂ½Ç»ùµãµÄx×ø±ê
					  double dPosY,//×Ö·û´®µÄ×óÏÂ½Ç»ùµãµÄy×ø±ê
					  double dPosZ,//×Ö·û´®¶ÔÏóµÄz×ø±ê
					  int    nAlign,//¶ÔÆë·½Ê½0£­8
					  double dTextRotateAngle,//×Ö·û´®ÈÆ»ùµãÐý×ªµÄ½Ç¶ÈÖµ(»¡¶ÈÖµ)
					  int nPenNo,//¶ÔÏóÊ¹ÓÃµÄ¼Ó¹¤²ÎÊý
					  BOOL bHatchText);//ÊÇ·ñÌî³äÎÄ±¾¶ÔÏó


//INTENTION: add the appointed file into database.
//DEFINITION：int lmc1_AddFileToLib(
//	TCHAR* pFileName,    //filename
//	TCHAR* pEntName,        // name of the file object
//	double dPosX,     // X coordinate of left-bottom point 
//	double dPosY,     // Y coordinate of left-bottom point
//	double dPosZ,    // Z coordinate of the file object
//	int    nAlign,    // align way 0－8
//	double dRatio,    // scaling ratio                  
//	int nPenNo,    //the pen number of the file object
//	BOOL bHatchFile     // hatch the file object or not
//	);        
//	 
//DESCRIPTION: call lmc1_AddFileLib to add new file object into database. The following file formats are supported : 
// - ezd, dxf， dst, plt, ai, bmp, jpg, tga, png, gif, tiff
//RETURN VALUE: common error code
typedef int  (*LMC1_ADDFILETOLIB)(TCHAR* pFileName,	//ÎÄ¼þÃû³Æ
								   TCHAR* pEntName,//×Ö·û´®¶ÔÏóÃû³Æ
					  double dPosX, //ÎÄ¼þ×óÏÂ½Ç»ùµãx×ø±ê
					  double dPosY, //ÎÄ¼þ×óÏÂ½Ç»ùµãy×ø±ê
					  double dPosZ, //ÎÄ¼þz×ø±ê
					  int    nAlign,//¶ÔÆë·½Ê½0£­8
					  double dRatio,//ÎÄ¼þËõ·Å±ÈÀý				  
					  int nPenNo,//¶ÔÏóÊ¹ÓÃµÄ¼Ó¹¤²ÎÊý
					  BOOL bHatchFile);//ÊÇ·ñÌî³äÎÄ¼þ¶ÔÏó Èç¹ûÊÇezdÎÄ¼þ»òÎ»Í¼ÎÄ¼þ´Ë²ÎÊýÎÞÐ§


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
typedef int (*LMC1_ADDCURVETOLIB)(double ptBuf[][2],//ÇúÏß¶¥µãÊý×é
					   int  ptNum,//ÇúÏß¶¥µãÊý
					   TCHAR* pEntName,//ÇúÏß¶ÔÏóÃû³Æ
					   int nPenNo,//ÇúÏß¶ÔÏóÊ¹ÓÃµÄ±ÊºÅ
					   int bHatch);//ÇúÏßÊÇ·ñÌî³ä


//INTENTION: add a new bar code object into database.
//DEFINITION: int lmc1_AddBarCodeToLib(
//	TCHAR* pStr, 
//	TCHAR* pEntName,
//	double dPosX, 
//	double dPosY, 
//	double dPosZ, 
//	int    nAlign,
//	int    nPenNo,
//	int    bHatchText,
//	int    nBarcodeType,
//	WORD   wBarCodeAttrib, 
//	double dHeight,
//	double dNarrowWidth,
//	double dBarWidthScale[4],
//	double dSpaceWidthScale[4],
//	double dMidCharSpaceScale,
//	double dQuietLeftScale,
//	double dQuietMidScale,
//	double dQuietRightScale,
//	double dQuietTopScale,
//	double dQuietBottomScale,
//	int    nRow, 
//	int    nCol, 
//	int    nCheckLevel,
//	int    nSizeMode,
//	double dTextHeight, 
//	double dTextWidth, 
//	double dTextOffsetX, 
//	double dTextOffsetY, 
//	double dTextSpace, 
//	TCHAR* pTextFontName
//	); 
// 
//	pStr        character string of the bar code 
//	pEntName  name of the bar code object
//	dPosX,  X coordinate of left-bottom basic point of the barcode
//	dPosY  Y coordinate of left-bottom basic point of the barcode
//	dPosZ  Z coordinate of the bar code object
//	nAlign, align way 0－8
//	nPenNo   the pen NO. of the barcode object
//	bHatchText  hatch the barcode object or not
//	nBarcodeType  type of barcode, see following:
//		#define BARCODETYPE_39      0
//		#define BARCODETYPE_93      1
//		#define BARCODETYPE_128A    2
//		#define BARCODETYPE_128B    3
//		#define BARCODETYPE_128C    4
//		#define BARCODETYPE_128OPT  5
//		#define BARCODETYPE_EAN128A 6
//		#define BARCODETYPE_EAN128B 7
//		#define BARCODETYPE_EAN128C 8
//		#define BARCODETYPE_EAN13   9
//		#define BARCODETYPE_EAN8    10
//		#define BARCODETYPE_UPCA    11
//		#define BARCODETYPE_UPCE    12
//		#define BARCODETYPE_25      13
//		#define BARCODETYPE_INTER25 14
//		#define BARCODETYPE_CODABAR 15
//		#define BARCODETYPE_PDF417  16
//		#define BARCODETYPE_DATAMTX 17
//	wBarCodeAttrib attribute of barcode.
//		#define BARCODEATTRIB_REVERSE        0x0008    // reverse barcode 
//		#define BARCODEATTRIB_HUMANREAD    0x1000 // show the character
//		#define BARCODEATTRIB_CHECKNUM    0x0004 // need check number 
//		#define BARCODEATTRIB_PDF417_SHORTMODE        0x0040 //short mode of PDF417
//		#define BARCODEATTRIB_DATAMTX_DOTMODE    0x0080 //point mode of DataMtrix
// 
//	dHeight        height of bar code     
//	dNarrowWidth: width of the narrowest module
//	dBarWidthScale: ratio of bar width to narrowest module
//	dSpaceWidthScale: ratio of space width to the narrowest module
//	dMidCharSpaceScale ratio of character space width to the narrowest module
//	dQuietLeftScale: ratio of the left blank width to the narrowest module
//	dQuietMidScale: ration of the middle blank width to the narrowest module
//	dQuietRightScale: ratio of the right blank width to the narrowest module
//	dQuietTopScale: ratio of the top blank width to the narrowest module
//	dQuietBottomScale: ratio of the bottom blank width to the narrowest module
//	nRow:  row number of two-dimension barcode
//	nCol:  column number of two-dimension barcode
//	nCheckLevel, //pdf417 error recovery level 0-8
//	nSizeMode, //DataMatrix size mode 0-30
//		#define DATAMTX_SIZEMODE_SMALLEST  0
//		#define DATAMTX_SIZEMODE_10X10     1
//		#define DATAMTX_SIZEMODE_12X12     2
//		#define DATAMTX_SIZEMODE_14X14     3
//		#define DATAMTX_SIZEMODE_16X16     4
//		#define DATAMTX_SIZEMODE_18X18     5
//		#define DATAMTX_SIZEMODE_20X20     6
//		#define DATAMTX_SIZEMODE_22X22     7
//		#define DATAMTX_SIZEMODE_24X24     8
//		#define DATAMTX_SIZEMODE_26X26     9
//		#define DATAMTX_SIZEMODE_32X32     10
//		#define DATAMTX_SIZEMODE_36X36     11
//		#define DATAMTX_SIZEMODE_40X40     12
//		#define DATAMTX_SIZEMODE_44X44     13
//		#define DATAMTX_SIZEMODE_48X48     14
//		#define DATAMTX_SIZEMODE_52X52     15
//		#define DATAMTX_SIZEMODE_64X64     16
//		#define DATAMTX_SIZEMODE_72X72     17
//		#define DATAMTX_SIZEMODE_80X80     18
//		#define DATAMTX_SIZEMODE_88X88     19
//		#define DATAMTX_SIZEMODE_96X96     20
//		#define DATAMTX_SIZEMODE_104X104   21
//		#define DATAMTX_SIZEMODE_120X120   22
//		#define DATAMTX_SIZEMODE_132X132   23
//		#define DATAMTX_SIZEMODE_144X144   24
//		#define DATAMTX_SIZEMODE_8X18     25
//		#define DATAMTX_SIZEMODE_8X32     26
//		#define DATAMTX_SIZEMODE_12X26     27
//		#define DATAMTX_SIZEMODE_12X36     28
//		#define DATAMTX_SIZEMODE_16X36     29
//		#define DATAMTX_SIZEMODE_16X48     30
//	dTextHeight  height of the character string 
//	dTextWidth width of the character string
//	dTextOffsetX  X offset of the character string
//	dTextOffsetY  Y offset of the character string
//	dTextSpace  space of the character string
//	pTextFontName  font name of the character string
//DESCRIPTION: call lmc1_AddBarCodeToLib to add bar code object into database.
//RETURN VALUE: common error code
//¼ÓÈëÌõÂëµ½Êý¾Ý¿âÖÐ
typedef int  (*LMC1_ADDBARCODETOLIB)(TCHAR* pStr,	//×Ö·û´®
									  TCHAR* pEntName,//×Ö·û´®¶ÔÏóÃû³Æ
						 double dPosX, //×Ö·û×óÏÂ½Ç»ùµãx×ø±ê
						 double dPosY, //×Ö·û×óÏÂ½Ç»ùµãy×ø±ê
						 double dPosZ, //×Ö·ûz×ø±ê
						 int    nAlign,//¶ÔÆë·½Ê½0£­8
						 int    nPenNo,
						 int    bHatchText,
						 int    nBarcodeType,//ÌõÂëÀàÐÍ
						 WORD   wBarCodeAttrib,//ÌõÂëÊôÐÔ
						 double dHeight,//Õû¸öÌõÂëµÄ¸ß	
						 double dNarrowWidth,//×îÕ­Ä£¿é¿í	
						 double dBarWidthScale[4],//Ìõ¿í±ÈÀý	(Óë×îÕ­Ä£¿é¿íÏà±È)
						 double dSpaceWidthScale[4],//¿Õ¿í±ÈÀý(Óë×îÕ­Ä£¿é¿íÏà±È)
						 double dMidCharSpaceScale,//×Ö·û¼ä¸ô±ÈÀý(Óë×îÕ­Ä£¿é¿íÏà±È)
						 double dQuietLeftScale,//ÌõÂë×ó¿Õ°×¿í¶È±ÈÀý(Óë×îÕ­Ä£¿é¿íÏà±È)
						 double dQuietMidScale,//ÌõÂëÖÐ¿Õ°×¿í¶È±ÈÀý(Óë×îÕ­Ä£¿é¿íÏà±È)
						 double dQuietRightScale,//ÌõÂëÓÒ¿Õ°×¿í¶È±ÈÀý(Óë×îÕ­Ä£¿é¿íÏà±È)	
						 double dQuietTopScale,//ÌõÂëÉÏ¿Õ°×¿í¶È±ÈÀý(Óë×îÕ­Ä£¿é¿íÏà±È)
						 double dQuietBottomScale,//ÌõÂëÏÂ¿Õ°×¿í¶È±ÈÀý(Óë×îÕ­Ä£¿é¿íÏà±È)						 
						 int    nRow,//¶þÎ¬ÂëÐÐÊý
						 int    nCol,//¶þÎ¬ÂëÁÐÊý
						 int    nCheckLevel,//pdf417´íÎó¾ÀÕý¼¶±ð0-8
						 int    nSizeMode,//DataMatrix³ß´çÄ£Ê½0-30
						 double dTextHeight,//ÈËÊ¶±ð×Ö·û×ÖÌå¸ß¶È
						 double dTextWidth,//ÈËÊ¶±ð×Ö·û×ÖÌå¿í¶È
						 double dTextOffsetX,//ÈËÊ¶±ð×Ö·ûX·½ÏòÆ«ÒÆ
						 double dTextOffsetY,//ÈËÊ¶±ð×Ö·ûY·½ÏòÆ«ÒÆ
						 double dTextSpace,//ÈËÊ¶±ð×Ö·û¼ä¾à
						 double dDiameter,
						 TCHAR* pTextFontName);//ÎÄ±¾×ÖÌåÃû³Æ

//INTENTION: change the content of the text with appointed name.
//DEFINITION: int lmc1_ChangeTextByName(TCHAR* strTextName, TCHAR* strTextNew);
//	strTextName     the name of text object whose content will be changed
//	strTextNew      new content of text
//DESCRIPTION: after loading ezd file by lmc_LoadEzdFile, user can use this function to change the content of appointed text object before marking it.
typedef int (*LMC1_CHANGETEXTBYNAME)(TCHAR* strTextName, TCHAR* strTextNew);


//INTENTION: set parameter for rotation transform
//DEFINITION：int lmc1_SetRotateParam(double dCenterX, double dCenterY, double dRotateAng)；            
//	dCenterX:  X coordinate of rotate center
//	dCenterY:  Y coordinate of rotate center 
//	dRotateAng:  rotation angle (in radian)
//DESCRIPTION: call lmc1_SetRotateParam to set the parameter of rotation transform. All of the objects in database are rotated around the appointed center. 
//RETURN VALUE: common error code
typedef void (*LMC1_SETROTATEPARAM)(double dCenterX,double dCenterY,double dRotateAng);


//////////////////////////////////////////////////////////////////
//À©Õ¹Öáº¯Êý

//INTENTION: move the extended axis to appointed position.
//DEFINITION:int lmc1_AxisMoveTo(int axis,double GoalPos)；
//	axis     axis number  0 = axis 0  1 = axis 1
//	GoalPos: absolute coordinate of position
//DESCRIPTION: call lmc1_AxisMoveTo to move extended axis to the appointed absolute coordinate position. 
//             The moving speed is the biggest speed set in parameter.
//RETURN VALUE: common error code
typedef int    (*LMC2_AXISMOVETO)(int axis,double GoalPos);

//INTENTION: calibrate the origin of extended axis
//DEFINITION：int lmc1_AxisCorrectOrigin(int axis)；
//	axis     axis number  0 = axis 0  1 = axis 1
//DESCRIPTION: call lmc1_AxisCorrectOrigin to calibrate the origin of extended axis automatically
//RETURN VALUE: common error code
typedef int    (*LMC1_AXISCORRECTORIGIN)(int axis);

//INTENTION: get the coordinate of extended axis.
//DEFINITION: int lmc1_GetAxisCoor(int axis)；
//	axis:     axis number  0 = axis 0  1 = axis 1
//DESCRIPTION: call lmc1_GetAxisCoor to get the coordinate of extended axis. 
//RETURN VALUE:  coordinate of appointed extended axis
typedef double (*LMC2_GETAXISCOOR)(int axis);

//À©Õ¹ÖáÒÆ¶¯µ½Ö¸¶¨Âö³å×ø±êÎ»ÖÃ
//ÊäÈë²ÎÊý: axis     À©Õ¹Öá  0 = Öá0  1 = Öá1
//          nGoalPos  Âö³å×ø±êÎ»ÖÃ
typedef int   (*LMC1_AXISMOVETOPULSE)(int axis,int nGoalPos);

//µÃµ½À©Õ¹ÖáµÄµ±Ç°Âö³å×ø±ê
//ÊäÈë²ÎÊý: axisÀ©Õ¹Öá  0 = Öá0  1 = Öá1
typedef int (*LMC1_GETAXISCOORPULSE)(int axis);


//INTENTION: enable and reset the coordinate of extend axis
//DEFINITION: int lmc1_Reset(BOOL bEnAxis0 , BOOL bEnAxis1)；
//	bEnAxis0    enable extended axis 0 or not
//	bEnAxis1    enable extended axis 1 or not
//DESCRIPTION: Before calling any other function about extended axis, you must call lmc1_Reset first to enable the appointed axis. 
//             When the extended axis moves to the limited position, lmc1_Reset can be called to reset the coordinate.  
//RETURN VALUE: common error code
typedef double (*LMC1_RESET)(BOOL bEnAxis0 , BOOL bEnAxis1);


//×ÖÌåÀàÐÍÊôÐÔ¶¨Òå
#define FONTATB_JSF        0x0001        //JczSingle×ÖÐÍ
#define FONTATB_TTF        0x0002        //TrueType×ÖÐÍ   
#define FONTATB_DMF        0x0004        //DotMatrix×ÖÐÍ   
#define FONTATB_BCF        0x0008        //BarCode×ÖÐÍ  

//×ÖÐÍ¼ÇÂ¼
struct lmc1_FontRecord
{  	
    TCHAR   szFontName[256];     //×ÖÌåÃû×Ö	    
    DWORD   dwFontAttrib;       //×ÖÌåÊôÐÔ
};

typedef lmc1_FontRecord* (*LMC1_GETALLFONTRECORD)(int& nFontNum);

//INTENTION: save all objects in database to the appointed .ezd file.
//DEFINITION: int lmc1_SaveEntLibToFile(TCHAR* strFileName)；
//	strFileName    name of ezd file
//DESCRIPTION: save all objects in database to the appointed ezd file.
//RETURN VALUE: common error code
typedef int (*LMC1_SAVEENTLIBTOFILE)(TCHAR* strFileName);

//INTENTION: get the maximum and minimal coordinate of the appointed object.
//DEFINITION: int lmc1_GetEntSize(TCHAR* pEntName, 
//									 double& dMinx,
//									 double& dMiny,
//									 double& dMaxx,
//									 double& dMaxy ,
//							double& dZ)；
//	pEntName  name of object
//	dMinx  minimal coordinate X
//	dMiny  minimal coordinate Y
//	dMaxx  maximum coordinate X
//	dMaxy  maximum coordinate Y
//	dZ      coordinate Z 
// 
//DESCRIPTION: get the maximum and minimal coordinate of the appointed object.
//RETURN VALUE: common error code
//typedef int  (*LMC1_GETENTSIZE)(TCHAR* pEntName,//×Ö·û´®¶ÔÏóÃû³Æ
//									 double& dMinx,
//									 double& dMiny,
//									 double& dMaxx,
//									 double& dMaxy,
//                   double& dZ);

//INTENTION: move object appointed distance
//DEFINITION: int lmc1_GetEntSize(TCHAR* pEntName, 
//						double dMovex,
//								double dMovey)；
//	pEntName: name of object
//	dMovex: the X distance of object moving
//	dMovey: the Y distance of object moving
// 
//DESCRIPTION: move object appointed distance
//RETURN VALUE: common error code
typedef int  (*LMC1_MOVEENT)(TCHAR* pEntName,//×Ö·û´®¶ÔÏóÃû³Æ
								double dMovex,
								double dMovey);


//Ëõ·ÅÖ¸¶¨¶ÔÏó£¬Ëõ·ÅÖÐÐÄ×ø±ê(dCenx£¬dCeny)  dScaleX=X·½ÏòËõ·Å±ÈÀý  dScaleY=Y·½ÏòËõ·Å±ÈÀý
typedef int  (*LMC1_SCALEENT)(TCHAR* pEntName,//×Ö·û´®¶ÔÏóÃû³Æ
				   double dCenx,
				   double dCeny,
				   double dScaleX,
				   double dScaleY);
//¾µÏñÖ¸¶¨¶ÔÏó£¬¾µÏñÖÐÐÄ×ø±ê(dCenx£¬dCeny)  bMirrorX=TRUE X·½Ïò¾µÏñ  bMirrorY=TRUE Y·½Ïò¾µÏñ
typedef  int  (*LMC1_MIRRORENT)(TCHAR* pEntName,//×Ö·û´®¶ÔÏóÃû³Æ
				   double dCenx,
				   double dCeny,
				   BOOL   bMirrorX,
				   BOOL   bMirrorY);

//Ðý×ªÖ¸¶¨¶ÔÏó£¬Ðý×ªÖÐÐÄ×ø±ê(dCenx£¬dCeny)  dAngle=Ðý×ª½Ç¶È(ÄæÊ±ÕëÎªÕý£¬µ¥Î»Îª¶È)
typedef  int  (*LMC1_ROTATEENT)(TCHAR* pEntName,//×Ö·û´®¶ÔÏóÃû³Æ
				   double dCenx,
				   double dCeny,
				   double dAngle);

//INTENTION: mark the contour using indicated red light
//DEFINITION: int lmc1_RedLightMark();
//DESCRIPTION: mark the contour using indicated red light
//RETURN VALUE: common error code
typedef int  (*LMC1_REDLIGHTMARK)();//±ê¿ÌÒ»´Îºì¹âÏÔÊ¾¿ò

//INTENTION: get the total number of objects in database.
//DEFINITION: int lmc1_GetEntityCount()；
//DESCRIPTION: get the total number of objects in database.
//RETURN VALUE:  Total count of object in database
typedef  int (*LMC1_GETENTITYCOUNT)();

//INTENTION: get the name of the object that has appointed serial number
//DEFINITION: int lmc1_GetEntityName(int nEntityIndex,
//								TCHAR szEntName[256])；
//NEntityIndex: appoint the serial number， 0－(total number-1). The total objects count can be got by lmc1_GetEntityCount.
//	szEntName: name of appointed object
//DESCRIPTION: get the name of the object that has appointed serial number
//RETURN VALUE: common error code
typedef int (*LMC1_GETENTITYNAME)(int nEntityIndex,TCHAR  szEntName[256]);

//µÃµ½¹·µÄ¿Í»§IDºÅ
typedef WORD (*LMC1_GETCLIENTID)();

//µÃµ½Ö¸¶¨¶ÔÏóµÄÎÄ±¾
typedef int (*LMC1_GETTEXTBYNAME)(TCHAR* strTextName, TCHAR strText[256]);


//É¾³ýÖ¸¶¨¶ÔÏó
typedef int  (*LMC1nt_DELETEENT)(TCHAR* pEntName);

//¿½±´Ö¸¶¨¶ÔÏóÎªÐÂ¶ÔÏó
typedef int (*LMC1_COPYENT)(TCHAR* strEntName,TCHAR* strNewEntName);

//¸Ä±äÖ¸¶¨¶ÔÏóµÄÃû×ÖÎªstrNewName
typedef int (*LMC1_CHANGEENTNAME)(TCHAR* strEntName,TCHAR* strNewName);

#endif

