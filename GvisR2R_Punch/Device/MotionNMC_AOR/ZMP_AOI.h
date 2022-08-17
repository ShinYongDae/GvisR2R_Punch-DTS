/******************************************************************************
* File name : ZMP_AOI.h
* Version   : 1.00
* Date      : 2021.03.03
******************************************************************************/

#pragma once


#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	MPIStateIDLE,
	MPIStateMOVING,
	MPIStateSTOPPING,
	MPIStateSTOPPED,
	MPIStateSTOPPING_ERROR,
	MPIStateERROR,
} MPIState;

typedef enum {
	MPIMotionTypeINVALID = -1,


	MPIMotionTypePT,
	MPIMotionTypePTF,
	MPIMotionTypePVT,
	MPIMotionTypePVTF,
	MPIMotionTypeSPLINE,
	MPIMotionTypeBESSEL,
	MPIMotionTypeBSPLINE,
	MPIMotionTypeBSPLINE2,


	MPIMotionTypeS_CURVE,
	MPIMotionTypeTRAPEZOIDAL,
	MPIMotionTypeS_CURVE_JERK,


	MPIMotionTypeVELOCITY,
	MPIMotionTypeVELOCITY_JERK,


	MPIMotionTypeCAM_LINEAR,
	MPIMotionTypeCAM_CUBIC,


	MPIMotionTypeFRAME,


	MPIMotionTypeEND,
	MPIIMotionTypeFIRST = MPIMotionTypeINVALID + 1,
} MPIMotionType;

typedef enum {
	MPIActionINVALID = -1,

	MPIActionNONE,
	MPIActionTRIGGERED_MODIFY,
	MPIActionSTOP,
	MPIActionE_STOP,
	MPIActionE_STOP_ABORT,
	MPIActionE_STOP_MODIFY,
	MPIActionE_STOP_CMD_EQ_ACT,
	MPIActionABORT,

	MPIActionDONE,
	MPIActionSTART,
	MPIActionRESUME,
	MPIActionRESET,
	MPIActionCANCEL_REPEAT,

	MPIActionEND,
	MPIActionFIRST = MPIActionINVALID + 1
} MPIAction;


/* General messages */
typedef enum MPIMessage {
	MPIMessageINVALID = -1,

	MPIMessageOK,

	MPIMessageARG_INVALID,
	MPIMessagePARAM_INVALID,
	MPIMessageHANDLE_INVALID,

	MPIMessageNO_MEMORY,

	MPIMessageOBJECT_FREED,
	MPIMessageOBJECT_NOT_ENABLED,
	MPIMessageOBJECT_NOT_FOUND,
	MPIMessageOBJECT_ON_LIST,
	MPIMessageOBJECT_IN_USE,

	MPIMessageTIMEOUT,

	MPIMessageUNSUPPORTED,
	MPIMessageFATAL_ERROR,

	MPIMessageFILE_CLOSE_ERROR,
	MPIMessageFILE_OPEN_ERROR,
	MPIMessageFILE_READ_ERROR,
	MPIMessageFILE_WRITE_ERROR,
	MPIMessageFILE_MISMATCH,

	MPIMessageASSERT_FAILURE,

	MPIMessageINVALID_IO_CONFIG,

	MPIMessageEND,
	MPIMessageFIRST = MPIMessageINVALID + 1,
	MPIMessageCOUNT = MPIMessageEND - MPIMessageFIRST
} MPIMessage;







typedef enum { TRAPEZOIDAL = MPIMotionTypeTRAPEZOIDAL, S_CURVE = MPIMotionTypeS_CURVE_JERK, PARABOLIC } SpeedProfile;

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
	MotionTypeS_CURVE = MPIMotionTypeS_CURVE_JERK,
	MotionTypeTRAPEZOIDAL = MPIMotionTypeTRAPEZOIDAL,
	MotionTypeS_CURVE_JERK = MPIMotionTypeS_CURVE_JERK,
	MotionTypeVELOCITY = MPIMotionTypeVELOCITY_JERK,
	MotionTypeVELOCITY_JERK = MPIMotionTypeVELOCITY_JERK,
	MotionTypeCAM_LINEAR = MPIMotionTypeCAM_LINEAR,
	MotionTypeCAM_CUBIC = MPIMotionTypeCAM_CUBIC,
	MotionTypeFRAME = MPIMotionTypeFRAME,
//	MotionTypeUSER = MPIMotionTypeUSER,
	MotionTypeEND = MPIMotionTypeEND,
//	MotionTypeFIRST = MPIMotionTypeFIRST,
	MotionTypeLAST,
	MotionTypeMASK
} MotionType;





#ifdef __cplusplus
}

#endif
