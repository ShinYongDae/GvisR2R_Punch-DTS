//=============================================================================
//
// MMCE Motion SDK
// RSAutomation.co.,LTD.
// 2014-03-14
// Revision History
//
//=============================================================================

#ifndef __NMCMOTIONSDK_H__
#define __NMCMOTIONSDK_H__

#include <windef.h>

#pragma warning(disable: 4091)

#ifdef FLAMENCOMOTIONAPI_EXPORTS
#define NMCMOTIONAPI __declspec(dllexport)
#else
#define NMCMOTIONAPI __declspec(dllimport)
#endif

//Data Type Definition
#ifndef INT8
typedef signed char INT8;
#endif

#ifndef UINT8
typedef unsigned char UINT8;
#endif

#ifndef INT16
typedef signed short INT16;
#endif

#ifndef UINT16
typedef unsigned short UINT16;
#endif

#ifndef INT32
typedef signed int INT32;
#endif

#ifndef UINT32
typedef unsigned int UINT32;
#endif

#ifndef DOUBLE
typedef double DOUBLE;
#endif

#ifndef REAL
typedef double REAL;
#endif

//v12.1.0.73
#ifndef FLOAT
typedef float FLOAT;
#endif

#ifndef BYTE
typedef unsigned char BYTE;
#endif

#ifndef WORD
typedef unsigned short WORD;
#endif

#ifndef DWORD
typedef unsigned long DWORD;
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif


