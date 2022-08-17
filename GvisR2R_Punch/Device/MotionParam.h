#if !defined(AFX_MOTIONPARAM_H__INCLUDED_)
#define AFX_MOTIONPARAM_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MotionParam.h : header file
//



#ifdef USE_ZMP


#include "stdmpi.h"
#include "apputil.h"
#include "firmware.h"
#include "MFWPTR.h"



#ifdef _DEBUG
#pragma comment(lib, "mpi64vc100d.lib")
#else
#pragma comment(lib, "mpi64vc100.lib")
#endif

#pragma comment(lib, "mpiAppLib64.lib")
#pragma comment(lib, "sqNodeLib64.lib")
#pragma comment(lib, "apputil64.lib")




typedef enum stSpeedProfile { TRAPEZOIDAL = 0, S_CURVE, PARABOLIC } SpeedProfile;


typedef enum CMotionType {
	MotionTypeINVALID = MPIMotionTypeINVALID,
	MotionTypePT = MPIMotionTypePT,
	MotionTypePTF = MPIMotionTypePTF,
	MotionTypePVT = MPIMotionTypePVT,
	MotionTypePVTF = MPIMotionTypePVTF,
	MotionTypeSPLINE = MPIMotionTypeSPLINE,
	MotionTypeBESSEL = MPIMotionTypeBESSEL,
	MotionTypeBSPLINE = MPIMotionTypeBSPLINE,
	MotionTypeBSPLINE2 = MPIMotionTypeBSPLINE2,
	MotionTypeS_CURVE = MPIMotionTypeS_CURVE_JERK_PERCENT,
	MotionTypeTRAPEZOIDAL = MPIMotionTypeTRAPEZOIDAL,
	MotionTypeS_CURVE_JERK = MPIMotionTypeS_CURVE_JERK,
	MotionTypeVELOCITY = MPIMotionTypeVELOCITY_JERK_PERCENT,
	MotionTypeVELOCITY_JERK = MPIMotionTypeVELOCITY_JERK,
	MotionTypeCAM_LINEAR = MPIMotionTypeCAM_LINEAR,
	MotionTypeCAM_CUBIC = MPIMotionTypeCAM_CUBIC,
	MotionTypeFRAME = MPIMotionTypeFRAME,
	MotionTypeUSER = MPIMotionTypeUSER,
	MotionTypeEND = MPIMotionTypeEND,
	MotionTypeFIRST = MPIMotionTypeFIRST,
	MotionTypeLAST,
	MotionTypeMASK
} MotionType;


#endif






typedef struct stMotionHome
{
	BOOL	bIndex;
	BOOL    bAct;				// 0: Don't 1:Do
	int 	nDir;				//Initial Home Dir, 1:plus-dir	-1:minus-dir
	double  fEscLen;			// Escape length from sensor touch position
	double	f1stSpd;			// PreHomming Speed
	double	f2ndSpd;			// Homming Speed
	double	fAcc;				// Homming Accel
	double	fShift;				// shift
	double	fOffset;			// offset
	double	fMargin;			// Margin between Origin & index After Homming
}MotionHome;

typedef struct stMotionSpeed
{
	double	fSpd;			// Speed
	double	fAcc;			// Acceleration
	double	fDec;			// Deceleration
	double  fAccPeriod;		// each acceleration period
	double	fJogFastSpd;	// Speed
	double	fJogMidSpd;		// Speed
	double	fJogLowSpd;		// Speed
	double	fJogAcc;		// Acceleration
}MotionSpeed;

typedef struct stMotionMsg
{
	CString strTitleMsg;
	CString strMsg;
	COLORREF color;
	DWORD dwDispTime;
	BOOL bOverWrite;
}MotionMsg;

typedef struct stInfoAxisMap
{
	int		nNumMotor;
	int		nMappingMotor[5];
}InfoAxisMap;

typedef struct stInfoMsMap
{
	int			nNumAxis;
	int			nMappingAxis[5];
	InfoAxisMap stMappingMotor[5];
}InfoMsMap;

