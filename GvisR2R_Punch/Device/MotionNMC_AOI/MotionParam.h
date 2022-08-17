#if !defined(AFX_MOTIONPARAM_H__INCLUDED_)
#define AFX_MOTIONPARAM_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MotionParam.h : header file
//
//#include "stdmpi.h"
//#include "stdmei.h"
//#include "apputil.h"
//#include "soonhan_io2k.h"

#define POSITIVE_DIR	1
#define NEGATIVE_DIR	-1
#define STOP_DIR		0

#define HOME_SENSOR		1
#define INDEX_SIGNAL	2
#define POS_SENSOR      3
#define OFF_CAPTURE		4

// Motion enumeration variables
typedef enum {NO_OPTIMIZED = 0,OPTIMIZED = 1} SPEED_CONTROL_MODE; 
typedef enum {UNI_DIRECTION = 0,BIDIRECTION = 1} SCAN_MODE; 
typedef enum {BACKWARD = -1,FORWARD = 1} SCAN_DIRECTION; 
typedef enum {CCW_MOVE = -1, CW_MOVE = 1, STOP_MOVE} enMotion;
// typedef enum {OFF,ON} OnOff;
typedef enum {MINUS=-1,PLUS=1} Direction;
typedef enum {ACTIVE_LOW = FALSE, ACTIVE_HIGH=TRUE} InOutLogicLevel;
typedef enum {SERVO=0,STEPPER} enMotionType;
typedef enum {MOTOR_PARAM,AXIS_PARAM,SPEED_PARAM,HOME_PARAM,JOG_PARAM,LIMIT_PARAM} enMotionParam;
typedef enum {PREVIOUS=-1,STOPED=0,NEXT=1} enMotionStatus;
typedef enum {LOW_SPEED=0,MIDDLE_SPEED=1,HIGH_SPEED=2} Jog_Speed;
typedef enum {TRAPEZOIDAL=0,S_CURVE,PARABOLIC} SpeedProfile;
typedef enum {INC=FALSE, ABS=TRUE} Coordinate;
typedef enum {NO_WAIT=FALSE, WAIT=TRUE} MotionWait;