//MC_XXXX Function Return Values.
typedef enum MC_STATUS_tag
{
	MC_OK                                   			=	0x00000000,
	
	//Device Driver Error Defines 
	MC_ERROR_HW_NOT_INSTALLED							=	0x000000DC,
	MC_ERROR_DD_SEND_ERROR								=	0x000000DD,
	MC_ERROR_DD_READ_ERROR								=	0x000000DE,
	MC_DD_ERROR_SEND									=	0x000000DF,
	MC_DD_ERROR_RECV									=	0x000000E0,
	MC_DD_OPEN_FAIL										=	0x000000E6,
	MC_DD_NOT_OPENED									=	0x000000E7,
	MC_DD_CONN_FAIL										=	0x000000E8,
	MC_DD_CLIENT_START_FAIL								=	0x000000E9,
	MC_DD_OK											=	0x00000000,
	MC_CN_NOT_CONNECTED									=	0x000000F0,
	MC_CN_CONNECTED										=	0x000000F1,
	MC_CN_CONNERROR										=	0x000000F2,

	//PLCOpen Motion Command Response Error Defines (F/W)
	MC_INVALID_SYSTEM_STATE								=	0x00010000, 
	MC_UNSUPPORT_CMD									=	0x00020000,	
	MC_INVALID_AXIS_STATE_NOT_HOMING_MODE				=   0x00020047, //v12.1.0.42 
	MC_UNSUPPORTED_CMD_AXIS_HOMING						=	0x00024148, //v12.2.0.1
	MC_UNSUPPORTED_CMD_INVALID_ETHERCAT_CYCLE_TIME      =   0x00024354, //v12.2.0.10
	MC_UNSUPPORTED_CMD_INVALID_ETHERCAT_OPERATION_MODE	=	0x0002454D, //v12.2.0.1
	MC_UNSUPPORTED_CMD_BOARD_CATALOG					=	0x00024743, //v12.2.0.1
	MC_UNSUPPORTED_CMD_GANTRY_ENABLING					=	0x00024745, //v12.2.0.1
	MC_UNSUPPORTED_CMD_GANTRY_HOMING					=	0x00024748, //v12.2.0.1
	MC_UNSUPPORTED_CMD_GANTRY_YAW_UNSTABLE				=	0x00024759, //v12.2.0.1
	MC_UNSUPPORTED_CMD_BOARD_CATALOG_IO					=   0x00024943, //v12.2.0.3
	MC_UNSUPPORTED_CMD_INVALID_REF_AXIS_STATUS		    =   0x00025241, //v12.2.0.10
	MC_UNSUPPORTED_CMD_SLAVE_HOMING						=	0x00025348, //v12.2.0.1
	MC_UNSUPPORTED_CMD_SLAVE_HOME_MODE					=	0x0002534D, //v12.2.0.1
	MC_UNSUPPORTED_CMD_TORQUE_LIMIT						=   0x0002544C, //v12.2.0.10
	MC_INVALID_PARAM									=	0x00030000,
	MC_INVALID_PARAM_1									=	0x00030001,
	MC_INVALID_PARAM_2									=	0x00030002,
	MC_INVALID_PARAM_3									=	0x00030003,
	MC_INVALID_PARAM_4									=	0x00030004,
	MC_INVALID_PARAM_5									=	0x00030005,
	MC_INVALID_PARAM_6									=	0x00030006,
	MC_INVALID_PARAM_7									=	0x00030007,
	MC_INVALID_PARAM_8									=	0x00030008,
	MC_INVALID_PARAM_9									=	0x00030009,
	MC_INVALID_PARAM_10									=	0x0003000A,	
	MC_INVALID_GANTRY_CONDITION							=	0x00030018, //v12.2.0.0
	MC_INVALID_SIZE										=	0x00040000,
	MC_INVALID_AXIS_NUM									=	0x00050000,
	MC_GANTRY_SLAVE_AXIS_CONFIG							=   0x00050017, //v12.2.0.0
	MC_NOT_ENOUGH_RESOURCE								=	0x00060000,
	MC_LIMIT_ERROR_PARAM								=	0x00070000,
	MC_LIMIT_ERROR_PARAM_1								=	0x00070001,
	MC_LIMIT_ERROR_PARAM_2								=	0x00070002,
	MC_LIMIT_ERROR_PARAM_3								=	0x00070003,
	MC_LIMIT_ERROR_PARAM_4								=	0x00070004,
	MC_LIMIT_ERROR_PARAM_5								=	0x00070005,
	MC_LIMIT_ERROR_PARAM_6								=	0x00070006,
	MC_LIMIT_ERROR_PARAM_7								=	0x00070007,
	MC_LIMIT_ERROR_PARAM_8								=	0x00070008,
	MC_LIMIT_ERROR_PARAM_9								=	0x00070009,
	MC_LIMIT_ERROR_PARAM_10								=	0x0007000A,
	MC_INVALID_DEVICE_STATE								=	0x00080000,
	MC_INVALID_DEVICE_STATE_ERROR						=	0x00080001,		
	MC_INVALID_DEVICE_STATE_GANTRY_OPERATION_MODE		=	0x00080017, //v12.2.0.0
	MC_GANTRY_UNMATCHED_ETHERCAT_CYCLE_TIME				=	0x00080025, //v12.2.0.1	
	MC_INVALID_AXIS_STATE_DISABLED						=	0x00090000,
	MC_INVALID_AXIS_STATE_STANDSTILL					=	0x00090001, //v12.1.0.42
	MC_INVALID_AXIS_STATE_DISCRETE_MOTION				=	0x00090002, //v12.1.0.42
	MC_INVALID_AXIS_STATE_CONTINUOUS_MOTION				=	0x00090003, //v12.1.0.42
	MC_INVALID_AXIS_STATE_SYNC_MOTION					=	0x00090004,
	MC_INVALID_AXIS_STATE_HOMING						=	0x00090005, //v12.1.0.42
	MC_INVALID_AXIS_STATE_STOPPING						=	0x00090006,
	MC_INVALID_AXIS_STATE_ERRORSTOP						=	0x00090007,
	MC_INVALID_AXIS_STATE_MODE_CHANGE					=	0x00090008, //v12.1.0.42
	MC_MASKED_IO_MASTER_AXIS							=   0x00090016, //v12.2.0.0
	MC_INVALID_AXIS_STATE_EC_ENABLED					=   0x00090020,	//v12.2.0.0	
	MC_INVALID_AXIS_CONFIG								=	0x000A0000,
	MC_INVALID_CONFIG_AXIS_TYPE							=   0x000A0002, //v12.2.0.0
	MC_INVALID_AXIS_CONFIG_POS_LIMIT_SWITCH				=	0x000A0006, //v12.1.0.42
	MC_INVALID_AXIS_CONFIG_NEG_LIMIT_SWITCH				=	0x000A0009, //v12.1.0.42
	MC_INVALID_AXIS_CONFIG_HOME_SWITCH					=	0x000A000B, //v12.1.0.42	
	MC_INVALID_AXIS_CONFIG_Z_PHASE_INPUT				=	0x000A000D, //v12.1.0.42
	MC_INVALID_AXIS_CONFIG_HOME_SENSOR					=	0x000A0010,
	MC_INVALID_AXIS_CONFIG_MARK_PULSE					=	0x000A0012,
	MC_AXIS_CONFIG_JOYSTICK								=   0x000A001F,	//v12.2.0.0	
	MC_INVALID_INPUT_SHAPING_FREQUENCY					=	0x000A0020, //v12.2.0.0
	MC_INVALID_INPUT_SHAPING_DAMPIN_RATIO				=	0x000A0021, //v12.2.0.0
	MC_UNSUPPORTED_INPUT_SHAPING_MODE_IN_THIS_API	    =	0x000A0022, //v12.2.0.0
	MC_UNSUPPORTED_AXIS_TYPE_IN_THIS_API				=	0x000A0023, //v12.2.0.0
	MC_INVALID_AXIS_CONFIG_HOME_TYPE					=	0x000A0032, //v12.1.0.42
	MC_INVALID_AXIS_CONFIG_HOME_FLAG_HANDLE				=	0x000A003A, //v12.1.0.42
	MC_INVALID_AXIS_CONFIG_HOMING_MODE					=	0x000A0064,	
	MC_GEARING_RULE_VIOLATION                           =	0x000B0000,	
	MC_LIMIT_POSITION_OVER								=	0x000C0000,	
	MC_POS_HW_LIMIT_POSITION_OVER						=	0x000C0001,	//v12.1.0.42
	MC_NEG_HW_LIMIT_POSITION_OVER						=	0x000C0002,	//v12.1.0.42
	MC_POS_SW_LIMIT_POSITION_OVER						=	0x000C0004, //v12.1.0.42
	MC_NEG_SW_LIMIT_POSITION_OVER						=	0x000C0008, //v12.1.0.42
	MC_INVALID_AXES_GROUP_NUM							=	0x000D0000,
	MC_AXIS_ALREADY_ASSIGNED							=	0x000E0002, //v12.1.0.42
	MC_IDENT_ALREADY_ASSIGNED							=	0x000E0004, //v12.1.0.42
	MC_AXES_GROUP_INVALID_STATE							=	0x000F0000,	
	MC_GROUP_INVALID_STATE_MOVING						=	0x000F0002,	//v12.1.0.42
	MC_GROUP_INVALID_STATE_HOMING						=	0x000F0003,	//v12.1.0.42
	MC_GROUP_INVALID_STATE_STOPPING						=	0x000F0004,	//v12.1.0.42
	MC_GROUP_INVALID_STATE_ERRORSTOP					=	0x000F0005,	//v12.1.0.42
	MC_AXIS_IN_SINGLE_MOTION_STATE						=	0x00100000,	
	MC_1ST_AXIS_IN_MOTION_STATE							=	0x00100001,	//v12.1.0.42
	MC_2ND_AXIS_IN_MOTION_STATE							=	0x00100002,	//v12.1.0.42
	MC_3RD_AXIS_IN_MOTION_STATE							=	0x00100003,	//v12.1.0.42
	MC_4TH_AXIS_IN_MOTION_STATE							=	0x00100004,	//v12.1.0.42
	MC_5TH_AXIS_IN_MOTION_STATE							=	0x00100005,	//v12.1.0.42
	MC_6TH_AXIS_IN_MOTION_STATE							=	0x00100006,	//v12.1.0.42
	MC_7TH_AXIS_IN_MOTION_STATE							=	0x00100007,	//v12.1.0.42
	MC_8TH_AXIS_IN_MOTION_STATE							=	0x00100008,	//v12.1.0.42
	MC_GROUP_MEMBER_EMPTY								=	0x00110000,	
	MC_GROUP_MEMBER_EMPTY_1								=	0x00110001, //v12.2.0.1
	MC_GROUP_MEMBER_EMPTY_2								=	0x00110002, //v12.2.0.1
	MC_GROUP_MEMBER_EMPTY_3								=	0x00110003, //v12.2.0.1
	MC_GROUP_MEMBER_EMPTY_4								=	0x00110004,	//v12.2.0.1
	MC_GROUP_MEMBER_EMPTY_5								=	0x00110005, //v12.2.0.1
	MC_GROUP_MEMBER_EMPTY_6								=	0x00110006, //v12.2.0.1
	MC_GROUP_MEMBER_EMPTY_7								=	0x00110007, //v12.2.0.1
	MC_1ST_AXIS_IN_GROUP_LIMIT_OVER 					=	0x00120000, //v12.1.0.42
	MC_2ND_AXIS_IN_GROUP_LIMIT_OVER						=	0x00120001,	//v12.1.0.42
	MC_3RD_AXIS_IN_GROUP_LIMIT_OVER						=	0x00120002,	//v12.1.0.42
	MC_4TH_AXIS_IN_GROUP_LIMIT_OVER						=	0x00120003,	//v12.1.0.42
	MC_5TH_AXIS_IN_GROUP_LIMIT_OVER						=	0x00120004,	//v12.1.0.42
	MC_6TH_AXIS_IN_GROUP_LIMIT_OVER						=	0x00120005,	//v12.1.0.42
	MC_7TH_AXIS_IN_GROUP_LIMIT_OVER						=	0x00120006,	//v12.1.0.42
	MC_8TH_AXIS_IN_GROUP_LIMIT_OVER 					=	0x00120007,	//v12.1.0.42
	MC_GROUP_CMD_SIZE_ERROR							    =	0x00130000,	
	MC_GROUP_CMD_PARAMETER_SIZE_ERROR					=	0x00130003,	//v12.1.0.42
	MC_GROUP_MEMBER_NOT_ALLOCATED_X						=	0x00140000,	//v12.1.0.42
	MC_GROUP_MEMBER_NOT_ALLOCATED_Y						=	0x00140001,	//v12.1.0.42
	MC_GROUP_NOT_ALLOCATED_ROTATION_AXIS				=	0x00140003, //v12.2.0.0
	MC_GROUP_NOT_ALLOCATED_TILT_AXIS					=   0x00140004, //v12.2.0.0
	MC_AXIS_IN_GROUP_MOTION								=	0x00150000,	
	MC_GANTRY_ASSIGNED_NOT								=	0x001A0000, //v12.2.0.0
	MC_GANTRY_SLAVE_AXIS								= 	0x00170000, //v12.2.0.0
	MC_MASKED_IO_SLAVE_AXIS								=	0x00170016, //v12.2.0.0
	MC_GANTRY_ASSIGNED_ALREADY_AXIS						= 	0x00180000, //v12.2.0.0
	MC_ALREADY_ENABLED_GANTRY_ID						=   0x00180047, //v12.2.0.0
	MC_CHECK_LASTINDEX_ON_EC_DATA_FILE					=   0x001B0000, //v12.2.0.0	
	MC_CHECK_EC_DATA_FILE_FOCUS_ON_1ST_COMMAND_POSITION =   0x001C0001, //v12.2.0.0
	MC_CHECK_EC_DATA_FILE_FOCUS_ON_2ND_COMMAND_POSITION =   0x001C0002, //v12.2.0.0
	MC_CHECK_EC_DATA_FILE_FOCUS_ON_3RD_COMMAND_POSITION =   0x001C0003, //v12.2.0.0
	MC_CHECK_EC_DATA_FILE_FOCUS_ON_1ST_ACTUAL_POSITION  =   0x001D0001, //v12.2.0.0
	MC_CHECK_EC_DATA_FILE_FOCUS_ON_2ND_ACTUAL_POSITION  =   0x001D0002, //v12.2.0.0
	MC_CHECK_EC_DATA_FILE_FOCUS_ON_3RD_ACTUAL_POSITION  =   0x001D0003, //v12.2.0.0
	MC_CAN_NOT_CHANGE_EC_MODE_BY_INVALID_AMP_STATE		=   0x001E0019, //v12.2.0.0
	MC_ALREADY_EC_MODE_ENABLE							=   0x001E0020, //v12.2.0.0
	MC_ALREADY_EC_MODE_DISABLE							=   0x001E0021, //v12.2.0.0
	MC_EC_DIMENSION_MISMATCH							=   0x001E0022, //v12.2.0.0
	MC_EC_TABLE_EMPTIED									=   0x001E0023, //v12.2.0.0	
	MC_EC_TABLE_INDEX_MISMATCH							=   0x001E0024, //v12.2.0.0
	MC_IO_INVALID_STATE_EMPTY_MASKED_IO					=	0x00200000, //v12.1.0.66
	MC_IO_INVALID_STATE_ALREADY_MASKED_IO				=	0x0020000E, //v12.1.0.66
	MC_IO_INVALID_STATE_REQUEST_UNMASK_IO				=	0x00200016, //v12.1.0.66
	MC_NOT_ENOUGH_EC_MEMORY								=   0x00240000, //v12.2.0.0
	MC_ALREADY_DISABLED_GANTRY_ID						=   0x00250047, //v12.2.0.0
	MC_INVALID_GANTRY_STATE_NOT_COMPLETE_MOTION			=   0x00260000, //v12.2.0.0
	MC_GANTRY_IN_HOMING_SEQUENCE						=   0x00270000, //v12.2.0.0
	MC_INVALID_GANTRY_STATE_DO_NOT_CHANGE_RATIO			=	0x00280001, //v12.2.0.1
	MC_INVALID_GANTRY_STATE_EC_ENABLED					=   0x00280020, //v12.2.0.0
	MC_INVALID_GANTRY_STATE_ENABLED_TORQUE_LIMIT		=   0x00280026, //v12.2.0.10
	MC_INVALID_GANTRY_STATE_ALIGN_SW_LIMIT				=   0x00280027, //v12.2.0.10
	MC_INVALID_AXIS_ID_ENTER_GANTRY_SALVE_AXIS_ID		=   0x00290000, //v12.2.0.10
	MC_INVALID_PARAMETER_ATTRIBUTE						=   0x002A0000, //v12.2.0.10
	MC_GROUP_EC_MODE_ENABLED							=	0x11010020, //v12.2.0.0
	MC_GANTRY_EC_MODE_ENABLED							=	0x12000020, //v12.2.0.0

	//Motion Library Error Defines
	MC_FAIL												=	0xE00C0001,
	MC_ERROR											=	0xE00C0002,
	MC_IOMAPING_ERR										=	0xE00C0003,
	MC_COMMINIT_ERR										=	0xE00C0004,
	MC_COMM_EVENT_INIT_ERR								=	0xE00C0005,
	MC_READ_ENI_NODE_ERR								=	0xE00C0006,
	MC_INVALID_AXIS_ERR									=	0xE00C0007,
	MC_INVALID_BOARD_ERR								=	0xE00C0008,
	MC_XML_PARSING_ERR									=	0xE00C0009,
	MC_XML_ITEM_COUNT_MISMATCH							=	0xE00C000A,
	MC_NO_BOARD_INSTALLED								=	0xE00C000B,
	MC_INVALID_DOWNLOAD_FILE_TYPE						=	0xE00C000C,
	MC_OPEN_ENI_ERR										=	0xE00C000D,
	MC_FILE_OPEN_FAIL									=	0xE00C000E,
	MC_NO_MATCHING_DOWNLOADINFORMATION					=	0xE00C000F,
	MC_NONE_OP											=	0xE00C0010,
	MC_FAIL_GEN_DOWNLOAD_FILE							=	0xE00C0011,
	MC_REG_KEY_READ_FAIL								=	0xE00C0012,
	MC_NOT_ALLOWED_IN_THIS_MASTER_MODE					=	0xE00C0014,
	MC_MASTERID_OUT_OF_RANGE							=	0xE00C0015,			
	MC_BOARDNO_OUT_OF_RANGE								=	0xE00C0016,
	MC_AXISNO_OUT_OF_RANGE								=	0xE00C0017,
	MC_BOARDCNT_OUT_OF_RANGE							=	0xE00C0018,
	MC_RETURN_SIZE_NOT_EQUAL							=	0xE00C001A,
	MC_MASTERID_DUPLICATION_ERR							=	0xE00C001B,
	MC_PARAM_ERROR_FILE_IS_NULL							=	0xE00C001C,
	MC_NO_MATCHING_BOARDID_FOUND						=	0xE00C001D,
	MC_NOT_READY_NETWORK_CONFIGURATION                  =   0xE00C001E,
	MC_INVALID_MASTERID_ERR								=	0xE00C001F,
	MC_MASTER_MODE_CHANGE_NOT_ALLOWED					=	0xE00C0020,
	MC_MASTER_REQUEST_PARAM_ERROR   					=	0xE00C0021,
	MC_MASTER_INVALID_STATE         					=	0xE00C0022,  
	MC_NOT_MOTION_LIBRAY_INITIALIZED                 	=	0xE00C0023, //2014.08.22
	MC_IOMANAGER_NOT_RUNNING		                 	=	0xE00C0024, //2014.08.22
	MC_ANOTHER_PROGRAM_IS_USING_NMC_LIBRARY             =	0xE00C0025, //2014.10.02
	MC_SLAVE_ITEM_MISMATCH		                        =	0xE00C0026, 
	MC_SLAVE_ITEM_COUNT_MISMATCH				        =	0xE00C0027, 
	MC_AXIS_COUNT_OUT_OF_RANGE							=   0xE00C0028, //v12.1.0.46
	MC_SLAVE_DEVICEID_OUT_OF_RANGE						=   0xE00C0029, //v12.1.0.64
	MC_FM_TRIG_COUNT_OUT_OF_RANGE						=	0xE00C002A, //v12.1.0.64
	MC_FM_IDXNO_OUT_OF_RANGE							=	0xE00C002B, //v12.1.0.64
	MC_MODULE_INFO_FILE_OPEN_FAIL						=	0xE00C002C, //v12.1.0.64
	MC_ID_IDX_TYPE_OUT_OF_RANGE							=   0xE00C002D, //v12.1.0.66
	MC_WAIT_TIME_OUT_OF_RANGE							=   0xE00C0030, //v12.1.0.67	
	MC_SELF_HOLD_IO_NOT_FOUND							=   0xE00C0031, //v12.1.0.67
	MC_INVALID_SLAVE_SAFEOP_STATE						=   0xE00C0032, //v12.1.0.67	
	MC_AXIS_TYPE_OUT_OF_RANGE							=   0xE00C0040, //v12.2.0.0
	MC_EC_TYPE_OUT_OF_RANGE								=   0xE00C0041, //v12.2.0.0
	MC_EC_MODE_OUT_OF_RANGE								=   0xE00C0042, //v12.2.0.0
	MC_EC_MAPID_OUT_OF_RANGE							=   0xE00C0043, //v12.2.0.0
	MC_EC_LAST_INDEX_OUT_OF_RANGE						=   0xE00C0044, //v12.2.0.0
	MC_BOARD_CATALOG_MISMATCH_ERROR						=	0xE00C0045, //v12.2.0.10

	//Comm Library Error Defines
	MC_PCICIP_GEN_10    								=	0xCC100000,
	COMM_CONNECTION_ESTABLISHED                     	=	0xED000001,
	COMM_CONN_CONFIG_FAILED_INVALID_NETWORK_PATH    	=	0xED000002,
	COMM_CONN_CONFIG_FAILED_NO_RESPONSE             	=	0xED000003,
	COMM_CONN_CONFIG_FAILED_ERROR_RESPONSE          	=	0xED000004,
	COMM_CONNECTION_TIMED_OUT                       	=	0xED000005,
	COMM_CONNECTION_CLOSED                          	=	0xED000006,
	COMM_INCOMING_CONNECTION_RUN_IDLE_FLAG_CHANGED  	=	0xED000007,
	COMM_ASSEMBLY_NEW_INSTANCE_DATA                 	=	0xED000008,
	COMM_ASSEMBLY_NEW_MEMBER_DATA                   	=	0xED000009,
	COMM_CONNECTION_NEW_INPUT_SCANNER_DATA          	=	0xED00000A,
	COMM_CONNECTION_VERIFICATION						=	0xED00000B,
	COMM_CONNECTION_RECONFIGURED                    	=	0xED00000C,
	COMM_REQUEST_RESPONSE_RECEIVED                  	=	0xED000064,
	COMM_REQUEST_FAILED_INVALID_NETWORK_PATH        	=	0xED000065,
	COMM_REQUEST_TIMED_OUT                          	=	0xED000066,
	COMM_CLIENT_OBJECT_REQUEST_RECEIVED             	=	0xED000067,
	COMM_NEW_CLASS3_RESPONSE                        	=	0xED000068,
	COMM_CLIENT_PCCC_REQUEST_RECEIVED               	=	0xED000069,
	COMM_NEW_LIST_IDENTITY_RESPONSE						=	0xED00006A,
	COMM_ID_RESET										=	0xED00006B,
	COMM_BACKPLANE_REQUEST_RECEIVED						=	0xED00006C,
	COMM_OUT_OF_MEMORY                              	=	0xED0000C8,
	COMM_UNABLE_INTIALIZE_WINSOCK                   	=	0xED0000C9,
	COMM_UNABLE_START_THREAD                        	=	0xED0000CA,
	COMM_ERROR_USING_WINSOCK                        	=	0xED0000CB,
	COMM_ERROR_SETTING_SOCKET_TO_NONBLOCKING        	=	0xED0000CC,
	COMM_ERROR_SETTING_TIMER                        	=	0xED0000CD,
	COMM_SESSION_COUNT_LIMIT_REACHED                	=	0xED0000CE,
	COMM_CONNECTION_COUNT_LIMIT_REACHED             	=	0xED0000CF,
	COMM_PENDING_REQUESTS_LIMIT_REACHED             	=	0xED0000D0,
	COMM_PENDING_REQUEST_GROUPS_LIMIT_REACHED       	=	0xED0000D1,
	COMM_ERROR_UNABLE_START_MODBUS						=	0xED0000D2,
	COMM_ERROR_HW_NOT_INSTALLED							=	0xED0000DC,
	COMM_ERROR_DD_SEND_ERROR							=	0xED0000DD,
	COMM_ERROR_DD_READ_ERROR							=	0xED0000DE,
	COMM_DD_ERROR_SEND									=	0xED0000DF,
	COMM_DD_ERROR_RECV									=	0xED0000E0,
	COMM_DD_OPEN_FAIL									=	0xED0000E6,
	COMM_DD_NOT_OPENED									=	0xED0000E7,
	COMM_DD_CONN_FAIL									=	0xED0000E8,
	COMM_DD_CLIENT_START_FAIL							=	0xED0000E9,
	COMM_DD_OK											=	0xED000000,
	COMM_CN_NOT_CONNECTED								=	0xED0000F0,
	COMM_CN_CONNECTED									=	0xED0000F1,
	COMM_CN_CONNERROR									=	0xED0000F2,
	//
	COMM_ERROR_SUCCESS									=	0xEE000000,
	COMM_ERROR_FAILURE									=	0xEE010000,
	COMM_EXT_ERR_DUPLICATE_FWD_OPEN						=	0xEE010100,
	COMM_EXT_ERR_CLASS_TRIGGER_INVALID					=	0xEE010103,
	COMM_EXT_ERR_OWNERSHIP_CONFLICT						=	0xEE010106,
	COMM_EXT_ERR_CONNECTION_NOT_FOUND					=	0xEE010107,
	COMM_EXT_ERR_INVALID_CONN_TYPE						=	0xEE010108,
	COMM_EXT_ERR_INVALID_CONN_SIZE						=	0xEE010109,
	COMM_EXT_ERR_DEVICE_NOT_CONFIGURED					=	0xEE010110,
	COMM_EXT_ERR_RPI_NOT_SUPPORTED						=	0xEE010111,
	COMM_EXT_ERR_CONNECTION_LIMIT_REACHED				=	0xEE010113,
	COMM_EXT_ERR_VENDOR_PRODUCT_CODE_MISMATCH			=	0xEE010114,
	COMM_EXT_ERR_PRODUCT_TYPE_MISMATCH					=	0xEE010115,
	COMM_EXT_ERR_REVISION_MISMATCH						=	0xEE010116,
	COMM_EXT_ERR_INVALID_CONN_POINT						=	0xEE010117,
	COMM_EXT_ERR_INVALID_CONFIG_FORMAT					=	0xEE010118,
	COMM_EXT_ERR_NO_CONTROLLING_CONNECTION				=	0xEE010119,
	COMM_EXT_ERR_TARGET_CONN_LIMIT_REACHED				=	0xEE01011A,
	COMM_EXT_ERR_RPI_SMALLER_THAN_INHIBIT				=	0xEE01011B,
	COMM_EXT_ERR_CONNECTION_TIMED_OUT					=	0xEE010203,
	COMM_EXT_ERR_UNCONNECTED_SEND_TIMED_OUT				=	0xEE010204,
	COMM_EXT_ERR_PARAMETER_ERROR						=	0xEE010205,
	COMM_EXT_ERR_MESSAGE_TOO_LARGE						=	0xEE010206,
	COMM_EXT_ERR_UNCONN_ACK_WITHOUT_REPLY				=	0xEE010207,
	COMM_EXT_ERR_NO_BUFFER_MEMORY_AVAILABLE				=	0xEE010301,
	COMM_EXT_ERR_BANDWIDTH_NOT_AVAILABLE				=	0xEE010302,
	COMM_EXT_ERR_TAG_FILTERS_NOT_AVAILABLE				=	0xEE010303,
	COMM_EXT_ERR_REAL_TIME_DATA_NOT_CONFIG				=	0xEE010304,
	COMM_EXT_ERR_PORT_NOT_AVAILABLE						=	0xEE010311,
	COMM_EXT_ERR_LINK_ADDR_NOT_AVAILABLE				=	0xEE010312,
	COMM_EXT_ERR_INVALID_SEGMENT_TYPE_VALUE				=	0xEE010315,
	COMM_EXT_ERR_PATH_CONNECTION_MISMATCH				=	0xEE010316,
	COMM_EXT_ERR_INVALID_NETWORK_SEGMENT				=	0xEE010317,
	COMM_EXT_ERR_INVALID_LINK_ADDRESS					=	0xEE010318,
	COMM_EXT_ERR_SECOND_RESOURCES_NOT_AVAILABLE			=	0xEE010319,
	COMM_EXT_ERR_CONNECTION_ALREADY_ESTABLISHED			=	0xEE01031A,
	COMM_EXT_ERR_DIRECT_CONN_ALREADY_ESTABLISHED		=	0xEE01031B,
	COMM_EXT_ERR_MISC									=	0xEE01031C,
	COMM_EXT_ERR_REDUNDANT_CONNECTION_MISMATCH			=	0xEE01031D,
	COMM_EXT_ERR_NO_MORE_CONSUMER_RESOURCES				=	0xEE01031E,
	COMM_EXT_ERR_NO_TARGET_PATH_RESOURCES				=	0xEE01031F,
	COMM_EXT_ERR_VENDOR_SPECIFIC						=	0xEE010320,
	COMM_ERROR_NO_RESOURCE								=	0xEE020000,
	COMM_ERROR_INVALID_PARAMETER_VALUE					=	0xEE030000,
	COMM_ERROR_INVALID_SEG_TYPE							=	0xEE040000,
	COMM_ERROR_INVALID_DESTINATION						=	0xEE050000,
	COMM_ERROR_PARTIAL_DATA								=	0xEE060000,
	COMM_ERROR_CONN_LOST								=	0xEE070000,
	COMM_ERROR_BAD_SERVICE								=	0xEE080000,
	COMM_ERROR_BAD_ATTR_DATA							=	0xEE090000,
	COMM_ERROR_ATTR_LIST_ERROR							=	0xEE0A0000,
	COMM_ERROR_ALREADY_IN_REQUESTED_MODE				=	0xEE0B0000,
	COMM_ERROR_OBJECT_STATE_CONFLICT					=	0xEE0C0000,
	COMM_ERROR_OBJ_ALREADY_EXISTS						=	0xEE0D0000,
	COMM_ERROR_ATTR_NOT_SETTABLE						=	0xEE0E0000,
	COMM_ERROR_PERMISSION_DENIED						=	0xEE0F0000,
	COMM_ERROR_DEV_IN_WRONG_STATE						=	0xEE100000,
	COMM_ERROR_REPLY_DATA_TOO_LARGE						=	0xEE110000,
	COMM_ERROR_FRAGMENT_PRIMITIVE						=	0xEE120000,
	COMM_ERROR_NOT_ENOUGH_DATA							=	0xEE130000,
	COMM_ERROR_ATTR_NOT_SUPPORTED						=	0xEE140000,
	COMM_ERROR_TOO_MUCH_DATA							=	0xEE150000,
	COMM_ERROR_OBJ_DOES_NOT_EXIST						=	0xEE160000,
	COMM_ERROR_NO_FRAGMENTATION							=	0xEE170000,
	COMM_ERROR_DATA_NOT_SAVED							=	0xEE180000,
	COMM_ERROR_DATA_WRITE_FAILURE						=	0xEE190000,
	COMM_ERROR_REQUEST_TOO_LARGE						=	0xEE1A0000,
	COMM_ERROR_RESPONSE_TOO_LARGE						=	0xEE1B0000,
	COMM_ERROR_MISSING_LIST_DATA						=	0xEE1C0000,
	COMM_ERROR_INVALID_LIST_STATUS						=	0xEE1D0000,
	COMM_ERROR_SERVICE_ERROR							=	0xEE1E0000,
	COMM_ERROR_VENDOR_SPECIFIC							=	0xEE1F0000,
	COMM_ERROR_INVALID_PARAMETER						=	0xEE200000,
	COMM_ERROR_WRITE_ONCE_FAILURE						=	0xEE210000,
	COMM_ERROR_INVALID_REPLY							=	0xEE220000,
	COMM_ERROR_BAD_KEY_IN_PATH							=	0xEE250000,
	COMM_ERROR_BAD_PATH_SIZE							=	0xEE260000,
	COMM_ERROR_UNEXPECTED_ATTR							=	0xEE270000,
	COMM_ERROR_INVALID_MEMBER							=	0xEE280000,
	COMM_ERROR_MEMBER_NOT_SETTABLE						=	0xEE290000,
	COMM_ERROR_UNKNOWN_MODBUS_ERROR						=	0xEE2B0000,
	COMM_ERROR_HW_NOT_INSTALLED1						=	0xEE2C0000,
	COMM_ERROR_ENCAP_PROTOCOL							=	0xEE6A0000,
	COMM_ERROR_STILL_PROCESSING							=	0xEEFF0000,
	MC_DOWNLOAD_FAIL_DUE_TO_ANOTHER_PROGRAM_IS_RUNNING	=	0xEE800000, //Ver_0c010012_1
}MC_STATUS;