typedef struct stMotionMotion
{
	InfoMsMap	stMsMap;
	// 	int			nNumAxis;
	// 	int			nMappingAxis[5];
	MotionHome		Home;
	MotionSpeed	Speed;
	double dEStopTime;


	BOOL bBufHomming;
	CString sLmtFdAdjOffSet;	// Feeding량 Offset보정용 초과값설정. [mm]
	CString sLmtFdOvrNum;		// Feeding량 연속 초과값 초과횟수 설정. [회]
	CString sLmtFdErr;			// Feeding량 Offset보정(설비정지) 한계값 설정. [mm]
	CString sMkTq;				// Tension Servo 0 Gain Torque Value. Plus Value is Feeding direction torque[Kgf].
	CString sAoiTq;				// Tension Servo 0 Gain Torque Value. Plus Value is Feeding direction torque[Kgf].
	CString sEngraveTq;			// Tension Servo 0 Gain Torque Value. Plus Value is Feeding direction torque[Kgf].
	CString sMkFdDist, sMkFdVel, sMkFdAcc;
	CString sMkJogVel, sMkJogAcc;
	CString sMkFdTotLen, sMkTotVel, sMkPatlVel; //
	CString sAoiFdDist, sAoiFdVel, sAoiFdAcc;
	CString sAoiJogVel, sAoiJogAcc;
	CString sAoiFdTotLen, sAoiTotVel, sAoiPatlVel; //
	CString sPinPosX[2], sPinPosY[2];
	CString sStPosX[2], sStPosY[2];
	CString sMkEdPosX[2], sMkEdPosY[2];
	CString sStBufPos, sBufHomeSpd, sBufHomeAcc, sEngraveStBufPos;
	CString sMkFdLead, sAoiFdLead, sEngraveFdLead;
	CString sFdMkAoiInitDist, sFdAoiAoiDistShot, sFdEngraveAoiInitDist;//, sBufStdPosDist;
	CString sMkFdBarcodeOffset, sAoiFdBarcodeOffset, sEngraveFdBarcodeOffset;
	CString sFdBarcodeOffsetVel, sFdBarcodeOffsetAcc;
	CString sEngraveLaserAdjOffSetLimitAuto, sEngraveLaserAdjOffSetManual;	// Laser Offset보정용. [mm]
	CString sEngraveLmtFdAdjOffSet;											// Feeding량 Offset보정용 초과값설정. [mm]
	CString sEngraveLmtFdOvrNum;											// Feeding량 연속 초과값 초과횟수 설정. [회]
	CString sEngraveLmtFdErr;												// Feeding량 Offset보정(설비정지) 한계값 설정. [mm]
	BOOL bStNewPrdt, bMkTq, bAoiTq, bEngraveTq;
	CString sMkFdVacOff, sAoiFdVacOff, sEngraveFdVacOff;
	CString sSafeZone, sCollisionLength, sCollisionMargin;
	CString sAlignResultPosX[2][4], sAlignResultPosY[2][4], sAlignResultTheta[2][4], sAlignResultScore[2][4]; // [Cam][Pos]
	CString s2DEngLen, s2DAoiLen, s2DMkLen, s2DMoveVel, s2DMoveAcc, s2DOneShotRemainLen;
	CString sEngAoiLen, sEngFdDiffMax, sEngFdDiffRng, sEngFdDiffNum;
	CString sEngBuffInitPos, sEngBuffCurrPos;

	stMotionMotion()
	{
		bBufHomming = FALSE;
		sLmtFdAdjOffSet = _T("");		// Feeding량 Offset보정용 초과값설정. [mm]
		sLmtFdOvrNum = _T("");		// Feeding량 연속 초과값 초과횟수 설정. [회]
		sLmtFdErr = _T("");			// Feeding량 Offset보정(설비정지) 한계값 설정. [mm]
		sMkTq = _T("");				// Tension Servo 0 Gain Torque Value. Plus Value is Feeding direction torque[Kgf].
		sAoiTq = _T("");				// Tension Servo 0 Gain Torque Value. Plus Value is Feeding direction torque[Kgf].
		sEngraveTq = _T("");				// Tension Servo 0 Gain Torque Value. Plus Value is Feeding direction torque[Kgf].
		sMkFdDist = _T(""); sMkFdVel = _T(""); sMkFdAcc = _T("");
		sMkJogVel = _T(""); sMkJogAcc = _T("");
		sMkFdTotLen = _T(""); sMkTotVel = _T(""); sMkPatlVel = _T("");
		sAoiFdDist = _T(""); sAoiFdVel = _T(""); sAoiFdAcc = _T("");
		sAoiJogVel = _T(""); sAoiJogAcc = _T("");
		sAoiFdTotLen = _T(""); sAoiTotVel = _T(""); sAoiPatlVel = _T("");
		sPinPosX[0] = _T(""); sPinPosY[0] = _T("");
		sPinPosX[1] = _T(""); sPinPosY[1] = _T("");
		sStPosX[0] = _T(""); sStPosY[0] = _T("");
		sStPosX[1] = _T(""); sStPosY[1] = _T("");
		sMkEdPosX[0] = _T(""); sMkEdPosY[0] = _T("");
		sMkEdPosX[1] = _T(""); sMkEdPosY[1] = _T("");
		sStBufPos = _T(""); sBufHomeSpd = _T(""); sBufHomeAcc = _T(""); sEngraveStBufPos = _T("");
		sMkFdLead = _T(""); sAoiFdLead = _T(""); sEngraveFdLead = _T("");
		sFdMkAoiInitDist = _T(""); sFdAoiAoiDistShot = _T(""); sFdEngraveAoiInitDist = _T("");//sBufStdPosDist="";
		sMkFdBarcodeOffset = _T(""); sAoiFdBarcodeOffset = _T(""); sEngraveFdBarcodeOffset = _T("");
		sFdBarcodeOffsetVel = _T(""); sFdBarcodeOffsetAcc = _T("");
		sEngraveLaserAdjOffSetLimitAuto = _T(""); sEngraveLaserAdjOffSetManual = _T("");	// Laser Offset보정용. [mm]
		sEngraveLmtFdAdjOffSet = _T("");													// Feeding량 Offset보정용 초과값설정. [mm]
		sEngraveLmtFdOvrNum = _T("");														// Feeding량 연속 초과값 초과횟수 설정. [회]
		sEngraveLmtFdErr = _T("");															// Feeding량 Offset보정(설비정지) 한계값 설정. [mm]
		bStNewPrdt = FALSE; bMkTq = FALSE; bAoiTq = FALSE; bEngraveTq = FALSE;
		sMkFdVacOff = _T(""); sAoiFdVacOff = _T("");
		sSafeZone = _T("");
		sCollisionLength = _T("");
		sCollisionMargin = _T("");

		for (int k = 0; k < 2; k++)
		{
			for (int i = 0; i < 4; i++)
			{
				sAlignResultPosX[k][i] = _T("");
				sAlignResultPosY[k][i] = _T("");
				sAlignResultTheta[k][i] = _T("");
				sAlignResultScore[k][i] = _T("");
			}
		}

		s2DEngLen = _T(""); s2DAoiLen = _T(""); s2DMkLen = _T(""); s2DMoveVel = _T(""); s2DMoveAcc = _T(""); s2DOneShotRemainLen = _T("");
		sEngAoiLen = _T(""); sEngFdDiffMax = _T(""); sEngFdDiffRng = _T(""); sEngFdDiffNum = _T("");
		sEngBuffInitPos = _T(""); sEngBuffCurrPos = _T("");

	}

}MotionMotion;

