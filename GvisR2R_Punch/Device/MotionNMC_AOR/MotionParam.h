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




/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOTIONPARAM_H__INCLUDED_)