#define MAX_EEPROM_SIZE			0xFFFF //16bit max (64k)
#define ECAT_MAX_NAME			0xFF   //255
#define MAX_BOARD_CNT			4
#define MAX_AXIS_CNT			64
#define MAX_AXIS_ID				65535  //Ver_0c010012_6
#define MAX_NODE_CNT			256
#define MAX_PHYSICAL_ADDR		65535  //Ver_0c010012_3
#define MAX_MASTER_ID			10
#define MAX_ERR_LEN				128
#define MAX_PROFILE_ITEM_COUNT	6
#define MAX_AXES_IN_GROUP		8	   //3->8 //v12.1.0.45
#define MAX_AXES_GROUP_CNT		32
#define MAX_AXES_STATUS_SIZE	8
#define MAX_ALL_STATUS_SIZE		1536   //Ver_0c010019_1	: 계산식 = (MAX_AXIS_CNT + MAX_AXES_GROUP_CNT) 

#define MAX_LOGICAL_AXIS_COUNT	 64	   //v12.1.0.41
#define MAX_LOGICAL_DEVICE_COUNT 1024  //v12.1.0.41
#define MAX_DEVICE_NAME_SIZE	 256   //v12.1.0.64

#define INVALID_BOARD		0xFFFF
#define INVALID_AXIS		0xFFFF
#define INVALID_MASTERID	0xFFFF
#define INVALID_FLASH		0xFFFFFFFF

//Axis Parameter Definition
#define COMMANDED_POSITION 			1		
#define SW_LIMIT_POS  				2		
#define SW_LIMIT_NEG  				3		
#define ENABLE_LIMIT_POS  			4		
#define ENABLE_LIMIT_NEG  			5		
#define ENABLE_POS_LAG_MONITORING	6		
#define MAX_POSITION_LAG 			7		
#define MAX_VELOCITY_SYSTEM 		8			
#define MAX_VELOCITY_APPL  			9			
#define ACTUAL_VELOCITY  			10		
#define COMMANDED_VELOCITY 			11		
#define MAX_ACCELERATION_SYSTEM		12			
#define MAX_ACCELERATION_APPL		13		
#define MAX_DECELERATION_SYSTEM		14			
#define MAX_DECELERATION_APPL		15		
#define MAX_JERK 					16		
#define ACTUAL_POSITION				1001
#define PROFILE_DACCEL				1100	

#define IOMAP_HEADER_SIZE 28 //DSP 0.13 이상에서 변경됨  2014.08.26 (32 - 4)
#define IOPAGE_SIZE 4096

//=============================================================================
//-----------------------------------------------------------------------------
//
//
// Enum Types
//
//
//-----------------------------------------------------------------------------
// EtherCat Master
typedef enum EcatState_tag
{
	eST_UNKNOWN	  = 0,
	eST_INIT	  = 0x01, //Ver_0c010016_2
	eST_PREOP	  = 0x02, //Ver_0c010016_2
	eST_BOOTSTRAP = 0x03, //v12.1.0.48
	eST_SAFEOP	  = 0x04, //Ver_0c010016_2
	eST_OP		  = 0x08, //Ver_0c010016_2
	eST_ACKERR    = 0x10  //Ver_0c010016_2
}EcState;

typedef enum EcatMasterMode_tag
{
	eMM_IDLE = 0,
	eMM_SCAN,
	eMM_RUN,
	eMM_INTRANSITION,
	eMM_ERR,
	eMM_LINKBROKEN,
	eMM_FREE_RUN, //v12.1.0.54
}EcMstMode;

typedef enum EcatScanMode_tag
{
	SCAN_ALL = 0,
	SCAN_SINGLE ,
}EcScanMode;

typedef enum EcatScanStatus_tag
{
	SCAN_NONE = 0,
	SCAN_BUSY ,
	SCAN_DONE
}EcScanSts;

//IO
typedef enum IOBufferMode_Tag
{
	BUF_OUT= 0,
	BUF_IN = 1,
}IOBufMode;

/*
Bit0	ErrorStop	Error Stop State
Bit1	Disabled	Disabled State
Bit2	Stopping	Stopping State
Bit3	StandStill	Stand Still State
Bit4	DiscreteMotion	Discrete Motion State
Bit5	ContinuousMotion	Continuous Motion State
Bit6	SynchronizedMotion	Synchronized Motion State
Bit7	Homing	Homing State
Bit8-9	Reserved	Reserved for Future Use
Bit10	ConstantVelocity	Constant Velocity Moving
Bit11	Accelerating	Accelerating Moving
Bit12	Decelerating	Decelerating Moving
Bit13	DirectionPositive	Position is increasing
Bit14	DirectionNegative	Position is decreasing
Bit15-16	Reserved	Reserved for Future Use.
Bit17	HomeAbsSwitch	Home Abs. Switch Input Status
Bit18	LimitSwitchPos	Positive Limit Switch Input Status
Bit19	LimitSwitchNeg	Negative Limit Switch Input Status
Bit20	Reserved	Reserved for Future Use
Bit21	Reserved	Reserved for Future Use
Bit22	ReadyForPowerOn	Ready for Power on State
Bit23	PowerOn	Power on State
Bit24	IsHomed	Homing Complete State
Bit25	AxisWarning	Axis Warning State
Bit26	MotionComplete	Motion Complete State
Bit27	Gearing	Gearing State
Bit28	GroupMotion	Group Motion State
Bit29	BufferFull	Motion Buffer Full State
Bit30-Bit31	Reserved	Reserved for Future Use
*/

//Motion
//MC_ReadAxisInfo
typedef enum AxisStatus
{
	mcErrorStop					=	0x00000001,
	mcDisabled					=   0x00000002,
	mcStopping					=   0x00000004,
	mcStandStill				=	0x00000008,
	mcDiscreteMotion			=	0x00000010,
	mcContinuousMotion			=	0x00000020,
	mcSynchroMotion				=	0x00000040,
	mcHoming					=	0x00000080,
	mcReserved_as_8				= 	0x00000100,
	mcReserved_as_9				= 	0x00000200,
	mcConstantVelocity			=	0x00000400,
	mcAccelerating				=	0x00000800,
	mcDecelerating				=	0x00001000,
	mcDirectionPositive			=	0x00002000,
	mcDirectionNegative			=	0x00004000,
	mcLimitSwitchNeg			= 	0x00008000,
	mcLimitSwitchPos			= 	0x00010000,
	mcHomeAbsSwitch				= 	0x00020000,
	mcLimitSwitchPosEvent		= 	0x00040000,
	mcLimitSwitchNegEvent		= 	0x00080000,
	mcDriveFault				= 	0x00100000,
	mcSensorStop			    = 	0x00200000,
	mcReadyForPowerOn			= 	0x00400000,
	mcPowerOn					= 	0x00800000,
	mcIsHomed					= 	0x01000000,
	mcAxisWarning				= 	0x02000000,
	mcMotionComplete			= 	0x04000000,
	mcGearing					= 	0x08000000,
	mcGroupMotion				= 	0x10000000,
	mcBufferFull				= 	0x20000000,
	mcReserved_as_30			= 	0x40000000,
	mcReserved_as_31			= 	0x80000000,
}MC_AXISSTATUS;

//MC_ReadStatus
typedef enum StatusTag
{
	mcASErrorStop				=	0x00000001,
	mcASDisabled				=   0x00000002,
	mcASStopping				=   0x00000004,
	mcASStandStill				=	0x00000008,
	mcASDiscreteMotion			=	0x00000010,
	mcASContinuousMotion		=	0x00000020,
	mcASSynchroMotion			=	0x00000040,
	mcASHoming					=	0x00000080,
}MC_Status;

//MC_ReadMotionState
typedef enum MotionStateTag
{
	mcMSConstantVelocity		= 0x00000001,
	mcMSAccelerating			= 0x00000002,
	mcMSDecelerating			= 0x00000004,
	mcMSDirectionPositive		= 0x00000008,
	mcMSDirectionNegative		= 0x00000010,
}MC_MOTIONSTATE;

typedef enum AxisInfoTag
{
	mcAIHomeAbsSwitch			= 0x00000001,
	mcAILimitSwitchPos			= 0x00000002,
	mcAILimitSwitchNeg			= 0x00000004,
	mcAIReserved3				= 0x00000008,
	mcAIReserved4				= 0x00000010,
	mcAIReadyForPowerOn			= 0x00000020,
	mcAIPowerOn					= 0x00000040,
	mcAIIsHomed					= 0x00000080,
	mcAIAxisWarining			= 0x00000100,
	mcAIMotionComplete			= 0x00000200,
	mcAIGearing					= 0x00000400,
	mcAIGroupMotion				= 0x00000800,
	mcAIBufferFull				= 0x00001000,
	mcAIReseved13				= 0x00002000,
}MC_AXISINFO;

typedef enum AxisError
{
	mcAxis_NO_ERROR							= 0x0000,
	mcAxis_DEVICE_ERROR						= 0x0001,
	mcAxis_INVALID_AXIS_STATE				= 0x0002,
	mcAxis_PARAMETER_INVALID				= 0x0003,
	mcAxis_UNSUPPORT_CMD_REQUEST			= 0x0004,
	mcAxis_CMD_REQUEST_FORMAT_WRONG			= 0x0005,
	mcAxis_RESOURCE_ERROR					= 0x0006,
	mcAxis_CONFIG_INVALID					= 0x0007,
	mcAxis_POSITION_FOLLOWING_ERROR			= 0x0008,
	mcAxis_VELOCITY_FOLLOWING_ERROR			= 0x0009,
	mcAxis_SYSTEM_MAX_VELOCITY_OVER_ERROR	= 0x000A,
	mcAxis_SYSTEM_MAX_ACCEL_OVER_ERROR		= 0x000B,
	mcAxis_SYSTEM_MAX_DECEL_OVER_ERROR		= 0x000C,
	mcAxis_SYSYEM_MAX_JERK_OVER_ERROR		= 0x000D,
	mcAxis_MALFUNCTION_ERROR				= 0x000E,
	mcAxis_GEARING_RULE_VIOLATION			= 0x000F,
	mcAxis_HW_LIMIT_REACHED_WARNING			= 0x8001,
	mcAxis_SW_LIMIT_REACHED_WARNING			= 0x8002,
}MC_AXISERROR;

typedef struct AxisErrorInfoTag
{
	UINT16 ErrorId;
	UINT16 ErrorInfo0;
	UINT16 ErrorInfo1;
}MC_AxisErrorInfo;

typedef struct AxesGroupRawDataStatusTag
{
	UINT8  Mode;				// Raw Data Mode
	UINT8  Enabled;				// Enabled Flag
	UINT32 EmptyBufferCount;	// Empty Buffer Count
	UINT16 InBufferIndex;		// In Buffer Index
	UINT16 OutBufferIndex;		// Out Buffer Index
	UINT32 CurrentRawDataID;	// Current Out RawDataSet ID
	UINT8  CoordSystem;			// Reserved
}MC_AxesGroupRawDataStatus;

typedef enum MC_SOURCE_tag
{
	mcSetValue = 0, //Synchronization on master set value
	mcActualValue,  //Synchronization on master actual value
	mcSetValueFixedGear = 0x10,
	mcActualValueFixedGear = 0x11,
}MC_SOURCE;

typedef enum MC_EXECUTION_MODE_tag
{
	mcImmediately = 0, 
	mcQueued,  
}MC_EXECUTION_MODE;