typedef struct stMotionAxis
{
	InfoAxisMap stAxisMap;
	// 	int		nNumMotor;
	// 	int		nMappingMotor[5];
	CString sName;
	double	fInpRange;		// Inposition Range [mm]
	double	fMaxAccel;		// Maximum Acceleration in G
	double	fMinJerkTime;	// Minimum Jerk Time in Sec
	double	fSpd;			// Speed
	double	fAcc;			// Acceleration
	double	fDec;			// Deceleration
	double  fAccPeriod;		// each acceleration period
	double	fJogFastSpd;	// Speed
	double	fJogMidSpd;		// Speed
	double	fJogLowSpd;		// Speed
	double	fJogAcc;		// Acceleration
}MotionAxis;

typedef struct stMotionMotor
{
	BOOL	bType;			// Motor Type 0: servo 1: stepper
	BOOL    nDir;			// Motor Direction 0: CCW 1: CW
	double	fLeadPitch;		// ex) Ball Screw Lead Pitch
	BOOL    bAmpFaultLevel;	// Amplifier Fault Level 0: Low Active 1: High Active -1:Invalid
	BOOL    bAmpEnableLevel;// Amplifier Enable Level 0: Low Active 1: High Active
	BOOL    bPosLimitLevel;	// Positive Limit Sensor active Level 0: Low Active 1: High Active
	BOOL    bNegLimitLevel;	// Negative Limit Sensor active Level 0: Low Active 1: High Active
	BOOL    bHomeLevel;		// Home Sensor active Level 0: Low Active 1: High Active
	double  fPosLimit;		// Positive Software Limit
	double  fNegLimit;		// Negative Software Limit
	BOOL    bEncoder;		// External encoder feedback
	int		nEncPulse;		// Encoder Pulse 
	int		nEncMul;		// Encoder Multiplier 
	double	fGearRatio;		// Gear Ratio
	double	fRatingSpeed;	// Position

	stMotionMotor::stMotionMotor()
	{
		bType = 0;
	}
}MotionMotor;