//typedef enum {
//	/* only upper 16 bits defined, lower 16 bits are MEIXmpMotorDedicatedFlags */
//	MEIXmpMotorIOBitConfigurableFIRST	= 16,
//	MEIXmpMotorIOBitConfigurable0		= MEIXmpMotorIOBitConfigurableFIRST,
//	MEIXmpMotorIOBitConfigurable1		= 17,
//	MEIXmpMotorIOBitConfigurable2		= 18,
//	MEIXmpMotorIOBitConfigurable3		= 19,
//	MEIXmpMotorIOBitConfigurable4		= 20,
//	MEIXmpMotorIOBitConfigurable5		= 21,
//	MEIXmpMotorIOBitConfigurable6		= 22,
//	MEIXmpMotorIOBitConfigurable7		= 23,
//	MEIXmpMotorIOBitConfigurable8		= 24,
//	MEIXmpMotorIOBitConfigurable9		= 25,
//	MEIXmpMotorIOBitConfigurable10		= 26,
//	MEIXmpMotorIOBitConfigurable11		= 27,
//	MEIXmpMotorIOBitConfigurable12		= 28,
//	MEIXmpMotorIOBitConfigurable13		= 29,
//	MEIXmpMotorIOBitConfigurable14		= 30,
//	MEIXmpMotorIOBitConfigurable15		= 31,
//	MEIXmpMotorIOBitConfigurableLAST,
//	MEIXmpMotorIOBitConfigurableCOUNT	= (MEIXmpMotorIOBitConfigurableLAST - MEIXmpMotorIOBitConfigurableFIRST),
//
//	/* for backward compatability */
//	MEIXmpMotorIOBitXCVR_A				= MEIXmpMotorIOBitConfigurable0,
//	MEIXmpMotorIOBitXCVR_B				= MEIXmpMotorIOBitConfigurable1,
//	MEIXmpMotorIOBitXCVR_C				= MEIXmpMotorIOBitConfigurable2,
//	MEIXmpMotorIOBitXCVR_D				= MEIXmpMotorIOBitConfigurable3,
//	MEIXmpMotorIOBitXCVR_E				= MEIXmpMotorIOBitConfigurable4,
//	MEIXmpMotorIOBitXCVR_F				= MEIXmpMotorIOBitConfigurable5,
//	MEIXmpMotorIOBitUSER				= MEIXmpMotorIOBitConfigurable6,	/* depricated */
//	MEIXmpMotorIOBitUSER_IN_0			= MEIXmpMotorIOBitConfigurable6,
//	MEIXmpMotorIOBitUSER_OUT_0			= MEIXmpMotorIOBitConfigurable7,
//	MEIXmpMotorIOBitUSER_IN_1			= MEIXmpMotorIOBitConfigurable8,
//	MEIXmpMotorIOBitUSER_OUT_1			= MEIXmpMotorIOBitConfigurable9,
//	MEIXmpMotorIOBitENC_FAULT			= MEIXmpMotorDedicatedInputFlagsENC_FAULT,
//	MEIXmpMotorIOBitBWDET  				= MEIXmpMotorDedicatedInputFlagsENC_FAULT,
//	MEIXmpMotorIOBitILLDET				= MEIXmpMotorDedicatedInputFlagsENC_FAULT,
//	MEIXmpMotorIOBitINDEX				= MEIXmpMotorDedicatedInputFlagsENC_INDEX0,
//	MEIXmpMotorIOBitPOS_LIMIT			= MEIXmpMotorDedicatedInputFlagsPOS_LIMIT,
//	MEIXmpMotorIOBitNEG_LIMIT			= MEIXmpMotorDedicatedInputFlagsNEG_LIMIT,
//	MEIXmpMotorIOBitHOME				= MEIXmpMotorDedicatedInputFlagsHOME,
//	MEIXmpMotorIOBitAMP_FAULT			= MEIXmpMotorDedicatedInputFlagsFAULT,
//	MEIXmpMotorIOBitAMP_ENABLE			= MEIXmpMotorDedicatedOutputFlagsAMP_ENABLE,
//} MEIXmpMotorIOBit;






typedef struct stControl
{
	int nTotMotion;
	int nTotAxis;
	int nTotMotor;
	int nTotFilter;
	int nTotCapture;
	int nTotSequence;
}Control;

typedef struct stHome
{
	BOOL    bAct;				// 0: Don't 1:Do
	int 	nDir;				//Initial Home Dir, 1:plus-dir	-1:minus-dir
	double  fEscLen;			// Escape length from sensor touch position
	double	f1stSpd;			// PreHomming Speed
	double	f2ndSpd;			// Homming Speed
	double	fAcc;				// Homming Accel
	double	fShift;				// shift
	double	fOffset;			// offset
	double	fMargin;			// Margin between Origin & index After Homming
}Home;

typedef struct stSpeed
{
	double	fSpd;			// Speed
	double	fAcc;			// Acceleration
	double	fDec;			// Deceleration
	double  fAccPeriod;		// each acceleration period
	double	fJogFastSpd;	// Speed
	double	fJogMidSpd;		// Speed
	double	fJogLowSpd;		// Speed
	double	fJogAcc;		// Acceleration
}Speed;

typedef struct stMsg
{
	CString strTitleMsg;
	CString strMsg;
	COLORREF color;
	DWORD dwDispTime;
	BOOL bOverWrite;
}Msg;

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

typedef struct stMotion
{
	InfoMsMap	stMsMap;
// 	int			nNumAxis;
// 	int			nMappingAxis[5];
	Home		Home;
	Speed		Speed;

	double dEStopTime;
}Motion;

typedef struct stAxis
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
}Axis;

typedef struct stMotor
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

	stMotor::stMotor()
	{
		bType = 0;
	}
}Motor;

typedef struct stFilter
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
}Filter;


typedef struct stSingle
{
	CString sName;
	Motor Motor;
	Filter Filter;
	Axis Axis;
	Home Home;
}Single;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOTIONPARAM_H__INCLUDED_)