//Parameter Number
typedef enum mcParamID_tag
{
	mcpCommandedPosition          		= 1,
	mcpSWLimitPos_  					= 2,
	mcpSWLimitNeg_  					= 3,
	mcpEnableLimitPos_  				= 4,
	mcpEnableLimitNeg_  				= 5,
	mcpEnablePosLagMonitoring  			= 6,
	mcpMaxPositionLag_  				= 7,
	mcpMaxVelocitySystem_  				= 8,
	mcpMaxVelocityAppl_  				= 9,
	mcpActualVelocity  					= 10,
	mcpCommandedVelocity  				= 11,
	mcpMaxAccelerationSystem_  			= 12,
	mcpMaxAccelerationAppl  			= 13,
	mcpMaxDecelerationSystem_  			= 14,
	mcpMaxDecelerationAppl  			= 15,
	mcpMaxJerkSystem  					= 16,
	mcpMaxJerkAppl_  					= 17,
	mcpActualPosition					= 1000,
	mcpCommandedAccel					= 1001,
	mcpActualAccel						= 1002,
	mcpCommandedJerk					= 1003,
	mcpActualJerk						= 1004,
	mcpTotalBufferCount					= 1010,
	mcpAvailableBufferCount				= 1011,
	mcpECCommandPosition				= 1501, //v12.2.0.0
	mcpECActualPosition					= 1502, //v12.2.0.0
	mcpAxisType							= 2002,
	mcpModuloAxis						= 2003,
	mcpModuloValue						= 2004,
	mcpEnableHWLimitPos					= 2010,
	mcpHWLimitPosInputNum				= 2011,
	mcpHWLimitPosActLevel				= 2012,
	mcpEnableHWLimitNeg					= 2013,
	mcpHWLimitNegInputNum				= 2014,
	mcpHWLimitNegActLevel				= 2015,
	mcpHomeInputNum						= 2016,
	mcpHomeActLevel						= 2017,
	mcpMarkerInputNum					= 2018,
	mcpMarkerActLevel					= 2019,
	mcpInputActLevel					= 2020,
	mcpEnableLimitPos					= 2030,
	mcpSWLimitPos						= 2031,
	mcpEnableLimitNeg					= 2032,
	mcpSWLimitNeg						= 2033,
	mcpMaxVelocityAppl					= 2034,
	mcpMaxAccelAppl						= 2035,
	mcpMaxDecelAppl						= 2036,
	mcpMaxJerkAppl						= 2037,
	mcpMaxVelocitySystem				= 2038,
	mcpMaxAccelerationSystem			= 2039,
	mcpMaxDecelerationSystem			= 2040,
	mcpmcpMaxJerkSystem					= 2041,
	mcpmcpEStopType						= 2060,
	mcpEStopDecel						= 2061,
	mcpEStopJerk						= 2062,
	mcpInvertCmdDir						= 2070,
	mcpCmdScaleFactor					= 2071,
	mcpFeedbackMode						= 2072,
	mcpInvertFeedbackDir				= 2073,
	mcpFeedbackScaleFactor				= 2074,
	mcpPositionFeedbackFilter			= 2075,
	mcpVelocityFeedbackFilter			= 2076,
	mcpAccelerationFeedbackFilter	    = 2077,
	mcpStartVelocityOffset				= 2078,
	mcpStopVelocityOffset				= 2079,
	mcpInPositionCheckType				= 2080,
	mcpInPositionWindowSize				= 2081,
	mcpInVelocityWindowSize				= 2082,
	mcpEnablePositionLagMonit			= 2083,
	mcpMaxPositionLag					= 2084,
	mcpPositionLagCalMethod				= 2085,
	mcpEnableVelocityLagMonit			= 2086,
	mcpMaxVelocityLag					= 2087,
	mcpVelocityLagCalMethod				= 2088,
	mcpHomingType						= 2100,
	mcpHomingDir						= 2101,
	mcpHomingVelocity					= 2102,
	mcpHomingAcceleration				= 2103,
	mcpHomingDeceleration				= 2104,
	mcpHomingJerk						= 2105,
	mcpHomingCreepVelocity				= 2106,
	mcpHomePositionOffset				= 2107,
	mcpHomeCompleteFlagHandle			= 2108,
	//v12.1.0.33
	mcpSensor0StopEnable 				= 2109,
	mcpSensor0StopMode	 				= 2110,
	mcpSensor0StopIOOffset 				= 2111,
	mcpSensor0StopIOSize 				= 2112,
	mcpSensor0StopIOBit 				= 2113,
	mcpSensor0StopPosOffset 			= 2114,
	mcpSensor1StopEnable 				= 2115,
	mcpSensor1StopMode	 				= 2116,
	mcpSensor1StopIOOffset 				= 2117,
	mcpSensor1StopIOSize 				= 2118,
	mcpSensor1StopIOBit 				= 2119,
	mcpSensor1StopPosOffset 			= 2120,
	mcpSensor2StopEnable 				= 2121,
	mcpSensor2StopMode	 				= 2122,
	mcpSensor2StopIOOffset 				= 2123,
	mcpSensor2StopIOSize 				= 2124,
	mcpSensor2StopIOBit 				= 2125,
	mcpSensor2StopPosOffset 			= 2126,
	mcpSensor3StopEnable 				= 2127,
	mcpSensor3StopMode	 				= 2128,
	mcpSensor3StopIOOffset 				= 2129,
	mcpSensor3StopIOSize 				= 2130,
	mcpSensor3StopIOBit 				= 2131,
	mcpSensor3StopPosOffset 			= 2132,
	mcpSensor4StopEnable 				= 2133,
	mcpSensor4StopMode	 				= 2134,
	mcpSensor4StopIOOffset 				= 2135,
	mcpSensor4StopIOSize 				= 2136,
	mcpSensor4StopIOBit 				= 2137,
	mcpSensor4StopPosOffset 			= 2138,
	//v12.1.0.40
	mcpPositiveLimitErrorStop			= 2139,
	mcpNegativeLimitErrorStop 			= 2140,
	//
	mcpSensor0MovingVelocity			= 2141,
	mcpSensor1MovingVelocity			= 2142,
	mcpSensor2MovingVelocity			= 2143,
	mcpSensor3MovingVelocity			= 2144,
	mcpSensor4MovingVelocity			= 2145,
	//v12.1.0.67
	mcpPositionOffset					= 2146,
	mcpPositionOffsetValue				= 2147,
	mcpMaintainGearStateEnable			= 2148,
	mcpClampingVelocity					= 2149,
	//v12.2.0.2
	mcpMaxTorqueMode					= 2150,
}mcParamID;			

typedef enum BufferMode_tag
{
	//0:Aborting, 1:Buffered, 2:BlendingLow, 3:BlendingPrevious, 4:BlendingNext, 5:BlendingHigh
	//6:BufferedBlendingLow, 7:BufferedBlendingPrevious, 8:BufferedBlendingNext, 9:BufferedBlendingHigh
	mcAborting = 0,
	mcBuffered,
	mcBlendingLow,
	mcBlendingPrevious,
	mcBlendingNext,
	mcBlendingHigh,
	mcBufferedBlendingLow,
	mcBufferedBlendingPrevious,
	mcBufferedBlendingNext,
	mcBufferedBlendingHigh,
}MC_BUFFER_MODE;

typedef enum MCDirection_tag
{
	//0:Positive Direction, 1:Shortest_way, 2:Negative_Direction, 3:Current_Direction
	mcPositiveDirection = 0,
	mcShortestWay,
	mcNegativeDirection,
	mcCurrentDirection,
}MC_DIRECTION;

typedef enum MC_GEAR_MONITOR_tag
{
	mcGearActive	 = 0x0001,
	mcGearIn		 = 0x0002,
	mcGearReserved2  = 0x0004,
	mcGearReserved3  = 0x0008,
	mcGearReserved4  = 0x0010,
	mcGearReserved5  = 0x0020,
	mcGearReserved6  = 0x0040,
	mcGearReserved7  = 0x0080,
	mcGearReserved8  = 0x0100,
	mcGearReserved9  = 0x0200,
	mcGearReserved10 = 0x0400,
	mcGearReserved11 = 0x0800,
	mcGearReserved12 = 0x1000,
	mcGearReserved13 = 0x2000,
	mcGearReserved14 = 0x4000,
	mcGearReserved15 = 0x8000,
}MC_GearStatus;

//MC_AXIS_OBJ_INST_ATTR_ERROR_LIMIT_STOP_TYPE	21
typedef enum ErrorLimitstopType_tag
{
	IMMEDIATE = 0,
	DECEL
}AxisStopType;

//MC_AXIS_OBJ_INST_ATTR_HOMING_DIRECTION		22
//Do not use!!! (Old Version)
typedef enum Direction_tag
{
	NMC_CW = 0,
	NMC_CCW
}MC_AXIS_DIRECTION;

//MC_AXIS_OBJ_INST_ATTR_HOMING_DIRECTION		22 
//New Version
typedef enum Home_Direction_tag
{
	HOMING_DIR_CCW = 0,
	HOMING_DIR_CW
}MC_HOME_AXIS_DIRECTION;

typedef enum Home_Type_tag { HOMING_REF_PULSE = 0, HOMING_ABS_SWITCH = 1, HOMING_LIMIT_SWITCH = 2, HOMING_DIRECT = 3 ,HOMMING_NMC_CONFIG=4 } MC_HOME_AXIS_MODE;
typedef enum Home_Flag_tag { HOMING_CLEAR = 0, HOMING_NOT_CLEAR = 1, HOMING_DRIVE_CLEAR = 2 } MC_HOME_DONE_FLAG;

//MC_AXIS_OBJ_INST_ATTR_CONTROL_TYPE			23
typedef enum ControlType_tag
{
	OL_PULSE_DIRECTION= 0,
	OL_TWO_PULSE,
	OL_QUDARATURE_PULSE,
	CL_VELOCITY_LEVEL_ANALOG,
	CL_TORQUE_LEVEL_ANALOG,
}MC_AXIS_CONTROL;

//MC_AXIS_OBJ_INST_ATTR_POSITION_I_MODE			34
typedef enum IMode_tag
{
	//IN_STANDING= 0,
	STANDING = 0,
	ALWAYS,
}MC_AXIS_I_MODE;

//Flash Download Binary File Type
typedef enum BinFileType
{
	BIN_BOOT	= 1,
	BIN_A8OS	= 2,
	BIN_TM		= 3,
	BIN_DSPOS	= 5,
	BIN_ENI		= 6
};

typedef enum SWVerType
{
	SW_VER_MOTION = 0,
	SW_VER_PCICIP,
	SW_VER_DDSDK,
	SW_VER_DD,
};

typedef enum MC_CoordSystem_tag
{
	mcACS = 1,
	mcMCS,
	mcPCS,
}MC_COORDSYSTEM;

typedef enum MC_Transition_mode_tag
{
	mcTMNone = 0,			//Insert no transition curve (default mode)
	mcTMStartVelocity,		//Transition with given start velocity
	mcTMConstantVelocity,	//Transition with given constant velocity
	mcTMCornerDistance,		//Transition with given corner distance
	mcTMMaxCornerDeviation, //Transition with given maximum corner deviation
	//5 - 9 Reserved by PLCopen
	//10 - Supplier specific modes
	mcTMProfileContinue = 11, //RSA Transiton Mode (Special) //v12.1.0.71
}MC_TRANSITION_MODE;

typedef enum CircMode_tag
{
	mcBORDER = 0,
	mcCENTER,
	mcRADIUS,
	mcCENTER_ANGLE = 10,
	mcBORDER_ANGLE = 11,
	mcSPIRAL   = 20, //v12.1.0.71
	mcCYLINDER = 21, //v12.1.0.71  
	mcSPHERE   = 22, //v12.1.0.71
}MC_CIRC_MODE;

typedef enum CircPathChoice_tag
{
	mcClockWise = 0,
	mcCounterClockWise,
}MC_CIRC_PATHCHOICE;

typedef enum RawDataMode_tag
{
	mcPositionCmdMode = 0,
	mcVelocityCmdMode,
	mcTorqueCmdMode,
}MC_RAW_DATA_MODE;

typedef enum SaveMode_tag
{
	mcSMAuto = 0,
	mcSMIndex,
}MC_SAVE_MODE;

//v12.2.0.12
typedef enum StatusCombination_Tag
{
	STATUS_COMBINATION_1 = 1,	//Init.
	STATUS_COMBINATION_2 = 2,	//PreOP
	STATUS_COMBINATION_3 = 3,	//ProOP + Init or Bootstrap
	STATUS_COMBINATION_4 = 4,	//SafeOP
	STATUS_COMBINATION_5 = 5,	//SafeOP + Init
	STATUS_COMBINATION_6 = 6,	//SafeOP + PreOP
	STATUS_COMBINATION_7 = 7,	//SafeOP + PreOP + Init or SafeOP + Bootstrap
	STATUS_COMBINATION_8 = 8,	//OP
	STATUS_COMBINATION_9 = 9,	//OP + Init
	STATUS_COMBINATION_10 = 10,	//OP + PreOP
	STATUS_COMBINATION_11 = 11,	//OP + PreOP + Init or OP + Bootstrap
	STATUS_COMBINATION_12 = 12,	//OP + SafeOP
	STATUS_COMBINATION_13 = 13,	//OP + SafeOP + Init
	STATUS_COMBINATION_14 = 14,	//OP + SafeOP + PreOP
	STATUS_COMBINATION_15 = 15,	//OP + SafeOP + PreOP + Init or OP + SafeOP + Bootstrap
}STATUS_COMBINATION;

typedef enum GroupStatus_tag
{
	GroupMoving		 = 0x00000001,
	GroupHoming		 = 0x00000002,
	GroupErrorStop	 = 0x00000004,
	GroupStandby	 = 0x00000008,
	GroupStopping	 = 0x00000010,
	GroupDisabled	 = 0x00000020,
	ConstantVelocity = 0x00000040,
	Accelerating	 = 0x00000080,
	Decelerating     = 0x00000100,
	InPosition		 = 0x00000200,
}MC_GroupStatus;

//v12.2.0.1
typedef enum GantryStatus_tag
{
	mcGantry_MotionCompleted	= 0x00000001, //Bit 00
	mcGantry_Fault				= 0x00000002, //Bit 01
	mcGantry_Warning			= 0x00000004, //Bit 02
	mcGantry_FollowingError		= 0x00000008, //Bit 03
	mcGantry_YawError		    = 0x00000010, //Bit 04
	mcGantry_YawStable		    = 0x00000020, //Bit 05
	mcGantry_AmpOff				= 0x00000040, //Bit 06
	mcGantry_Moving				= 0x00000080, //Bit 07
	mcGantry_Buffering			= 0x00000100, //Bit 08
	mcGantry_IsHomed			= 0x00000200, //Bit 09
	mcGantry_Homing				= 0x00000400, //Bit 10
	mcGantry_PosHWLimitMaster   = 0x00000800, //Bit 11
	mcGantry_NegHWLimitMaster   = 0x00001000, //Bit 12
	mcGantry_PosSWLimitMaster   = 0x00002000, //Bit 13
	mcGantry_NegSWLimitMaster   = 0x00004000, //Bit 14
	mcGantry_PosHWLimitSlave	= 0x00008000, //Bit 15
	mcGantry_NegHWLimitSlave	= 0x00010000, //Bit 16
	mcGantry_PosSWLimitSlave	= 0x00020000, //Bit 17
	mcGantry_NegSWLimitSlave	= 0x00040000, //Bit 18
	mcGantry_HWLimitEventMaster = 0x00080000, //Bit 19
	mcGantry_HWLimitEventSlave  = 0x00100000, //Bit 20
	mcGantry_InputShapingZV	    = 0x00200000, //Bit 21
	mcGantry_InputShapingZVD	= 0x00400000, //Bit 22
	mcGantry_Reserved_23		= 0x00800000, //Bit 23
	mcGantry_Reserved_24		= 0x01000000, //Bit 24
	mcGantry_Reserved_25		= 0x02000000, //Bit 25
	mcGantry_Reserved_26		= 0x04000000, //Bit 26
	mcGantry_Reserved_27		= 0x08000000, //Bit 27
	mcGantry_Reserved_28		= 0x10000000, //Bit 28
	mcGantry_Reserved_29		= 0x20000000, //Bit 29
	mcGantry_Reserved_30		= 0x40000000, //Bit 30
	mcGantry_Reserved_31		= 0x80000000, //Bit 31
}MC_GantryStatus;

//Callback
#ifndef MAX_PATH
#define MAX_PATH          260
#endif

//File download callback function prototype.
typedef struct flashStatus_tag
{
	UINT32 Status;
	UINT32 Step;
	UINT32 Progress;
	char Message[MAX_PATH];
}FlashStatus;
typedef void (*fnCallbackDownload) (FlashStatus *); 

//=============================================================================
//-----------------------------------------------------------------------------
//
//
// System APIs - Initialization, General & Utility Functions
//
//
//-----------------------------------------------------------------------------

//=============================================================================
// MC_STATUS MC_STATUS MC_Init(void)
// Desc :: 설치된 모든 보드를 초기화 한다.
// Param ::
//     IN : 
// return ::
//     No Error :
//         MC_OK
//     Error :
//         
//         
//=============================================================================
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_Init(void);
//=============================================================================
// MC_STATUS MC_MasterInit(UINT16 MasterID)
// Desc :: BoardID 를 입력받아서 Master 를 초기화 한다.
// Param ::
//     IN : UINT16 MasterID     : 초기화 하고자 하는 Master 의 ID 
// return ::
//     No Error :
//         MC_OK
//     Error :
//         
//         
//=============================================================================
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_MasterInit(UINT16 MasterID);
//=============================================================================
// MC_STATUS MC_MasterRUN(UINT16 MasterID)
// Desc :: BoardID 를 입력받아서 Master의 EtherCAT 동작 상태를 RUN 상태로 만든다.
// Param ::
//     IN : UINT16 MasterID     : Master 의 ID 
// return ::
//     No Error :
//         MC_OK
//     Error :
//         MC_NOT_READY_NETWORK_CONFIGURATION
//         
//=============================================================================
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_MasterRUN(UINT16 MasterID);
//=============================================================================
// MC_STATUS MC_MasterSTOP(UINT16 MasterID)
// Desc :: BoardID 를 입력받아서 Master의 EtherCAT 동작 상태를 STOP 상태로 만든다.
// Param ::
//     IN : UINT16 MasterID     : Master 의 ID 
// return ::
//     No Error :
//         MC_OK
//     Error :
//         
//         
//=============================================================================
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_MasterSTOP(UINT16 MasterID);
//=============================================================================
// MC_STATUS MC_GetSWVersion(UINT16 Type, UINT16 * Major, UINT16 *Minor)
// Desc :: Version 정보를 얻는다.
// Param ::
//     OUT : UINT16 * MasterCount : 설치된 보드 갯수
// return ::
//     No Error :
//         MC_OK
//     Error :
//         
//=============================================================================
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_GetSWVersion(UINT16 Type, UINT16 * Major, UINT16 *Minor);
//=============================================================================
// MC_STATUS MC_GetErrorMessage(UINT32 ErrorCode, UINT32 Size, char * ErrorMessage)
// Desc :: ErrorCode 에 대한 문자열을 얻는다.
// Param ::
//     IN  : UINT32 ErrorCode; : Error Code
//     IN  : UINT32 Size ; Size should be MAX_ERR_LEN;
//     OUT : char * ErrorMessage; : 상응하는 문자열
// return ::
//     No Error :
//         MC_OK
//     Error :
//         
//=============================================================================
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_GetErrorMessage(UINT32 ErrorCode, UINT32 Size, char * ErrorMessage);