typedef struct stMotionFilter
{
	double fP;
	double fI;
	double fD;
	double fPff;
	double fVff;
	double fAff;
	double fFff;
	double fIMaxMoving;
	double fIMaxRest;
	double fDRate;
	double fOutputLimit;
	double fOutputLimitHigh;
	double fOutputLimitLow;
	double fOutputOffset;
}MotionFilter;


typedef struct stMotionSingle
{
	CString sName;
	MotionMotion Motion;
	MotionAxis Axis;
	MotionMotor Motor;
	MotionFilter Filter;
	MotionHome Home;
}MotionSingle;

typedef struct stMotionControl
{
	int nTotMotion;
	int nTotAxis;
	int nTotMotor;
	int nTotFilter;
	int nTotCapture;
	int nTotSequence;
	int nSynqnetTxTime;
	int nFirmwareOption;
}MotionControl;


#define POSITIVE_DIR	1
#define NEGATIVE_DIR	-1
#define STOP_DIR		0

#define HOME_SENSOR		1
#define INDEX_SIGNAL	2
#define POS_SENSOR      3
#define OFF_CAPTURE		4

// Motion enumeration variables
typedef enum stSPEED_CONTROL_MODE{NO_OPTIMIZED = 0,OPTIMIZED = 1} SPEED_CONTROL_MODE; 
typedef enum stSCAN_MODE{UNI_DIRECTION = 0,BIDIRECTION = 1} SCAN_MODE; 
typedef enum stSCAN_DIRECTION{BACKWARD = -1,FORWARD = 1} SCAN_DIRECTION; 
typedef enum stenMotion{CCW_MOVE = -1, CW_MOVE = 1, STOP_MOVE} enMotion;
// typedef enum {OFF,ON} OnOff;
typedef enum stDirection{MINUS=-1,PLUS=1} Direction;
typedef enum stInOutLogicLevel{ACTIVE_LOW = FALSE, ACTIVE_HIGH=TRUE} InOutLogicLevel;
typedef enum stenMotionType{SERVO=0,STEPPER} enMotionType;
typedef enum stenMotionParam{MOTOR_PARAM,AXIS_PARAM,SPEED_PARAM,HOME_PARAM,JOG_PARAM,LIMIT_PARAM} enMotionParam;
typedef enum stenMotionStatus{PREVIOUS=-1,STOPED=0,NEXT=1} enMotionStatus;
typedef enum stJog_Speed{LOW_SPEED=0,MIDDLE_SPEED=1,HIGH_SPEED=2} Jog_Speed;

typedef enum stCoordinate{INC=FALSE, ABS=TRUE} Coordinate;
typedef enum stMotionWait { NO_WAIT = FALSE, WAIT = TRUE } MotionWait;
typedef enum stTowerLamp { LAMP_RED = 0, LAMP_YELLOW, LAMP_GREEN, LAMP_FLASH_RED, LAMP_FLASH_YELLOW, LAMP_ALL_OFF, LAMP_PREV } TowerLamp;

