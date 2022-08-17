/******************************************************************************
* File name : NMC_R2R.h
* Version   : 2.00
* Date      : 2011.06.30
* Version history
*     2010.07.23 : 1.5  : Brand Transition to RS Automation RS OEMAX
*     2010.08.13 : 1.51 : Added get_option_io_bit
*     2011.01.26 : 1.53 : Boundary check enabled
*     2011.02.16 : 1.54 : Control Timer 수정: (250 usec 제거) 
*     2011.05.02 : 1.55 : dll 종료시 homming thread 종료 루틴 변경 -> app 비정상 종료시 NMC_VRS.dll 정상 종료되지 않음 건 개선
*     2011.06.30 : 2.00 : New Device Driver for ASUS P8P67 main board
                          Board Reset API 지원
                          mmc_initx 에서 사용자가 입력한 board number 확인 후 설치된 보드 숫자와 맞지 않는 경우 에러 리턴
*     2011.09.07 : 2.01 : Device Driver 교체 (Driver Board 인식 순서 수정)
*     2014.01.07 : 2.20 : Joystick MPG 추가
*                         frames_left() 함수의 Return 값을 하나씩 줄여서 돌려줌. -> buffer full check 가능
*                         WaitForSingleObject() 의 Return 값 확인 Case 추가 : WAIT_ABANDONED          
******************************************************************************/

#ifndef __NMCR2R_H__
#define __NMCR2R_H__


#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
/* True and False                                                            */
/*---------------------------------------------------------------------------*/
#define		TRUE				1
#define		FALSE				0

/*---------------------------------------------------------------------------*/
/* High and Low                                                              */
/*---------------------------------------------------------------------------*/
#define		HIGH				1
#define		LOW  				0

/*---------------------------------------------------------------------------*/
/* Circle Move Direction                                                     */
/*---------------------------------------------------------------------------*/
#define		CIR_CCW 			1
#define		CIR_CW  			0

/*---------------------------------------------------------------------------*/
/* Coordinate  Direction                                                     */
/*---------------------------------------------------------------------------*/
#define		CORD_CCW 			1
#define		CORD_CW  			0

/*---------------------------------------------------------------------------*/
/* Encoder  Direction                                                        */
/*---------------------------------------------------------------------------*/
#define		ENCODER_CCW 		1
#define		ENCODER_CW  		0


/*
typedef enum tagStateBit {
// 정상동작 가능 상태
sSTATE_OK = 0x0001,
// 원점(home),+ Limit, - Limit, AMP Fault, +SWLimit, -SWLimit 감지
sHOME_SWITCH = 0x0002,
sHW_POS_LIMIT = 0x0004,
sHW_NEG_LIMIT = 0x0008,
sAMP_FAULT = 0x0020,
sSW_NEG_LIMIT = 0x0040,
sSW_POS_LIMIT = 0x0080,
// 비정상 정지 상태
sERROR_STOP = 0x0200,
// Frame Buffer가 가득찬 상태
sOUT_OF_BUFFER = 0x0400,
// AMP Disable 상태일때 발생
sAMP_POWER_OFF = 0x0800,
// 축 동작 완료 상태
sAXIS_COMPLETE = 0x2000,
} STATE_BIT;
*/
/*---------------------------------------------------------------------------*/
/* Event Source Status defines                                               */
/*---------------------------------------------------------------------------*/
#define		ST_ERROR_STOP			0x00000000	// NONE
#define		ST_NONE					0x00000001	// BIT 0
#define		ST_AMP_POWER_ONOFF		0x00000002	// BIT 1
#define		ST_STOPPING				0x00000004	// BIT 2
#define		ST_RUN_STOP_COMMAND		0x00000008	// BIT 3
#define		ST_PC_COMMAND			0x00000010	// BIT 4
#define		ST_VEL_RUNNING			0x00000020	// BIT 5
#define		ST_SYNC_RUNNING			0x00000040	// BIT 6
#define		ST_HOMING				0x00000080	// BIT 7
#define		ST_X_POS_LIMIT			0x00000100	// BIT 8	ST_POS_SW_LIMIT
#define		ST_X_NEG_LIMIT			0x00000200	// BIT 9	ST_NEG_SW_LIMIT
#define		ST_CONST_MOVING			0x00000400	// BIT 10
#define		ST_ACCELERATING			0x00000800	// BIT 11
#define		ST_DECELERATING			0x00001000	// BIT 12
#define		ST_POS_MOVING			0x00002000	// BIT 13
#define		ST_NEG_MOVING			0x00004000	// BIT 14
#define		ST_NEG_LIMIT			0x00008000	// BIT 15
#define		ST_POS_LIMIT			0x00010000	// BIT 16
#define		ST_HOME_SWITCH			0x00020000	// BIT 17
#define		ST_POS_LIMIT_EVENT		0x00040000	// BIT 18
#define		ST_NEG_LIMIT_EVENT		0x00080000	// BIT 19
#define		ST_AMP_FAULT			0x00100000	// BIT 20
#define		ST_COLLISION_STATE		0x00200000	// BIT 21	ST_SENS_STOP
#define		ST_IDLE					0x00400000	// BIT 22
#define		ST_AMP_ENABLE			0x00800000	// BIT 23
#define		ST_HOME_DONW			0x01000000	// BIT 24
#define		ST_WARNNING				0x02000000	// BIT 25
#define		ST_INPOSITION_STATUS	0x04000000	// BIT 26
#define		ST_GEARING_MOVE			0x08000000	// BIT 27
#define		ST_GROUP_ENABLE			0x10000000	// BIT 28
#define		ST_BUFFER_FULL			0x20000000	// BIT 29
#define		ST_SPARE_BIT			0x40000000	// BIT 30
#define		ST_DEBUGGING			0x80000000	// BIT 31