//=============================================================================
// MC_STATUS MC_GetMismatchErrorInfo(UINT16 BoardID, UINT16 *MisMatchSlaveCount, UINT16 MismatchSlaveID[MAX_AXIS_CNT]) 
// Desc :: MC_SLAVE_ITEM_MISMATCH Error SlaveCount와 SlaveID Array을 얻는다.
// Param ::
//     IN  : UINT16 BoardID
//	   OUT : UINT16 MisMatchSlaveCount
//     OUT : UINT16 MismatchSlaveID[MAX_AXIS_CNT] : Mismatch SlaveID Array
// return ::
//     No Error :
//         MC_OK
//     Error : 
//		   MC_INVALID_BOARD_ERR or	MC_BOARDNO_OUT_OF_RANGE
//         
//=============================================================================
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_GetMismatchErrorInfo(UINT16 BoardID, 
													UINT16 * MismatchSlaveCount, 
													UINT16 MismatchSlaveID[MAX_AXIS_CNT]);

//-----------------------------------------------------------------------------
// BoardID : BoardSwitchID
// nFileType : Binary File Type :
// sNvsFileName : FullPath File Name : c:\\test\\NMC.nvs
// sBinFileName : FullPath File Name : c:\\test\\xxx.bin or c:\\test\\xxx.xml ...
//
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS	FileDownloadCB(
									UINT16 BoardID, 
									LPVOID lpCallbackFunc, // Callback Function
									int nFileType, 
									char *sNvsFileName, 
									char *sBinFileName
									);
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS FileMasterFWDownloadCB(
									UINT16 BoardID, 
									LPVOID lpCallbackFunc,
									int nFileType, 
									char *sNvsFileName, 
									char * sBinFileName
									);

//-----------------------------------------------------------------------------
// BoardID : BoardSwitchID
// nFileType : Binary File Type :
// sNvsFileName : FullPath File Name : c:\\test\\NMC.nvs
// sBinFileName : FullPath File Name : c:\\test\\xxx.bin or c:\\test\\xxx.xml ...
//
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS FileDownload(
									UINT16 BoardID, 
									int nFileType, 
									char * sNvsFileName, 
									char * sBinFileName, 
									int nMsgSize, 
									char * szMsg
									);

//=============================================================================
// MC_STATUS MC_GetMasterMap(UINT16 MasterMap[MAX_BOARD_CNT], UINT16 * MasterCount)
// Desc :: 현재 설치된 모든 보드의 ID List 를 돌려준다.
// Param ::
//     OUT : UINT16 MasterMap[MAX_BOARD_CNT] : 설치된 보드 ID List
//     OUT : UINT16 * MasterCount : 설치된 보드 갯수
// return ::
//     No Error :
//         MC_OK
//     Error :
//         MC_NO_BOARD_INSTALLED
//=============================================================================
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_GetMasterMap(
									UINT16 MasterMap[MAX_BOARD_CNT], 
									UINT16 * MasterCount
									);
//=============================================================================
// MC_STATUS MC_GetMasterCount(UINT16 * MasterCount)
// Desc :: 현재 설치된 보드의 갯수를 리턴한다.
// Param ::
//     OUT : UINT16 * MasterCount : 설치된 보드 갯수
// return ::
//     No Error :
//         MC_OK
//     Error :
//         MC_NO_BOARD_INSTALLED
//=============================================================================
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_GetMasterCount(
									UINT16 * MasterCount
									);
//=============================================================================
// MC_STATUS  MC_GetBoardScanNo(UINT16 BoardID, UINT16 * BoardScanNo)
// Desc :: BoardID 에 해당하는 Master 의 Scan Number 를 돌려준다.
// Param ::
//     IN : UINT16 BoardID
//     OUT : UINT16 * BoardScanNo
// return ::
//     No Error :
//         MC_OK
//     Error :
//         MC_MASTERID_OUT_OF_RANGE
//=============================================================================
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_GetBoardScanNo(
									UINT16 BoardID, 
									UINT16 * BoardScanNo
									);

#ifdef __cplusplus
extern "C"
#endif
	NMCMOTIONAPI MC_STATUS	MC_SetSystemPerformance(
									UINT16 Level
									);

//v12.1.0.55
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS	MasterGetNodeFailureID(
									UINT16	BoardID,       //_in Board ID
									UINT16 * NodeFailureID //_out Node FailureID
									);

//v12.1.0.58
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS	MC_RestoreSWEmergency(
									UINT16 BoardID,			//_in BoardID
									UINT8 * RestoreStatus   //_out Restore Status
									);

//v12.1.0.67
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS	MC_SetIOSelfHoldFunc(
									UINT16 BoardID,		//_in BoardID
									UINT16 WaitTime		//_in Wait Time Before Slave SafeOP Check (Range: 0 ~ 100000 ms)
									);
//v12.1.0.68
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS	MC_EniFileDownload(
									UINT16 BoardID,	     //_in BoardID
									char * sNvsFileName, //_in eMMC.nvs Full Path File Name ex) c:\\test\\eMMC.nvs
									char * sEniFileName  //_in ENI.xml Full Path File Name ex) c:\\test\\ENI.xml
									);

//=============================================================================
//-----------------------------------------------------------------------------
//
//
// Master APIs
//
//
//-----------------------------------------------------------------------------

//=============================================================================
// MC_STATUS  MasterGetSDOData(		UINT16 BoardID, 
//									UINT16 SlaveNo, 
//									UINT16 SDOIndex,
//									UINT8  SubIndex,
//									UINT32  DataSize,
//									UINT32 *respDataSize,
//									UINT8 *bDataArray
//									);
// Desc :: Get SDO Data from Slave.
// Param ::
//     IN : UINT16 BoardID
//     IN : UINT16 SlaveNo : Scan No
//     IN : UINT16 SDOIndex
//     IN : UINT16 SubIndex
//     IN : UINT16 DataSize
//     OUT : UINT16 * respDataSize
//     OUT : UINT16 * bDataArray
// return ::
//     No Error :
//         MC_OK
//     Error :
//         
//=============================================================================
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MasterGetSDOData(
									UINT16 BoardID, 
									UINT16 SlaveNo, 
									UINT16 SDOIndex,
									UINT8  SubIndex,
									UINT32  DataSize,
									UINT32 *respDataSize,
									UINT8 *bDataArray
									);

//40h-1 - MasterGetSDODataEcatAddr
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MasterGetSDODataEcatAddr(
									UINT16 BoardID, 
									UINT16 EcatAddr, 
									UINT16 SDOIndex,
									UINT8  SubIndex,
									UINT32  DataSize,
									UINT32 *respDataSize,
									UINT8 *bDataArray
									);

//=============================================================================
// MC_STATUS  MasterSetSDOData(		UINT16 BoardID, 
//									UINT16 SlaveNo, 
//									UINT16 SDOIndex,
//									UINT8  SubIndex,
//									UINT32  DataSize,
//									UINT32 *respDataSize,
//									UINT8 *bDataArray
//									);
// Desc :: Set SDO Data to Slave.
// Param ::
//     IN : UINT16 BoardID
//     IN : UINT16 SlaveNo : Scan No
//     IN : UINT16 SDOIndex
//     IN : UINT16 SubIndex
//     IN : UINT16 DataSize
//     OUT : UINT16 * respDataSize
//     IN : UINT16 * bDataArray : Data Arrays to Set
// return ::
//     No Error :
//         MC_OK
//     Error :
//         
//=============================================================================
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MasterSetSDOData(
									UINT16 BoardID, 
									UINT16 SlaveNo, 
									UINT16 SDOIndex,
									UINT8  SubIndex,
									UINT32  DataSize,
									UINT32 *respDataSize,
									UINT8 *bDataArray
									);

// 41h-1 - DnloadSDOData
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MasterSetSDODataEcatAddr(
									UINT16 BoardID, 
									UINT16 EcatAddr, 
									UINT16 SDOIndex,
									UINT8  SubIndex,
									UINT32  DataSize,
									UINT32 *respDataSize,
									UINT8 *bDataArray
									);

//=============================================================================
// MC_STATUS  MasterGetCurMode(UINT16 BoardID, UINT8 * MasterMode);
// Desc :: Board 의 Master 상태를 돌려 준다.
// Param ::
//     IN : UINT16 BoardID
//     OUT : UINT8 * MasterMode  : EcMstMode 의 상태 값
// return ::
//     No Error :
//         MC_OK
//     Error :
//         MC_MASTERID_OUT_OF_RANGE
//=============================================================================
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MasterGetCurMode(UINT16 BoardID, UINT8 * MasterMode);
//=============================================================================
// MC_STATUS  MasterGetBoardID(UINT16 MasterScanNo, UINT16 * MasterID);
// Desc :: Master 의 ID 를 얻는다. (보드가 OS 에서 인식된 순서 번호를 입력하면, 해당 보드의 ID 를 돌려 준다.)
// Param ::
//     IN : UINT16 MasterScanNo : 보드 인식 번호
//     OUT : UINT16 * MasterID : 보드의 ID 값
// return ::
//     No Error :
//         MC_OK
//     Error :
//         MC_MASTERID_OUT_OF_RANGE
//=============================================================================
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MasterGetBoardID(UINT16 MasterScanNo, UINT16 * MasterID);
//=============================================================================
// MC_STATUS  MasterGetOSRevision(UINT16 BoardID, UINT8 * Major, UINT8 * Minor);
// Desc :: Retrive Arm OS Version Information of Master
// Param ::
//     IN : UINT16 BoardID
//     OUT : UINT8 * Major : Majer Version
//     OUT : UINT8 * Minor : Minor Version
// return ::
//     No Error :
//         MC_OK
//     Error :
//         MC_MASTERID_OUT_OF_RANGE
//=============================================================================
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MasterGetOSRevision(UINT16 BoardID, UINT8 * Major, UINT8 * Minor);
//=============================================================================
// MC_STATUS  MasterGetMCRevision(UINT16 BoardID, UINT8 * Major, UINT8 * Minor);
// Desc :: Retrive Motion Control Version Information of Master
// Param ::
//     IN : UINT16 BoardID
//     OUT : UINT8 * Major : Majer Version
//     OUT : UINT8 * Minor : Minor Version
// return ::
//     No Error :
//         MC_OK
//     Error :
//         MC_MASTERID_OUT_OF_RANGE
//=============================================================================
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MasterGetMCRevision(UINT16 BoardID, UINT8 * Major, UINT8 * Minor);

// MasterGetErrorState //Ver_0c010010_3
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MasterGetLastError(
									UINT16 BoardID,
									UINT32* pSequenceNo,
									UINT32* pErrorCode,
									UINT8 ExtErrorInfo[6]
									);

// MasterResetErrorState //Ver_0c010010_4
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MasterClearError(
									UINT16 BoardID
									);

// Read Error Info //Ver_0c01000D_4
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  NMCReadErrorInfo(
									UINT16 BoardID,
									UINT32* pSequenceNo,
									UINT32* pErrorCode,
									UINT8 ExtErrorInfo[6],
									UINT32* RemainErrorCount
									);

//=============================================================================
//-----------------------------------------------------------------------------
//
//
// Master APIs - Device Info.
//
//
//-----------------------------------------------------------------------------
//Master Get Axes Info.
#ifdef __cplusplus
extern "C"
#endif
	NMCMOTIONAPI MC_STATUS	MasterGetAxesCount(
									UINT16 BoardID,			//_in BoardID
									UINT16 * TotalAxisCount //_out Total Axis Count
									); 

#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS	MasterGetAxesID(
									UINT16 BoardID,		 //_in BoardID
									UINT16 * AxisIDArray //_out AxisID Array
									);

//Master Get Device Info.
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS	MasterGetDeviceCount(
									UINT16 BoardID,			  //_in BoardID
									UINT16 * TotalDeviceCount //_out Total Device Count
									);

#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS	MasterGetDeviceID(
									UINT16 BoardID,		   //_in BoardID
									UINT16 * DeviceIDArray //_out DeviceID Array (EcatAddr)
									);

//Master Get Digital Input Info.
#ifdef __cplusplus
extern "C"
#endif
	NMCMOTIONAPI MC_STATUS	MasterGet_DI_DeviceCount(
									UINT16 BoardID,		 //_in BoardID
									UINT16 * DeviceCount //_out DI Device Count
									);

#ifdef __cplusplus
extern "C"
#endif
	NMCMOTIONAPI MC_STATUS	MasterGet_DI_DeviceID(
									UINT16 BoardID,		   //_in BoardID
									UINT16 * DeviceIDArray //_out DI DeviceID Array (EcatAddr)
									);

#ifdef __cplusplus
extern "C"
#endif
	NMCMOTIONAPI MC_STATUS	MasterGet_DI_DeviceTotalChSize(	
									UINT16 BoardID,			   //_in BoardID
									UINT16 DeviceID,		   //_in DI DeviceID (EcatAddr)
									UINT16 * DeviceTotalChSize //_out DI Device Total Channel Size
									);

#ifdef __cplusplus
extern "C"
#endif
	NMCMOTIONAPI MC_STATUS	MasterGet_DI_DeviceName(	
									UINT16 BoardID,		   //_in BoardID
									UINT16 DeviceID,	   //_in DI DeviceID (EcatAddr)
									char DeviceNameArray[MAX_DEVICE_NAME_SIZE] //_out DI Device Name Array [256]
									);

//Master Get Digital Output Info.
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS	MasterGet_DO_DeviceCount(
									UINT16 BoardID,		  //_in BoardID
									UINT16 * DeviceCount  //_out DO Device Count
									);

#ifdef __cplusplus
extern "C"
#endif
	NMCMOTIONAPI MC_STATUS	MasterGet_DO_DeviceID(
									UINT16 BoardID,		   //_in BoardID
									UINT16 * DeviceIDArray //_out DO DeviceID Array (EcatAddr)
									);

#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS	MasterGet_DO_DeviceTotalChSize(	
									UINT16 BoardID,			   //_in BoardID
									UINT16 DeviceID,		   //_in DO DeviceID (EcatAddr)
									UINT16 * DeviceTotalChSize //_out DO Device Total Channel Size
									);

#ifdef __cplusplus
extern "C"
#endif
	NMCMOTIONAPI MC_STATUS	MasterGet_DO_DeviceName(	
									UINT16 BoardID,		   //_in BoardID
									UINT16 DeviceID,	   //_in DO DeviceID (EcatAddr)
									char DeviceNameArray[MAX_DEVICE_NAME_SIZE] //_out DO Device Name Array [256]
									);

//Master Get Analog Input Info.
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS	MasterGet_AI_DeviceCount(
									UINT16 BoardID,		 //_in BoardID
									UINT16 * DeviceCount //_out AI DeviceCount
									);

#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS MasterGet_AI_DeviceID(
									UINT16 BoardID,		   //_in BoardID
									UINT16 * DeviceIDArray //_out AI DeviceID Array (EcatAddr)
									);

#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS	MasterGet_AI_DeviceTotalChSize(	
									UINT16 BoardID,			   //_in BoardID
									UINT16 DeviceID,	  	   //_in AI DeviceID (EcatAddr)
									UINT16 * DeviceTotalChSize //_out AI Device Total Channel Size
									);

#ifdef __cplusplus
extern "C"
#endif
	NMCMOTIONAPI MC_STATUS	MasterGet_AI_DeviceName(	
									UINT16 BoardID,		   //_in BoardID
									UINT16 DeviceID,	   //_in AI DeviceID (EcatAddr)
									char DeviceNameArray[MAX_DEVICE_NAME_SIZE] //_out AI Device Name Array [256]
									);

//Master Get Analog Output Info.
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS	MasterGet_AO_DeviceCount(
									UINT16 BoardID,		  //_in BoardID
									UINT16 * DeviceCount  //_out AO DeviceCount
									);

#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS	MasterGet_AO_DeviceID(
									UINT16 BoardID,		   //_in BoardID									
									UINT16 * DeviceIDArray //_out AO DeviceID Array (EcatAddr)
									);

