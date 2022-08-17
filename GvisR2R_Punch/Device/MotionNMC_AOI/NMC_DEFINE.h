/******************************************************************************
* File name : NMC_DEFINE.h
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

#ifndef __NMCAOI_H__
#define __NMCAOI_H__


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
//#define		HIGH				1
//#define		LOW  				0

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


/*---------------------------------------------------------------------------*/
/* Event defines                                                             */
/*---------------------------------------------------------------------------*/
#define		NO_EVENT			0 	/* ignore a condition */
#define		STOP_EVENT			1	/* generate a stop event */
#define 	E_STOP_EVENT		2 	/* generate an e_stop event */
#define		ABORT_EVENT			3 	/* disable PID control, and disable the amplifier */

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
/* General Defines                                                           */
/*---------------------------------------------------------------------------*/
// #define		POSITIVE		1
// #define		NEGATIVE		0

/*---------------------------------------------------------------------------*/
/* Motor Type                                                                */
/*---------------------------------------------------------------------------*/
#define		SERVO_MOTOR		0
#define		STEPPER			1
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

//#define		CHAR		char
//#define		INT			short
//#define		LONG		long
//#define		ULONG		unsigned long
//#define		FLOAT		float
//#define		DOUBLE		double

#define		pCHAR		CHAR *
#define     pINT		INT *
#define		pLONG		LONG *
#define		pULONG		unsigned long *
#define		pFLOAT		FLOAT *
#define		pDOUBLE		double *

#define		VOID		void


//typedef enum tagMotionCommandError
//{
//}MotionCommandError;

/*---------------------------------------------------------------------------*/
/* Trigger defines                                                           */
/*---------------------------------------------------------------------------*/

#define		TRIGGER_ETHERCAT_ADDR	9
	
#define		TRIGGER_CAMERA_VX		1
#define		TRIGGER_LASER_F_VX		2
#define		TRIGGER_LASER_B_VX		3
#define		TRIGGER_VERIFY_VX		1

#define		TRIGGER_CAMERA_IDX		0x2410
#define		TRIGGER_LASER_F_IDX		0x2810
#define		TRIGGER_LASER_B_IDX		0x2C10
#define		TRIGGER_VERIFY_IDX		0x3410

/*-------------------------------------------------------------------------------------*/
/* Errormap Compensation defines                                                       */
/*-------------------------------------------------------------------------------------*/
#define		EC_ERROR_PARAM1					0x00030001			// 1번째 파라미터 이상.
#define		EC_ERROR_PARAM2					0x00030002			// 2번째 파라미터 이상.
#define		EC_ERROR_PARAM3					0x00030003			// 3번째 파라미터 이상.
#define		EC_ERROR_PARAM4					0x00030004			// 4번째 파라미터 이상.
#define		EC_ERROR_PARAM5					0x00030005			// 5번째 파라미터 이상.
#define		EC_ERROR_ADJUST_EC_0000			0x000F0000			// EC Table을 적용할 수 없는 Group 상태.
#define		EC_ERROR_ADJUST_EC_FFFF			0x000FFFFF			// EC Table을 적용할 수 없는 Group 상태.
#define		EC_ERROR_NOT_ALLOC_GROUP		0x00110000			// 구성되어 있지 않음 Group (Member가 할당되어 있지 않음)
#define		EC_ERROR_3D_CMDPOS				0x001C0003			// 3D 명령위치가 규칙을 위반함.
#define		EC_ERROR_3D_MESPOS				0x001D0003			// 3D 측정위치가 규칙을 위반함.
#define		EC_ERROR_2D_CMDPOS				0x001C0002			// 2D 명령위치가 규칙을 위반함.
#define		EC_ERROR_2D_MESPOS				0x001D0002			// 2D 측정위치가 규칙을 위반함.
#define		EC_ERROR_1D_CMDPOS				0x001C0001			// 1D 명령위치가 규칙을 위반함.
#define		EC_ERROR_1D_MESPOS				0x001D0001			// 1D 측정위치가 규칙을 위반함.
#define		EC_ERROR_PROHIBIT_CHG_TABLE		0x001E0019			// EC Table을 상태를 변경할 수 없는 서보상태.
#define		EC_ERROR_BLANK_GANTRY_ID		0x001E001A			// 구성되어 있지 않음 Gantry ID.
#define		EC_ERROR_ALREADYED_ENABLE		0x001E0020			// EC가 설정된 상태.
#define		EC_ERROR_ALREADYED_DISABLE		0x001E0021			// EC가 해제된 상태.
#define		EC_ERROR_TABLE_DIMENSION		0x001E0022			// EC Table과 Dimension이 불일치함.
#define		EC_ERROR_NOT_EXIST_TABLE		0x001E0023			// EC Table 내용이 없음.
#define		EC_ERROR_CHG_TABLE_LAST_IDX		0x001E0024			// EC Table Last Index가 변경됨.

#define NMC_EC_PATH		"C:\\AoiSet\\EC_Data_Table_Sample_2D.ecf"		// EC : Errormap Compensation
#define NMC_EC_TYPE_FILE		1										// 0: 1D , 1: 2D , 2: 3D
#define NMC_EC_TYPE_MODE		2										// 0: Axis , 1: Gantry , 2: Group
#define NMC_EC_MAPID			1										// Range : 0 ~ 3 [ Table1 ~ Table4 ]
#define NMC_EC_AXISID			0										// Axis (Axis ID), Gantry (0~3), Group (0~15)
																		// 2 : CAM Axis
//#define NMC_GROUPID_ERRMAP				0;
//#define NMC_GROUPID_RTAF				1;
//#define NMC_GROUPID_INTERPOLRATION		2;
typedef enum { NMC_GROUPID_ERRMAP = 0, NMC_GROUPID_RTAF = 1, NMC_GROUPID_INTERPOLRATION = 2, NMC_GROUPID_TOTAL = 3} EC_GROUP_INDEX;
typedef enum { EC_ST_DISABLED = 0, EC_ST_ENABLED = 1, EC_ST_DISABLING = 2, EC_ST_ENABLING = 3 } EC_STATUS;
typedef enum { EC_TP_AXIS = 0, EC_ST_GANTRY = 1, EC_ST_GROUP_2D = 2, EC_ST_GROUP_3D = 3 } EC_TYPE;

#define USE_NMC_HOME			0


#ifdef __cplusplus
	}
#endif

#endif