#define		ST_A_LIMIT    			ST_SPARE_BIT
#define		ST_V_LIMIT  			ST_SPARE_BIT
#define		ST_ERROR_LIMIT			(ST_X_POS_LIMIT+ST_X_NEG_LIMIT)
#define		ST_OUT_OF_FRAMES    	ST_SPARE_BIT
#define		ST_ABS_COMM_ERROR   	ST_SPARE_BIT


//#define		ST_NONE					0x0000
//#define		ST_HOME_SWITCH			0x0001
//#define		ST_POS_LIMIT			0x0002
//#define		ST_NEG_LIMIT   			0x0004
//#define		ST_AMP_FAULT			0x0008
//#define		ST_A_LIMIT    			0x0010
//#define		ST_V_LIMIT  			0x0020
//#define		ST_X_NEG_LIMIT 			0x0040
//#define		ST_X_POS_LIMIT			0x0080
//#define		ST_ERROR_LIMIT			0x0100
//#define		ST_PC_COMMAND  			0x0200
//#define		ST_OUT_OF_FRAMES    	0x0400
//#define		ST_AMP_POWER_ONOFF  	0x0800
//#define		ST_ABS_COMM_ERROR   	0x1000
//#define		ST_INPOSITION_STATUS	0x2000
//#define		ST_RUN_STOP_COMMAND 	0x4000 		
//#define		ST_COLLISION_STATE  	0x8000 		
//#define		ST_PAUSTATE_STATE  		0x10000 		

/*---------------------------------------------------------------------------*/
/* Event defines                                                             */
/*---------------------------------------------------------------------------*/
#define		NO_EVENT			0 	/* ignore a condition */
#define		STOP_EVENT			1	/* generate a stop event */
#define 	E_STOP_EVENT		2 	/* generate an e_stop event */
#define		ABORT_EVENT			3 	/* disable PID control, and disable the amplifier */
#define		DEFAULT_EVENT		4

/*---------------------------------------------------------------------------*/
/* Digital Filter Defines                                                    */
/*---------------------------------------------------------------------------*/
#define		GAIN_NUMBER			5	/* elements expected get/set_filter(...) */
#define		GA_P				0	/* proportional gain */
#define		GA_I				1	/* integral gain */
#define		GA_D				2	/* derivative gain-damping term */
#define		GA_F       			3	/* velocity feed forward */
#define		GA_ILIMIT 			4	/* integration summing limit */