#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS	MasterGet_AO_DeviceTotalChSize(	
									UINT16 BoardID,			   //_in BoardID
									UINT16 DeviceID,		   //_in AO DeviceID (EcatAddr)
									UINT16 * DeviceTotalChSize //_out AO Device Total Channel Size
									);

#ifdef __cplusplus
extern "C"
#endif
	NMCMOTIONAPI MC_STATUS	MasterGet_AO_DeviceName(	
									UINT16 BoardID,			//_in BoardID
									UINT16 DeviceID,		//_in AO DeviceID (EcatAddr)
									char DeviceNameArray[MAX_DEVICE_NAME_SIZE] //_out AO Device Name Array [256]
									);

//=============================================================================
//-----------------------------------------------------------------------------
//
//
// Master APIs - Device I/O Info.
// v12.2.0.10
//
//-----------------------------------------------------------------------------
//Digital Input - I/O Info.
#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS	MasterGet_DI_IO_Count(
									UINT16 BoardID,			//_in BoardID
									UINT16 DeviceID,	//_in DeviceID (EcatAddr)
									UINT16 * Count		//_out Count
									);

#ifdef __cplusplus
	extern "C"
#endif
	NMCMOTIONAPI MC_STATUS	MasterGet_DI_IO_Name(
									UINT16 BoardID,					//_in BoardID
									UINT16 DeviceID,	 //_in DeviceID (EcatAddr)
									char NameArray[][MAX_DEVICE_NAME_SIZE] //_out Name Array [][256]
									);

#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS	MasterGet_DI_IO_Data(
									UINT16 BoardID,					//_in BoardID
									UINT16 DeviceID,		  //_in DeviceID (EcatAddr)
									UINT16 * BitSizeArray,	  //_out Bit Size Array 
									UINT16 * RawIOOffsetArray //_out Raw IOOffset Array
									);

//Digital Output - I/O Info.
#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS	MasterGet_DO_IO_Count(
									UINT16 BoardID,					 //_in BoardID
									UINT16 DeviceID,	//_in DeviceID (EcatAddr)
									UINT16 * Count		//_out Count
									);

#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS	MasterGet_DO_IO_Name(
									UINT16 BoardID,		 //_in BoardID
									UINT16 DeviceID,	 //_in DeviceID (EcatAddr)
									char NameArray[][MAX_DEVICE_NAME_SIZE] //_out Name Array [][256]
									);

#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS	MasterGet_DO_IO_Data(
									UINT16 BoardID,			  //_in BoardID
									UINT16 DeviceID,		  //_in DeviceID (EcatAddr)
									UINT16 * BitSizeArray,	  //_out Bit Size Array 
									UINT16 * RawIOOffsetArray //_out Raw IOOffset Array
									);

//Analog Input - I/O Info.
#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS	MasterGet_AI_IO_Count(
									UINT16 BoardID,		//_in BoardID
									UINT16 DeviceID,	//_in DeviceID (EcatAddr)
									UINT16 * Count		//_out Count
									);

#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS	MasterGet_AI_IO_Name(
									UINT16 BoardID,		 //_in BoardID
									UINT16 DeviceID,	 //_in DeviceID (EcatAddr)
									char NameArray[][MAX_DEVICE_NAME_SIZE] //_out Name Array [][256]
									);

#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS	MasterGet_AI_IO_Data(
									UINT16 BoardID,			  //_in BoardID
									UINT16 DeviceID,		  //_in DeviceID (EcatAddr)
									UINT16 * BitSizeArray,	  //_out Bit Size Array
									UINT16 * RawIOOffsetArray //_out Raw IOOffset Array
									);

//Analog Output - I/O Info.
#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS	MasterGet_AO_IO_Count(
									UINT16 BoardID,		//_in BoardID
									UINT16 DeviceID,	//_in DeviceID (EcatAddr)
									UINT16 * Count		//_out Count
									);

#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS	MasterGet_AO_IO_Name(
									UINT16 BoardID,		 //_in BoardID
									UINT16 DeviceID,	 //_in DeviceID (EcatAddr)
									char NameArray[][MAX_DEVICE_NAME_SIZE] //_out Name Array [][256]
									);

#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS	MasterGet_AO_IO_Data(
									UINT16 BoardID,					//_in BoardID
									UINT16 DeviceID,		  //_in DeviceID (EcatAddr)
									UINT16 * BitSizeArray,	  //_out Bit Size Array
									UINT16 * RawIOOffsetArray //_out Raw IOOffset Array
									);

//=============================================================================
//-----------------------------------------------------------------------------
//
//
// Slave APIs
//
//
//-----------------------------------------------------------------------------
//ObjEcatSlave.h 에서 옮겨 옴. //Ver_0c01000E_4
#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  SlaveGetAliasNo(
									UINT16 BoardID, 
									UINT16 EcatAddr, 
									UINT16 * AliasID
									);
//Ver_0c010012_8
#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  SlaveSetAliasNo(
									UINT16 BoardID, 
									UINT16 EcatAddr, 
									UINT16 AliasID
									);
//21
#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  SlaveGetCurState(
									UINT16 BoardID, 
									UINT16 EcatAddr, 
									UINT8 * data
									);


//v12.2.0.12
#ifdef __cplusplus
	extern "C"
#endif
	NMCMOTIONAPI MC_STATUS  SlaveGetCurStateAll(
									UINT16 BoardID,		  //_in BoardID
									UINT16 *DeviceCount,  //_out Device Count
									UINT16 *WorkingCount, //_out Working Count
									UINT8  *StatusCombination //_out Status Combination (All Slave)(STATUS_COMBINATION Enum)
									);

//42h - Ethercat Direct Access
#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MasterECatDirectAccess(
									UINT16 BoardID, 
									UINT8 EcatCmd, 
									UINT16 Adp,
									UINT16 Ado,
									UINT16 ReqDataSize,
									UINT8  *bReqDataArray,
									UINT16 *RespDataSize,
									UINT8  *bRespDataArray,
									UINT16 *WC
									);
//2015_01_30 Open EEPROM Read
//#ifdef __cplusplus
//	extern "C" 
//#endif
//	NMCMOTIONAPI MC_STATUS ReadET1100EEPROMWORD(
//									UINT16 BoardID, 
//									UINT16 EcatAddr, 
//									UINT32 Addr, 
//									UINT16 * wData, 
//									UINT32 TIMEOUT
//									);
////2015_01_30 Open EEPROM Write
//#ifdef __cplusplus
//	extern "C" 
//#endif
//	NMCMOTIONAPI MC_STATUS WriteET1100EEPROMWORD(
//									UINT16 BoardID, 
//									UINT16 EcatAddr, 
//									UINT32 Addr, 
//									UINT16 wData, 
//									UINT32 TIMEOUT
//									);

//=============================================================================
//-----------------------------------------------------------------------------
//
//
// Slave APIs - Slave Home
//
//
//-----------------------------------------------------------------------------
//v12.1.0.34
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS  MC_SetHomeFlag(
									UINT16 BoardID, //BoardID
									UINT16 AxisID,  //Axis number
									UINT16 EcatAddr //Ethercat Address
									);

//v12.1.0.34
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS  MC_GetHomeFlag(
									UINT16 BoardID,   //BoardID
									UINT16 AxisID,    //Axis number
									UINT32 *pHomeFlag //Home Flag
									);

//v12.1.0.35
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS  MC_ModeChange(
									UINT16 BoardID,     //BoardID
									UINT16 AxisID,      //Axis number
									UINT8  Mode,  	    //Mode(6:hm,8:csp)
									UINT8  ReservedZero //MC_BUFFER_MODE BufferMode (Reserved)
									);

//v12.1.0.35
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS  MC_SlaveHomeSet(
									UINT16 BoardID,      //BoardID
									UINT16 EcatAddr,     //Ethercat Address
									INT32  Offset,		 //Home Offset 
									INT8   Method,       //Method
									UINT32 SpeedSwitch,  //Speed during search for switch
									UINT32 SpeedZero,    //Speed during search for zero
									UINT32 Acceleration, //Acceleration
									UINT8  ReservedZero  //MC_BUFFER_MODE BufferMode (Reserved)
									);

//v12.1.0.35
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS  MC_SlaveHome( 
									UINT16 BoardID,     //BoardID
									UINT16 AxisID,      //Axis number
									UINT8  Start,       //0:Stop,1:Start
									UINT8  ReservedZero //MC_BUFFER_MODE BufferMode (Reserved)
									);

//v12.1.0.35
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS  MC_SlaveHomeHalt(
									UINT16 BoardID,     //BoardID
									UINT16 AxisID,      //Axis number
									UINT8  ReservedZero //MC_BUFFER_MODE BufferMode (Reserved)
									);

//v12.1.0.35
#ifdef __cplusplus
	extern "C"
#endif
		NMCMOTIONAPI MC_STATUS  MC_ReadSlaveHomeStatus(
									UINT16 BoardID,    //BoardID
									UINT16 AxisID,     //Axis number
									UINT32 *HomeStatus //Response Data(Bit0:Homing Complete, Bit1:Homing Error)
									);

//v12.1.0.35
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS  MC_ReadSlaveModeStatus(
									UINT16 BoardID,   //BoardID
									UINT16 AxisID,    //Axis number
									UINT8 *ModeStatus //Response Data(6:hm,8:csp)
									);

//=============================================================================
//-----------------------------------------------------------------------------
//
//
// I/O APIs
//
//
//-----------------------------------------------------------------------------
//MC_GetIOState
#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS MC_GetIOState(
									UINT16 BoardID,
									UINT32 type,
									UINT32 * State
									);

//MC_IO_WRITE 
#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_IO_RAW_WRITE(
									UINT16 BoardID,
									UINT32 Offset,
									UINT32 Size,
									UINT8 * DataArray
									);
//MC_IO_WRITE 
#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_IO_WRITE(
									UINT16 BoardID,
									UINT16 EcatAddr,
									UINT32 Offset,
									UINT32 Size,
									UINT8 * DataArray
									);
//MC_IO_WRITE_BIT
#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_IO_WRITE_BIT(
									UINT16 BoardID,
									UINT16 EcatAddr,
									UINT32 Offset,
									UINT8 bitOffset,
									bool  data
									);
//MC_IO_WRITE_BIT
#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_IO_RAW_WRITE_BIT(
									UINT16 BoardID,
									UINT32 Offset,
									UINT8 bitOffset,
									bool  data
									);
//MC_IO_WRITE_BYTE
#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_IO_WRITE_BYTE(
									UINT16 BoardID,
									UINT16 EcatAddr,
									UINT32 Offset,
									BYTE  data
									);
//MC_IO_WRITE_WORD
#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_IO_WRITE_WORD(
									UINT16 BoardID,
									UINT16 EcatAddr,
									UINT32 Offset,
									WORD  data
									);
//MC_IO_WRITE_DWORD
#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_IO_WRITE_DWORD(
									UINT16 BoardID,
									UINT16 EcatAddr,
									UINT32 Offset,
									DWORD  data
									);

//MC_IO_READ
#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_IO_READ(
									UINT16 BoardID,
									UINT16 EcatAddr,
									UINT16 BufferInOut,
									UINT32 Offset,
									UINT32 Size,
									UINT8 * DataArray
									);
//MC_IO_READ
#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_IO_RAW_READ(
									UINT16 BoardID,
									UINT16 BufferInOut,
									UINT32 Offset,
									UINT32 Size,
									UINT8 * DataArray
									);
//MC_IO_READ_BIT
#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_IO_READ_BIT(
									UINT16 BoardID,
									UINT16 EcatAddr,
									UINT16 BufferInOut,
									UINT32 Offset,
									UINT8 BitOffset,
									bool * data
									);
//MC_IO_READ_BIT
#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_IO_RAW_READ_BIT(
									UINT16 BoardID,
									UINT16 BufferInOut,
									UINT32 Offset,
									UINT8 BitOffset,
									bool * data
									);
//MC_IO_READ_BYTE
#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_IO_READ_BYTE(
									UINT16 BoardID,
									UINT16 EcatAddr,
									UINT16 BufferInOut,
									UINT32 Offset,
									BYTE * data
									);
//MC_IO_READ_WORD
#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_IO_READ_WORD(
									UINT16 BoardID,
									UINT16 EcatAddr,
									UINT16 BufferInOut,
									UINT32 Offset,
									WORD * data
									);
//MC_IO_READ_DWORD
#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_IO_READ_DWORD(
									UINT16 BoardID,
									UINT16 EcatAddr,
									UINT16 BufferInOut,
									UINT32 Offset,
									DWORD * data
									);

//=============================================================================
//-----------------------------------------------------------------------------
//
//
// I/O APIs - Buffering I/O Control
//
//
//-----------------------------------------------------------------------------
//v12.1.0.66 
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS  MC_IO_MaskDigitalOutputBit(
									UINT16 BoardID,			//_in BoardID
									UINT16 AxisID,			//_in AxisID
									UINT8  IDIndexType,		//_in ID Index Type (AxisID:1, GroupID:2, GantryID:3)
									UINT16 EcatAddr,		//_in EcatAddr
									UINT16 SlaveOffset,		//_in Slave Offset				
									UINT8  OutputBitNumber,	//_in Mask Output Bit Number (Max Range: 0~31)
									bool   OutputValue		//_in Mask Output Bit Value (등록시 Output 초기 출력값 설정) (Set:1, Clear:0)
									);

//v12.1.0.66 
#ifdef __cplusplus
	extern "C"
#endif
		NMCMOTIONAPI MC_STATUS  MC_IO_UnMaskDigitalOutputBit(
									UINT16 BoardID,			//_in BoardID
									UINT16 EcatAddr,		//_in EcatAddr
									UINT16 SlaveOffset,		//_in Slave Offset
									UINT8  OutputBitNumber	//_in UnMask Output Bit Number (Max Range: 0~31)
									);

//v12.1.0.66
#ifdef __cplusplus
	extern "C"
#endif
		NMCMOTIONAPI MC_STATUS  MC_IO_GetMaskedDigitalOutputBit(
									UINT16 BoardID,			//_in BoardID
									UINT16 EcatAddr,		//_in EcatAddr
									UINT16 SlaveOffset,		//_in Slave Offset								
									UINT8  OutputBitNumber,	//_in Output Bit Number (Max Range: 0~31)
									UINT16 * AxisID,		//_out Masked AxisID
									UINT8  * IDIndexType	//_out Masked ID Index Type (AxisID:1, GroupID:2, GantryID:3)
									);

//v12.1.0.66
#ifdef __cplusplus
	extern "C"
#endif
		NMCMOTIONAPI MC_STATUS  MC_IO_GetMaskedDigitalOutput(
									UINT16 BoardID,				//_in BoardID
									UINT8 * MaskCount,			//_out Masked Count	
									UINT16 EcatAddr[9],			//_out Masked EcatAddr
									UINT16 SlaveOffset[9],		//_out Masked Slave Offset
									UINT8  OutputBitNumber[9],	//_out Masked Output Bit Number
									UINT16 AxisID[9],			//_out Masked AxisID
									UINT8  IDIndexType[9]		//_out Masked ID Index Type (AxisID:1, GroupID:2, GantryID:3)
									);

//v12.1.0.66
#ifdef __cplusplus
	extern "C"
#endif
		NMCMOTIONAPI MC_STATUS  MC_IO_WriteDigitalOutputBit(
									UINT16 BoardID,			//_in BoardID
									UINT16 EcatAddr,		//_in EcatAddr
									UINT16 SlaveOffset,		//_in Slave Offset
									UINT8  OutputBitNumber,	//_in Output Bit Number (Max Range: Bit 0~31)
									bool   Value			//_in Output Bit Value (Set:1, Clear:0)
									);

//=============================================================================
//-----------------------------------------------------------------------------
//
//
// Motion APIs - Single Axis Motion
//
//
//-----------------------------------------------------------------------------
//5.1.1.1
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_Power(
									UINT16 BoardID, //BoardID
									UINT16 AxisID,  //Axis number
									bool Enable	    //Axis enable : 0:Disable, 1:Enable
									);