enum SliceDI {
	DI_ESTOP = 0, DI_SAFETY_SENSOR = 1, DI_CLAMP_SWITCH = 2, DI_CLAMP_GA11_ON = 3, DI_CLAMP_GA11_OFF = 4, DI_CLAMP_GA12_ON = 5, DI_CLAMP_GA12_OFF = 6,
	DI_VACUUM = 8, DI_OPEN_DOOR2 = 9, DI_OPEN_DOOR1 = 10, DI_OPEN_DOOR3 = 11, DI_UNLOCK_DOOR2 = 12, DI_UNLOCK_DOOR1 = 13, DI_UNLOCK_DOOR3 = 14, DI_3D_CYLINDER_UP = 15,
	DI_COVER_SENSOR = 16, DI_LD_TEMP_ALARM = 17, DI_LASER_TEMP_ALARM = 18, DI_CLAMP_GA13_ON = 19, DI_CLAMP_GA13_OFF = 20, DI_CLAMP_GA14_ON = 21, DI_CLAMP_GA14_OFF = 22,
	DI_3D_CYLINDER_DOWN = 23, DI_READY_LASER_POWER = 24, DI_READY_LD = 25, DI_CYLINDER_OFF = 26, DI_CYLINDER_ON = 27, DI_LD_READY = 28, DI_LD_PANEL_IN_DONE = 29, DI_LD_PANEL_OUT_DONE = 30,
	DI_LD_MANUAL = 31
};

enum SliceDO {
	DO_CLAMP_SWITCH_LED = 3, DO_CLAMP_ON = 4, DO_CLAMP_OFF = 5, DO_VACUUM_DESTROY = 6, DO_BUZZER = 7, DO_STARTLAMP = 8, DO_READYLAMP = 9,
	DO_SOLSMOG = 10, DO_SOL3 = 11, DO_SOLCOVER1 = 12, DO_LASER_POINTER = 13, DO_3D_CYLINDER = 14, DO_BLOW3 = 15, DO_MC = 16, DO_RINGBLOWER = 17,
	DO_SOLPANEL = 19, DO_TOWERR = 20, DO_TOWERY = 21, DO_TOWERG = 22, DO_LD_ON_OFF = 23, DO_LD_PANEL_IN = 24, DO_LD_PANEL_OUT = 25, DO_LD_ERROR = 26, DO_LD3 = 27, DO_CYLINDER = 28,
	DO_COOLER = 29, DO_BLOW = 30, DO_COOLER2 = 31
};


/* MPIState */
typedef enum MPIState {
	MPIStateINVALID = -1,

	MPIStateIDLE,
	MPIStateMOVING,
	MPIStateSTOPPING,
	MPIStateSTOPPED,
	MPIStateSTOPPING_ERROR,
	MPIStateERROR,

	MPIStateEND,
	MPIStateFIRST = MPIStateINVALID + 1
} MPIState;

/* MPIAction */
typedef enum MPIAction {
	MPIActionINVALID = -1,

	MPIActionNONE,
	MPIActionTRIGGERED_MODIFY,
	MPIActionSTOP,
	MPIActionABORT,
	MPIActionE_STOP,
	MPIActionE_STOP_ABORT,
	MPIActionE_STOP_CMD_EQ_ACT,
	MPIActionE_STOP_MODIFY,

	/* Differing priority support for E_STOP_MODIFY,
	PRIORITY_0 is the highest and the default
	*/
	MPIActionE_STOP_MODIFY_PRIORITY_0 = MPIActionE_STOP_MODIFY,
	MPIActionE_STOP_MODIFY_PRIORITY_1,
	MPIActionE_STOP_MODIFY_PRIORITY_2,
	MPIActionE_STOP_MODIFY_PRIORITY_3,
	MPIActionE_STOP_MODIFY_PRIORITY_4,
	MPIActionE_STOP_MODIFY_PRIORITY_5,
	MPIActionE_STOP_MODIFY_PRIORITY_6,
	MPIActionE_STOP_MODIFY_PRIORITY_7,

	MPIActionDONE,
	MPIActionSTART,
	MPIActionRESUME,
	MPIActionRESET,
	MPIActionCANCEL_REPEAT,

	MPIActionEND,
	MPIActionFIRST = MPIActionINVALID + 1
} MPIAction;


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOTIONPARAM_H__INCLUDED_)