/*---------------------------------------------------------------------------*/
/* Error Defines                                                             */
/*---------------------------------------------------------------------------*/
//#define		MAX_ERROR_LEN							80	/* maximum length for error massage string */
//#define		MMC_OK									0	/* no problems */
//#define		MMC_NOT_INITIALIZED						1	/* be sure to call mmc_init(...) */
//#define		MMC_TIMEOUT_ERR							2	/* DPRAM Communication Error */
//#define		MMC_INVALID_AXIS						3	/* axis out of range or not configured */
//#define		MMC_ILLEGAL_ANALOG						4	/* analog channel < 0 or > 4 */
//#define		MMC_ILLEGAL_IO							5	/* illegal I/O port */
//#define		MMC_ILLEGAL_PARAMETER					6	/* move with zero accel or velocity */
//#define		MMC_NO_MAP								7 	/* The map_axes(...) funcation has not been called */
//#define		MMC_AMP_FAULT							8 	/* amp fault occured */
//#define		MMC_ON_MOTION							9 	/* Motion is not completed */
//#define		MMC_NON_EXIST							10	/* MMC Board is not exist */
//#define		MMC_BOOT_OPEN_ERROR						11	/* MMC Boot File Read/Write Error*/
//#define		MMC_CHKSUM_OPEN_ERROR					12	/* MMC Checksum File Read/Write Error*/
//#define		MMC_WINNT_DRIVER_OPEN_ERROR				13	/* MMC Windows NT Driver Open Error*/
//#define		MMC_EVENT_OCCUR_ERROR					14
//#define		MMC_AMP_POWER_OFF     					15
//#define		MMC_DATA_DIRECTORY_OPEN_ERROR			16
//#define		MMC_INVALID_CPMOTION_GROUP				17
//#define		MMC_VELOCITY_ILLEGAL_PARAMETER			18	/* move with zero accel or velocity */
//#define		MMC_ACCEL_ILLEGAL_PARAMETER				19	/* move with zero accel or velocity */
//#define     MMC_BOARD_COUNT_MISMATCH                20  /* MMC Board count is mismatch with user assigned */
//#define		FUNC_ERR								-1	/* Function Error				*/
//
////homing api return
//#define		MMC_HOMING_DONE							0
//#define		MMC_HOMING_ERROR						1
//#define		MMC_HOMING_WORKING						2
//#define		MMC_HOMING_TIMEOUT						3
//#define		MMC_HOMING_STOP							4

/*---------------------------------------------------------------------------*/
/* General Defines                                                           */
/*---------------------------------------------------------------------------*/
//#define		POSITIVE		1
//#define		NEGATIVE		0

/*---------------------------------------------------------------------------*/
/* Motor Type                                                                */
/*---------------------------------------------------------------------------*/
#define		SERVO_MOTOR		0
#define		STEPPER_MOTOR			1
#define		MICRO_STEPPER	2

/*---------------------------------------------------------------------------*/
/* Feedback Configuration                                                    */
/*---------------------------------------------------------------------------*/
#define		FB_ENCODER		0
#define		FB_UNIPOLAR		1
#define		FB_BIPOLAR		2

/*---------------------------------------------------------------------------*/
/* Control_Loop Method                                                       */
/*---------------------------------------------------------------------------*/
#define		OPEN_LOOP		0
#define		CLOSED_LOOP		1
#define		SEMI_LOOP		2

/*---------------------------------------------------------------------------*/
/* Control Method                                                            */
/*---------------------------------------------------------------------------*/
#define		V_CONTROL		0
#define		T_CONTROL		1

#define		IN_STANDING		0
#define		IN_ALWAYS		1

#define		TWO_PULSE		0
#define		SIGN_PULSE		1
/*---------------------------------------------------------------------------*/
/* Limit Vlaue                                                               */
/*---------------------------------------------------------------------------*/
//#define		MMC_ACCEL_LIMIT		25000
//#define		MMC_VEL_LIMIT		5000000
//#define		MMC_POS_SW_LIMIT	2147483640
//#define		MMC_NEG_SW_LIMIT	-2147483640
//#define		MMC_ERROR_LIMIT		35000
//#define		MMC_PULSE_RATIO		255