//5.1.1.2
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_MoveAbsolute(
									UINT16 BoardID,  //BoardID
									UINT16 AxisID,   //Axis number
									DOUBLE Position, //Target Position
									DOUBLE Velocity, //Max Velocity
									DOUBLE Accel,	 //Max Acceleration
									DOUBLE Decel,    //Max Deceleration
									DOUBLE Jerk,     //Max Jerk
									MC_DIRECTION Dir,//0:Positive Direction, 1:Shortest_way, 2:Negative_Direction, 3:Current_Direction
									MC_BUFFER_MODE BufferMode //Buffer Mode : 0:Aborting, 1:Buffered, 2:BlendingLow, 3:BlendingPrevious, 4:BlendingNext, 5:BlendingHigh
									);
 //5.1.1.3
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_MoveRelative(
									UINT16 BoardID,  //BoardID
									UINT16 AxisID,   //Axis number
									DOUBLE Distance, //Distance
									DOUBLE Velocity, //Max Velocity
									DOUBLE Accel,    //Max Acceleration
									DOUBLE Decel,    //Max Deceleration
									DOUBLE Jerk,     //Max Jerk
									MC_BUFFER_MODE BufferMode //Buffer Mode : 0:Aborting, 1:Buffered, 2:BlendingLow, 3:BlendingPrevious, 4:BlendingNext, 5:BlendingHigh
									);
 //5.1.1.6
#ifdef __cplusplus
extern "C"
#endif
	NMCMOTIONAPI MC_STATUS  MC_MoveVelocity(
									UINT16 BoardID,  //BoardID
									UINT16 AxisID,   //Axis number
									DOUBLE Velocity, //Max Velocity
									DOUBLE Accel,    //Max Acceleration
									DOUBLE Decel,    //Max Deceleration
									DOUBLE Jerk,     //Max Jerk
									MC_DIRECTION Dir,//1: Positive_Direction, 3:Negative_Direction, 4:Current_Direction
									MC_BUFFER_MODE BufferMode //Buffer Mode : 0:Aborting, 1:Buffered
									);
//5.1.1.7
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_Home(
									UINT16 BoardID, //BoardID
									UINT16 AxisID,  //Axis number
									DOUBLE Position,// 
									MC_BUFFER_MODE BufferMode 
									);
//5.1.1.8
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_Stop(
									UINT16 BoardID, //BoardID
									UINT16 AxisID,  //Axis number
									bool  Execute,
									DOUBLE Decel,   //Max Deceleration
									DOUBLE Jerk     //Max Jerk
									);
//5.1.1.9 MCReadStatus
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_ReadStatus(
									UINT16 BoardID,  //BoardID
									UINT16 AxisID,   //Axis number
									UINT32 * pStatus //MC_Status
									);
 //5.1.1.10
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_ReadAxisError(
									UINT16 BoardID, //BoardID
									UINT16 AxisID,  //Axis number
									UINT16 * pErrorID,
									UINT16 * pErrorInfo,
									UINT16 * pErrorInfoExt
									);
//5.1.1.11
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_Reset(
									UINT16 BoardID, //BoardID
									UINT16 AxisID   //Axis number
									);

//5.1.1.12 MCReadParameter
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_ReadParameter(
									UINT16 BoardID, //BoardID
									UINT16 AxisID,  //Axis number,
									UINT32 ParameterNum, //PN
									DOUBLE * pValue
									);
//5.1.1.12 MCReadBoolParameter
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_ReadBoolParameter(
									UINT16 BoardID, //BoardID
									UINT16 AxisID,  //Axis number,
									UINT32 ParameterNum, //PN
									bool * pValue
									);
//5.1.1.12 MCReadIntParameter
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_ReadIntParameter(
									UINT16 BoardID, //BoardID
									UINT16 AxisID,  //Axis number,
									UINT32 ParameterNum, //PN
									UINT32 * pValue
									);
//5.1.1.13
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS MC_WriteParameter(
									UINT16 BoardID, 
									UINT16 AxisID, 
									UINT32 ParameterNum, 
									DOUBLE dValue
									);

//5.1.1.13
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS MC_WriteBoolParameter(
									UINT16 BoardID, 
									UINT16 AxisID, 
									UINT32 ParameterNum, 
									bool Value
									);

//5.1.1.13
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS MC_WriteIntParameter(
									UINT16 BoardID, 
									UINT16 AxisID, 
									UINT32 ParameterNum, 
									UINT32 dwValue
									);

//5.1.1.14 MCReadActualPosition
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_ReadActualPosition(
									UINT16 BoardID, //BoardID
									UINT16 AxisID,  //Axis number
									DOUBLE * pPosition //Position Acutal Value
									);
//5.1.1.21 MC_GearIn 
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS MC_GearIn (
									UINT16	BoardID,
									UINT16	MasterAxisID,
									UINT16	SlaveAxisID,
									UINT32	RatioNumerator,
									UINT32	RatioDenominator,
									MC_SOURCE	MasterValueSource,
									DOUBLE	Acceleration,
									DOUBLE	Deceleration,
									DOUBLE	Jerk,
									MC_BUFFER_MODE	BufferMode
									);
//5.1.1.22 MC_GearOut
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS MC_GearOut (
									UINT16	BoardID,
									UINT16	SlaveAxisID
									);

//5.1.1.24
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS MC_TouchProbe(
									UINT16 BoardID, 
									UINT16 AxisID, 
									UINT32 TriggerInput, 
									bool WindowOnly,
									DOUBLE FirstPosition, 
									DOUBLE LastPosition
									);
//5.1.1.25
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS MC_AbortTrigger(
									UINT16 BoardID, 
									UINT16 AxisID, 
									UINT32 TriggerInput
									);
//5.1.1.26
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS MC_ReadDigitalInput(
									UINT16 BoardID, 
									UINT16 AxisID, 
									UINT32 InputNumber,
									bool *pValue
									);
//5.1.1.27
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS MC_ReadDigitalOutput(
									UINT16 BoardID, 
									UINT16 AxisID, 
									UINT32 OutputNumber,
									bool *pValue
									);
//5.1.1.28
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS MC_WriteDigitalOutput(
									UINT16 BoardID, 
									UINT16 AxisID,
									UINT32 OutputNumber,
									bool Value
									);

//v12.2.0.11 add
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS MC_ReadDigitalInputByte(
									UINT16 BoardID,		 //_in BoardID
									UINT16 AxisID,		 //_in AxisID
									UINT8  Offset,		 //_in Offset (Range : 0 ~ 3)
									UINT8  Size,		 //_in Size (Range : 1 ~ 4)
									UINT8  InputValue[4] //_out InputValue[4] Array
									);

//v12.2.0.11 add
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS MC_ReadDigitalOutputByte(	 
									UINT16 BoardID,		  //_in BoardID
									UINT16 AxisID,		  //_in AxisID
									UINT8  Offset,		  //_in Offset (Range : 0 ~ 3)
									UINT8  Size,		  //_in Size (Range : 1 ~ 4)
									UINT8  OutputValue[4] //_out OutputValue[4] Array
									);

//v12.2.0.11 add
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS MC_WriteDigitalOutputByte(	  
									UINT16 BoardID,		  //_in BoardID
									UINT16 AxisID,		  //_in AxisID
									UINT8  Offset,		  //_in Offset (Range : 0 ~ 3)
									UINT8  Size,		  //_in Size (Range : 1 ~ 4)
									UINT8  OutputValue[4] //_in OutputValue[4] Array
									);

//5.1.1.29
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_SetPosition(
									UINT16 BoardID, //BoardID
									UINT16 AxisID,  //Axis number
									DOUBLE Position,
									bool Relative,
									MC_EXECUTION_MODE Mode
									);
//5.1.1.31 
#ifdef __cplusplus
extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_ReadActualVelocity(
									UINT16 BoardID, //BoardID
									UINT16 AxisID,  //Axis number
									DOUBLE * pVelocity //Velocity Acutal Value
									);

//5.1.1.37
#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS  MC_Halt(
									UINT16 BoardID, 
									UINT16 AxisID, 
									DOUBLE Deceleration, 
									DOUBLE Jerk, 
									MC_BUFFER_MODE BufferMode
									);
//5.1.1.38
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS MC_TriggerMonitor (
									UINT16 BoardID, 
									UINT16 AxisID, 
									UINT32 TriggerInput, 
									bool *pDone, 
									DOUBLE *pRecordedPosition,
									bool *pProbeActive
									);
//5.1.1.39 
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS  MC_ReadMotionState(
									UINT16 BoardID, //BoardID
									UINT16 AxisID,  //Axis number
									UINT32 * pMotionState //MC_MOTIONSTATE
									);
//5.1.1.40 
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS  MC_ReadAxisInfo(
									UINT16 BoardID, //BoardID
									UINT16 AxisID,  //Axis number
									UINT32 * pAxisInfo //MC_AXISINFO
									);
//5.1.1.41 
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS  MC_ReadAxisStatus(
									UINT16 BoardID, //BoardID
									UINT16 AxisID,  //Axis number
									UINT32 * pAxisStatus //MC_AXISSTATUS
									);
//5.1.1.42 MC_GearMonitor
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS MC_GearMonitor (
									UINT16	BoardID,
									UINT16	AxisID,
									UINT16 * pStatus
									);
//5.1.1.43 MC_ReadProfileData
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS MC_ReadProfileData (
									UINT16	BoardID,
									UINT16	AxisID,
									UINT32 * TickCount,
									DOUBLE * Position,
									DOUBLE * Velocity,
									DOUBLE * Accel,
									DOUBLE * Jerk,
									DOUBLE * ActPos,
									DOUBLE * ActVel
									);

//v12.1.0.45
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS  MC_ReadCommandedPosition(
									UINT16 BoardID,		//BoardID
									UINT16 AxisID,		//Axis number
									DOUBLE *pPosition   //Commmanded Position Value
									);

//v12.1.0.46
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS  MC_ReadCommandedVelocity(
									UINT16 BoardID,		//BoardID
									UINT16 AxisID,		//Axis number
									DOUBLE *pVelocity   //Commmanded Velocity Value
									);

//=============================================================================
//-----------------------------------------------------------------------------
//
//
// Motion APIs - Coordinated Motion
//
//
//-----------------------------------------------------------------------------
//5.1.1.44 MC_AddAxisToGroup [32]
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS MC_AddAxisToGroup (
									UINT16 BoardID,
									UINT16 AxisID,
									UINT16 AxesGroupNo,
									UINT16 IDInGroup
									);

//5.1.1.45 MC_RemoveAxisFromGroup [33]
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS MC_RemoveAxisFromGroup (
									UINT16 BoardID,
									UINT16 AxesGroupNo,
									UINT16 IDInGroup
									);

//5.1.1.46 MC_UngroupAllAxes [34]
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS MC_UngroupAllAxes (
									UINT16 BoardID,
									UINT16 AxesGroupNo
									);

//5.1.1.47 MC_GroupReadConfiguration [35]
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS MC_GroupReadConfiguration (
									UINT16 BoardID,
									UINT16 AxesGroupNo,
									UINT16 IDInGroup,
									MC_COORDSYSTEM  CoordSystem,
									UINT16 * AxisNo
									);

//5.1.1.48 MC_GroupEnable [36]
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS MC_GroupEnable (
									UINT16 BoardID,
									UINT16 AxesGroupNo
									);

//5.1.1.49 MC_GroupDisable [37]
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS MC_GroupDisable (
									UINT16 BoardID,
									UINT16 AxesGroupNo
									);

//5.1.1.50 MC_MoveLinearAbsolute [43]
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS MC_MoveLinearAbsolute (
									UINT16 BoardID,
									UINT16 AxesGroupNo,
									UINT16 PositionCount,
									DOUBLE * PositionData,
									DOUBLE Velocity,
									DOUBLE Acceleration,
									DOUBLE Deceleration,
									DOUBLE Jerk,
									MC_COORDSYSTEM CoordSystem,        //Coordination System 을 지정한다. //ACS, MCS, PCS
									MC_BUFFER_MODE BufferMode,         //Buffer Mode 를 지정한다.
									MC_TRANSITION_MODE TransitionMode, //Transition Mode 를 지정한다. 
									UINT16 TransitionParameterCount,   //TransitionParameter 의 Count 를 명시한다.
									DOUBLE * TransitionParameter       //TransitionParameterCount 에 명시된 크기 만큼의 Data 를 입력한다.
									);

//5.1.1.51 MC_GroupHalt [42]
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS MC_GroupHalt (
									UINT16 BoardID,
									UINT16 AxesGroupNo,
									DOUBLE Deceleration,
									DOUBLE Jerk,
									MC_BUFFER_MODE BufferMode //Buffer Mode 를 지정한다.
									);

//5.1.1.52 MC_GroupStop [41]
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS MC_GroupStop (
									UINT16 BoardID,
									UINT16 AxesGroupNo,
									bool  Execute,
									DOUBLE Deceleration,
									DOUBLE Jerk
									);

//5.1.1.53 MC_MoveCircularAbsolute2D [44]
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS MC_MoveCircularAbsolute2D (
									UINT16 BoardID,
									UINT16 AxesGroupNo,
									MC_CIRC_MODE CircMode,
									MC_CIRC_PATHCHOICE  PathChoice,
									DOUBLE AuxPoint[2],
									DOUBLE EndPoint[2],
									DOUBLE Angle,
									DOUBLE Velocity,
									DOUBLE Acceleration,
									DOUBLE Deceleration,
									DOUBLE Jerk,
									MC_COORDSYSTEM CordSystem,
									MC_BUFFER_MODE BufferMode,
									MC_TRANSITION_MODE TransitionMode,
									UINT16 TransitionParamCount,
									DOUBLE * TransitionParameter
									);

//v12.1.0.71
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS MC_MoveCircularAbsolute3D (
									UINT16 BoardID,						//_in BoardID
									UINT16 AxesGroupNo,					//_in Axes Group No
									MC_CIRC_MODE CircMode,				//_in Circ Mode (mcSPIRAL = 20, mcCYLINDER = 21, mcSPHERE = 22)
									DOUBLE OriginPoint[3],				//_in Origin Point[3]
									DOUBLE Angle,						//_in Angle
									DOUBLE Displacement[4],				//_in Displacement[4]
									DOUBLE Velocity,					//_in Velocity
									DOUBLE Acceleration,				//_in Acceleration
									DOUBLE Deceleration,				//_in Deceleration
									DOUBLE Jerk,						//_in Jerk
									MC_COORDSYSTEM CordSystem,			//_in Cord System (Reserved)
									MC_BUFFER_MODE BufferMode,			//_in BufferMode
									MC_TRANSITION_MODE TransitionMode,	//_in TransitionMode (mcTMProfileContinue = 11, :: RSA Transition Mode)
									UINT16 TransitionParamCount,		//_in TransitionParamCount
									DOUBLE * TransitionParameter		//_in TransitionParameter
									);

//v12.1.0.71
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS MC_ReadGroupStatus (
									UINT16 BoardID,			//_in BoardID
									UINT16 AxesGroupNo,		//_in Axes Group No
									UINT32 * pGroupStatus	//_out pGroupStatus (MMCE Board)
									);

//5.1.1.54 MC_GroupStop [38]
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS MC_GroupReadStatus (
									UINT16 BoardID,			//_in BoardID
									UINT16 AxesGroupNo,		//_in Axes Group No
									UINT32 * pGroupStatus	//_out pGroupStatus (Shared Memory)
									);

//5.1.1.55 MC_GroupReadError [39]
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS MC_GroupReadError (
									UINT16 BoardID,
									UINT16 AxesGroupNo,
									UINT16 * pErrorID,
									UINT16 * pErrorInfo0,
									UINT16 * pErrorInfo1
									);

//5.1.1.56 MC_GroupReset [40]
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS MC_GroupReset (
									UINT16 BoardID,
									UINT16 AxesGroupNo
									);

//5.1.1.57 MC_GroupReset []
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS MC_GroupReadProfileData (
									UINT16 BoardID,
									UINT16 AxesGroupNo,
									UINT16 * AxisCount,
									UINT32 * TimeTick, //msec
									DOUBLE ProfileDataArray[MAX_AXES_IN_GROUP][MAX_PROFILE_ITEM_COUNT] //[0]Pos,Vel,Acc,Jerk,ActPos,ActVel,[1]Pos,Vel,Acc,Jerk,ActPos,ActVel ...
									);

//5.1.1.58 MC_GroupReadInfo []
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS MC_GroupReadInfo (
									UINT16 BoardID,
									UINT16 AxesGroupNo,
									UINT16 * LastIdentNum,
									UINT16 IdentAxisNumList[MAX_AXES_IN_GROUP]
									);

//5.1.1.59 MC_ReadAllStatus []
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS MC_ReadAllStatus (
									UINT16 BoardID,
									UINT32 StatusData[MAX_ALL_STATUS_SIZE]
									);

//5.1.1.60 MC_GroupSetRawDataMode [] : 
#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS MC_GroupSetRawDataMode (
									UINT16 BoardID,
									UINT16 AxesGroupNo,
									MC_RAW_DATA_MODE Mode,
									bool Enable,		 //Raw data mode enable : 0:Disable, 1:Enable
									UINT8 ReservedZero1, //MC_COORDSYSTEM CoordSystem,
									UINT8 ReservedZero2	 //MC_BUFFER_MODE BufferMode
									);

//5.1.1.61 MC_GroupReadRawDataStatus [] : 
#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS MC_GroupReadRawDataStatus (
									UINT16 BoardID,
									UINT16 AxesGroupNo,
									MC_AxesGroupRawDataStatus *AxesGroupRawDataStatus
									);

//5.1.1.62 MC_GroupClearRawData [] : 
#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS MC_GroupClearRawData (
									UINT16 BoardID,
									UINT16 AxesGroupNo,
									UINT16 ReservedZero1,
									UINT16 ReservedZero2
									);

//5.1.1.63 MC_GroupSetRawData [] : 
#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS MC_GroupSetRawData (
									UINT16 BoardID,
									UINT16 AxesGroupNo,
									MC_SAVE_MODE SaveMode,
									UINT16 IndexNum,
									UINT16 RawDataSize,
									UINT16 RawDataCount,
									UINT8  * RawDataArray,
									UINT16 * StoredNum
									);
	
//5.1.1.64 MC_ReadRemainBuffer [] : 
#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS MC_ReadRemainBuffer (
									UINT16 BoardID,
									UINT16 AxesNo,
									UINT32 *BufferSize
									);

//5.1.1.65 MC_GroupReadRemainBuffer [] : 
#ifdef __cplusplus
	extern "C" 
#endif
	NMCMOTIONAPI MC_STATUS MC_GroupReadRemainBuffer (
									UINT16 BoardID,
									UINT16 AxesGroupNo,
									UINT32 *BufferSize
									);

//v12.1.0.47
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS	MC_GroupWriteParameter(
									UINT16 BoardID,      //_in BoardID
									UINT16 GroupNum,	 //_in GroupNum
									UINT32 ParameterNum, //_in ParameterNum
									DOUBLE Value		 //_in Value
									);

//v12.1.0.47
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS	MC_GroupReadParameter(
									UINT16 BoardID,      //_in BoardID
									UINT16 GroupNum,	 //_in GroupNum
									UINT32 ParameterNum, //_in ParameterNum
									DOUBLE *pValue		 //_out Value
									);

//=============================================================================
//-----------------------------------------------------------------------------
//
//
// Motion APIs - Multi Motion
//
//
//-----------------------------------------------------------------------------
//v12.1.0.40
#ifdef __cplusplus
	extern "C"
#endif
		NMCMOTIONAPI MC_STATUS MC_MoveAbsoluteMultiAxis( 
									UINT16 BoardID,					//Board ID
									UINT16 AxisCount,				//Number of Axis
									UINT16 *AxisArray,				//Array of Axis ID
									DOUBLE *PositionArray,			//Target Position Array
									DOUBLE Velocity,				//Max Velocity
									DOUBLE Acceleration,			//Max Acceleration
									DOUBLE Deceleration,			//Max Deceleration
									DOUBLE Jerk,					//Max Jerk
									MC_DIRECTION *DirectionArray,	//Array of Moving Direction for each Axis(0,1,2,3) 
									UINT8 ErrorStopMode				//Mode of multi-axes Motion Stop when Error occurred in one(or more) of multi-axes.
									);								//0:Remainder axes do not Stop (Continue their Motion), 1:Remainder axes also Stop with E-Stop Parameters.
		
//v12.1.0.40
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS MC_MoveRelativeMultiAxis(
									UINT16 BoardID,			//Board ID
									UINT16 AxisCount,		//Number of Axis
									UINT16 *AxisArray, 		//Array of Axis ID
									DOUBLE *PositionArray, 	//Target Position Array
									DOUBLE Velocity, 		//Max Velocity
									DOUBLE Acceleration, 	//Max Acceleration
									DOUBLE Deceleration, 	//Max Deceleration
									DOUBLE Jerk,			//Max Jerk
									UINT8 ErrorStopMode		//Mode of multi-axes Motion Stop when Error occurred in one(or more) of multi-axes.
									);						//0:Remainder axes do not Stop (Continue their Motion), 1:Remainder axes also Stop with E-Stop Parameters.

//v12.1.0.42 
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS MC_HaltMultiAxis(
									UINT16 BoardID,		//Board ID
									UINT16 AxisCount,	//Number of Axis
									UINT16 *AxisArray, 	//Array of Axis ID
									UINT8 ErrorStopMode	//Mode of multi-axes Motion Stop when Error occurred in one(or more) of multi-axes.
									);					//0:Remainder axes do not Stop (Continue their Motion), 1:Remainder axes also Stop with E-Stop Parameters.

//v12.1.0.43
#ifdef __cplusplus
	extern "C"
#endif
		NMCMOTIONAPI MC_STATUS MC_ReadMultiAxisStatus( 
									UINT16 BoardID,		//Board ID
									UINT16 AxisCount,	//Number of Axis
									UINT16 *AxisArray,	//Array of Axis ID
									UINT32 *Status		//Axis Status
									);

//=============================================================================
//-----------------------------------------------------------------------------
//
//
// Motion APIs - Gantry Motion
//
//
//-----------------------------------------------------------------------------
//Gantry 
//v12.2.0.0
#ifdef __cplusplus
	extern "C"
#endif
		NMCMOTIONAPI MC_STATUS MC_GantryEnable(
									UINT16 BoardID,		 //_in BoardID
									UINT16 GantryID,	 //_in GantryID (Range:0~1)
									UINT16 MasterAxisID, //_in Master AxisID
									UINT16 SlaveAxisID,	 //_in Slave AxisID
									DOUBLE VirtualRatio, //_in Virtual Ratio (Master/Slave Mode - Master(0), Virtual Axis Mode : 1 ~ 50)
									DOUBLE ErrorRange	 //_in Error Range (Yaw Error Range - Yaw Control Disable : 0, Yaw Control Enable : 1 ~ )
									);

//v12.2.0.0
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS MC_GantryDisable(
									UINT16 BoardID,		//_in BoardID
									UINT16 GantryID		//_in GantryID (Range:0~1)
									);

//v12.2.0.0
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS MC_GantryReadConfiguration(
									UINT16 BoardID,			//_in BoardID
									UINT16 MasterAxisID[4],	//_out MasterAxisID (array[2], array[3] Reserved)
									UINT16 SlaveAxisID[4],	//_out SlaveAxisID  (array[2], array[3] Reserved)
									DOUBLE VirtualRatio[4],	//_out Virtual Ratio (Master/Slave Mode - Master(0), Virtual Axis Mode : 1 ~ 50) (array[2], array[3] Reserved)
									DOUBLE ErrorRange[4]	//_out Error Range (Yaw Error Range - Yaw Control Disable : 0, Yaw Control Enable : 1 ~ ) (array[2], array[3] Reserved)
									);

//v12.2.0.0
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS MC_GantryReadStatus(
									UINT16 BoardID,	 //_in BoardID
									UINT16 GantryID, //_in GantryID (Range:0~1)
									UINT32 * Status	 //_out Status
									);

//v12.2.0.0
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS MC_GantryHome(
									UINT16 BoardID,			//_in BoardID
									UINT16 MasterAxisID,	//_in Master AxisID
									UINT16 HomeMethod,		//_in Home Method
									DOUBLE HomingVel,		//_in Homin Vel
									DOUBLE HomingAcc,		//_in Homin Acc
									DOUBLE HomingDec,		//_in Homin Dec
									DOUBLE HomingJerk,		//_in Homin Jerk
									DOUBLE HomingCreepVel,	//_in Homing Creep Vel
									UINT8  HomingDirection,	//_in Homing Direction (1:CW, 0:CCW)
									DOUBLE AlignOffset		//_in Align Offset
									);

//v12.2.0.2
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS MC_ChangeGantryAlign(
									UINT16 BoardID,				//_in BoardID
									UINT16 GantrySlaveAxisID,	//_in Gantry Slave AxisID (*Only Gantry Slave Axis ID, Virtual Axis Mode)
									DOUBLE AlignPosition,		//_in Align Position
									DOUBLE Velocity,			//_in Velocity
									DOUBLE Acceleration, 		//_in Acceleration
									DOUBLE Deceleration, 		//_in Deceleration
									DOUBLE Jerk,				//_in Jerk
									UINT8  DisplayMode			//_in DisplayMode //0 : Yaw Error Display (Master Axis - Slave Axis)
									);											  //1 : Yaw Diff. Display (Virtual Axis - Slave Axis)
																				  //2 : Yaw Error Display (Slave Axis - Master Axis)
																				  //3 : Yaw Diff. Display (Slave Axis - Virtual Axis)

//=============================================================================
//-----------------------------------------------------------------------------
//
//
// Motion APIs - Direct Output
//
//
//-----------------------------------------------------------------------------
//Pdo Output 
//v12.2.0.0
#ifdef __cplusplus
	extern "C"
#endif
		NMCMOTIONAPI MC_STATUS MC_DirectOutput(
									UINT16 BoardID,		//_in BoardID
									UINT16 AxisID,		//_in AxisID
									DOUBLE OutputValue	//_in Output Value
									);

//=============================================================================
//-----------------------------------------------------------------------------
//
//
// Motion APIs - Error Compensation
//
//
//-----------------------------------------------------------------------------
//Error Compensation
//v12.2.0.0
//v12.2.0.13
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS MC_ChangeErrorCompensationMode(
									UINT16 BoardID,		//_in BoardID
									UINT16 AxisID[3],	//_in AxisID (AxisID:1~65535 / Gantry:0~1 / Group:0~15 / Combination AxisID:1~65535)
									UINT8  Type,		//_in Type (Axis:0, Gantry:1, Group:2, Combination:3)
									UINT16 Mode,		//_in Mode (EC Data Mode Enable:1, EC Data Mode Disable:0)
									UINT8  ECMapID		//_in ECMapID (Range:0~3) (Table1 ~ Table4)									
									);

//v12.2.0.0
//v12.2.0.13
#ifdef __cplusplus
	extern "C"
#endif
		NMCMOTIONAPI MC_STATUS MC_ReadErrorCompensationStatus(
									UINT16 BoardID,		//_in BoardID
									UINT8  ECMapID,		//_in ECMapID (Range:0~3) (Table1 ~ Table4)
									UINT16 AxisID[3],	//_out AxisID (AxisID:1~65535 / Gantry:0~1 / Group:0~15 / Combination AxisID:1~65535)
									UINT8  * Type,		//_out Type (Axis:0, Gantry:1, Group-2D:2, Group-3D:3, 2D_Combination:4, 3D_Combination:5)
									UINT8  * Status		//_out Mode Status (Disabled:0, Enabled:1, Disabling:2, Enabling:3)
									);

//v12.2.0.0
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS MC_WriteErrorCompensationDataFile(
									UINT16 BoardID,			 //_in BoardID
									UINT16 ECType,			 //_in ECType (0:1D Axis or Gantry, 1:2D Group, 2:3D Group)
									UINT8  ECMapID,			 //_in ECMapID (Range:0~3) (Table1 ~ Table4)
									char * sFullPathFileName //_in WriteFile FullPathFileName
									);

//=============================================================================
//-----------------------------------------------------------------------------
//
//
// Motion APIs - etc. Application
//
//
//-----------------------------------------------------------------------------
//v12.2.0.3
#ifdef __cplusplus
	extern "C"
#endif
		NMCMOTIONAPI MC_STATUS MC_ChangeTorqueLimitPositionEvent(
			UINT16 BoardID,				//_in BoardID
			UINT16 AxisID,				//_in AxisID
			DOUBLE EventPosition,		//_in Event Position
			DOUBLE Width,				//_in Width
			UINT16 NormalTorque, 		//_in Normal Torque
			UINT16 EventTorque	 		//_in Event Torque
		);

//v12.2.0.4
#ifdef __cplusplus
	extern "C"
#endif
		NMCMOTIONAPI MC_STATUS MC_MovePositionEvent(
			UINT16 BoardID,				//_in BoardID
			UINT16 AxisID,				//_in Command AxisID
			UINT16 RefAxisID,			//_in Monitor AxisID (Trigger Source AxisID)
			DOUBLE EventPosition,		//_in Event Trigger Position
			UINT8  EventEdge,			//_in Event Trigger Edge (0 : Rising, 1 : Falling)
			UINT8  EventMotion,			//_in Event Motion (0 : Absolute, 1 : Relative)
			DOUBLE Position,			//_in Command Position
			DOUBLE Velocity,			//_in Command Velocity
			DOUBLE Acceleration,		//_in Command Acceleration
			DOUBLE Deceleration,		//_in Command Deceleration
			DOUBLE Jerk,				//_in Command Jerk
			MC_BUFFER_MODE BufferMode	//_in Buffer Mode (Reserved 0 : Aborting)
		);

//v12.2.0.13
#ifdef __cplusplus
	extern "C" 
#endif
		NMCMOTIONAPI MC_STATUS MC_SyncAxisToRefAxis(
			UINT16 BoardID,				//_in BoardID
			UINT16 AxisID,				//_in AxisID
			DOUBLE AxisEndPosition,		//_in Axis End Position (범위제한 없음 : -inf. ~ +inf.)
			UINT16 RefAxisID,			//_in Ref. AxisID
			DOUBLE RefStartPosition,	//_in Ref. Start Position (범위제한 없음 : -inf. ~ +inf.)
			DOUBLE RefEndPosition,		//_in Ref. End Position (범위제한 없음 : -inf. ~ +inf.)
			MC_BUFFER_MODE BufferMode	//_in Buffer Mode (Aborting:0, Buffered:1 옵션만 지원)
		);

//v12.2.0.13
#ifdef __cplusplus
	extern "C"
#endif
		NMCMOTIONAPI MC_STATUS MC_RTAFPositionSync(
			UINT16 BoardID,				//_in BoardID
			UINT16 AxisID,				//_in AxisID
			DOUBLE AxisStartPosition,	//_in Axis Start Position (범위제한 없음 : -inf. ~ +inf.)
			DOUBLE AxisEndPosition,		//_in Axis End Position (범위제한 없음 : -inf. ~ +inf.)
			UINT16 RefAxisID,			//_in Ref. AxisID
			DOUBLE RefStartPosition,	//_in Ref. Start Position (범위제한 없음 : -inf. ~ +inf.)
			DOUBLE RefEndPosition,		//_in Ref. End Position (범위제한 없음 : -inf. ~ +inf.)
			MC_BUFFER_MODE BufferMode	//_in Buffer Mode (Aborting:0, Buffered:1 옵션만 지원)
		);

//v12.2.0.13
#ifdef __cplusplus
	extern "C"
#endif
		NMCMOTIONAPI MC_STATUS MC_RTAFListIndex(
			UINT16 BoardID,				//_in BoardID
			UINT16 AxisID,				//_in AxisID
			UINT8  IndexInit,			//_in Index Init. (범위 : 0~9 / 0 - Read, 1 - Clear, 2 - Dec.,  3~9 - Reserved)
			UINT16 * RTAFIndex			//_out RTAF Index (0 ~ 65535)
		);

	//=============================================================================
	//-----------------------------------------------------------------------------
	//
	//
	// Function Module APIs - Special
	//
	//
	//-----------------------------------------------------------------------------
	//v12.1.0.48
#ifdef __cplusplus
	extern "C"
#endif
		NMCMOTIONAPI MC_STATUS	MC_WriteIntervalTrigParameterFM(
			UINT16 BoardID,			//_in BoardID
			UINT16 EcatAddr,		//_in Ethercat Address
			UINT16 AxisID,		    //_in AxisID
			DOUBLE StartPosition,	//_in Start Position
			DOUBLE EndPosition,		//_in End Position
			UINT16 IntervalPeriod,	//_in Interval Period
			UINT16 PulseWidth		//_in Pulse Width
		);

	//v12.1.0.48
#ifdef __cplusplus
	extern "C"
#endif
		NMCMOTIONAPI MC_STATUS	MC_WriteIntervalTrigEnableFM(
			UINT16 BoardID,		//_in BoardID
			UINT16 EcatAddr,	//_in Ethercat Address
			UINT16 AxisID,		//_in AxisID
			bool Enable			//_in Configuration Enable True/False
		);

#endif