/*---------------------------------------------------------------------------*/
// Homing Config and Option
/*---------------------------------------------------------------------------*/
//homing direction
//#define		MMC_HOMING_DIR_MAIN_NEG       0x0000      // main homing direction negative
//#define		MMC_HOMING_DIR_MAIN_POS       0x0001      // main homing direction positive
//#define		MMC_HOMING_DIR_MARKER_NEG     0x0000      // marker homing direction negative
//#define		MMC_HOMING_DIR_MARKER_POS     0x0002      // marker homing direction positive
//homing config
//#define		MMC_HOMING_EDGE_HOME_NEG      0x0001      // homing edge of home sensor negative
//#define		MMC_HOMING_EDGE_HOME_POS      0x0002      // homing edge of home sensor positive
//#define		MMC_HOMING_EDGE_LN			  0x0004      // homing edge of negative limit
//#define		MMC_HOMING_EDGE_LP			  0x0008      // homing edge of positive limit
//#define		MMC_HOMING_MARKER_USE	      0x0010	  // homing use marker
		
/*---------------------------------------------------------------------------*/
/* Type Define                                                               */
/*---------------------------------------------------------------------------*/
#define		CHAR		char
#define		INT			short
#define		LONG		long
#define		ULONG		unsigned long
#define		FLOAT		float
#define		DOUBLE		double
	
#define		pCHAR		CHAR *
#define     pINT		INT *
#define		pLONG		LONG *
#define		pULONG		unsigned long *
#define		pFLOAT		FLOAT *
#define		pDOUBLE		double *

#define		VOID		void


#define NMC_EC_PATH		"C:\\R2RSet\\EC_Data_Table_Sample_2D.ecf"		// EC : Errormap Compensation
#define NMC_EC_TYPE_FILE		1										// 0: 1D , 1: 2D , 2: 3D
#define NMC_EC_TYPE_MODE		2										// 0: Axis , 1: Gantry , 2: Group
#define NMC_EC_MAPID			1										// Range : 0 ~ 3 [ Table1 ~ Table4 ]
#define NMC_EC_AXISID			0										// Axis (Axis ID), Gantry (0~3), Group (0~15)
																		// 2 : CAM Axis

//typedef enum { NMC_DEVICE_BOARD = 0, NMC_DEVICE_IOIN = 2001, NMC_DEVICE_IOOUT = 2100 } NMC_DEVICE_ID;
typedef enum { NMC_DEVICE_BOARD = 0, NMC_DEVICE_IOIN = 19, NMC_DEVICE_IOOUT = 19 } NMC_DEVICE_ID;
typedef enum { NMC_GROUPID_INTERPOLRATION_X0Y0 = 0, NMC_GROUPID_INTERPOLRATION_X1Y1 = 1, NMC_GROUPID_TOTAL = 2 } EC_GROUP_INDEX;

//#define NMC_GROUPID_ERRMAP				0;
//#define NMC_GROUPID_RTAF				1;
//#define NMC_GROUPID_INTERPOLRATION		2;
//typedef enum { NMC_GROUPID_ERRMAP = 0, NMC_GROUPID_RTAF = 1, NMC_GROUPID_INTERPOLRATION = 2, NMC_GROUPID_TOTAL = 3} EC_GROUP_INDEX;
typedef enum { EC_ST_DISABLED = 0, EC_ST_ENABLED = 1, EC_ST_DISABLING = 2, EC_ST_ENABLING = 3 } EC_STATUS;
typedef enum { EC_TP_AXIS = 0, EC_ST_GANTRY = 1, EC_ST_GROUP_2D = 2, EC_ST_GROUP_3D = 3 } EC_TYPE;

typedef enum { MOTION_SUCCESS = 0, MOTION_LIMIT_OVER = 1, OPERATE_INTERLOCK = 2, WAIT_TIME_OVER = 3, EMERGENCY_STOP = 4 } NMC_ERROR_CODE;
typedef enum { TRAPEZOIDAL = 0, S_CURVE = 1 } SpeedProfile;

#define USE_NMC_HOME			0
#define USE_INTERPOLATION_MOVE		0


#ifdef __cplusplus
}
#endif

#endif