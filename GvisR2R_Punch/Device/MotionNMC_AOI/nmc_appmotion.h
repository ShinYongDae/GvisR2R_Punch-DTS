#ifndef __NMCAPPMOTION_H__
#define __NMCAPPMOTION_H__


#ifndef __NMCMOTIONSDK_H__
#include "NMC_Motion.h"
#endif


#ifdef FLAMENCOMOTIONAPI_EXPORTS
#define APPMOTIONAPI __declspec(dllexport)
#else
#define APPMOTIONAPI __declspec(dllimport)
#endif

//
typedef enum TRConfigEncDir_tag
{
	TRC_NoInvert = 0,
	TRC_Invert 
}TRConfigEncDir;

typedef enum TRConfigVLevel_tag
{
	Level_24V = 0,
	Level_5V,
	Level_Both,
}TRConfigVLevel;

typedef enum TRConfigMode_tag
{
	Mode_Table = 0,
	Mode_Interval = 1,
	Mode_OneShot = 2,
	Mode_Frequency = 3
}TRConfigMode;

typedef enum TRTableMode_tag
{
	Mode_High = 0,
	Mode_Low,
	Mode_High_Width,
	Mode_Low_Width,
	Mode_Toggle,
}TRTableMode;

#ifdef __cplusplus
extern "C" {
#endif	//__cplusplus

//디바이스의 통신 Status 변경
//BoardID, DeviceID : Board ID, Status 변경을 진행 할 Device Array
//Size : Device Count
//Mode : 0 Init, 1 Boot, 2 PreOP, 3 SafeOP, 4 OP
//TimeOut : TimeOut 처리 시간 (msec), 해당 설정 시간내에 Status 변경이 완료 되지 않을 경우 Return Error
APPMOTIONAPI MC_STATUS __stdcall MC_StatusChange_OP(UINT16 BoardID, UINT16* DeviceID, UINT16 Size,UINT16 Mode, UINT32 TimmeOut);

/*Trigger Configuration 설정, 선택 Trigger Index 의 Trigger 출력을 위한 기본 파라미터를  설정 한다.
BoardID, DeviceID : BoardID, FunctionModule DeviceID
TriggerIndex : Trigger Index (1~3, 5~7 선택 가능) 
EncSorceIndex : Encoder Sorce (0~7 선택 가능)
InvertEncDir : Trigger Position 설정 시 방향 반전. (false : NoInvert, true : Invert)
EncRatio : Trigger Position 설정 시 Ratio 반영, Encoder Ratio 값 설정
VLevel : (0:24V, 1:5V, 2:Both)  (5V 신호는 각 축 Axis Cable 11,12 Pin, 24V 신호는 DIO Cable 에서 출력 된다)
ActiveLevel : Trigger 출력 Active Level 설정 (true : High, false : Low)
Mode : 0:Interval, 1:Table, 2:OneShot, 3 : Frequency*/
APPMOTIONAPI MC_STATUS __stdcall TR_SetTriggerConfig(UINT16 BoardID, UINT16 DeviceID ,UINT16 TriggerIndex, UINT16 EncSorceIndex, bool InvertEncDir, double EncRatio, UINT32 VLevel, bool ActiveLevel, UINT32 Mode);  

/*
Encoder Counter 를 Clear 한다.*/
APPMOTIONAPI MC_STATUS __stdcall TR_TriggerEncoderCounterClear(UINT16 BoardID, UINT16 DeviceID, UINT16 EncSorceIndex);

/*Table Trigger 설정을 진행 한다.
BoardID, DeviceID : BoardID, FunctionModule DeviceID
TriggerIndex : Trigger Index (1~3, 5~7 선택 가능) 
Trigger 출력 Position
Mode : Signal 출력 Mode (0:High, 1:LOW, 2:High_Width, 3:Low_Width, 4:Toggle)
Width : Trigger 출력 폭 설정 (1당 100usec, Max 16383 설정 가능)
Count : Trigger 개수 (Max 128 개) */
APPMOTIONAPI MC_STATUS __stdcall TR_SetTableTriggerData(UINT16 BoardID, UINT16 DeviceID, UINT16 TriggerIndex, double* Pos, UINT32* Mode, UINT16 Width, UINT16 Count);

/*
Table Trigger Data 를 Clear 한다.*/
APPMOTIONAPI MC_STATUS __stdcall TR_TableTriggerDataClear(UINT16 BoardID, UINT16 DeviceID, UINT16 TriggerIndex, bool TriggerOff);

/*
Interval Trigger 관련 설정을 진행 한다.
BoardID, DeviceID : BoardID, FunctionModule DeviceID
TriggerIndex : Trigger Index (1~3, 5~7 선택 가능) 
StartPos, EndPos : Interval Trigger 시작 위치 종료 위치
Period : Trigger 발생 주기 설정
Widhth : Trigger 발생 폭. (1당 50nsec)*/
APPMOTIONAPI MC_STATUS __stdcall TR_SetIntervalTriggerData(UINT16 BoardID, UINT16 DeviceID, UINT16 TriggerIndex, double StartPos, double EndPos, double Period, UINT16 Width);

/*
Interval Trigger Enable, Disable
BoardID, DeviceID : BoardID, FunctionModule DeviceID
TriggerIndex : Trigger Index (1~3, 5~7 선택 가능) 
Mode : Start 위치에서 Trigger 출력 여부 설정 (0:None Trigger, 1:With Trigger) */
APPMOTIONAPI MC_STATUS __stdcall TR_SetIntervalTriggerEnable(UINT16 BoardID, UINT16 DeviceID, UINT16 TriggerIndex, bool Enable, UINT32 Mode);


/*
Interval Trigger 관련 설정과 Enable/Disable 동작을 한번에 진행 한다, 해당 함수 호출 시 Trigger Mode 는 자동으로 Interval Mode로 설정 된다.
BoardID, DeviceID : BoardID, FunctionModule DeviceID
TriggerIndex : Trigger Index (1~3, 5~7 선택 가능) 
StartPos, EndPos : Interval Trigger 시작 위치 종료 위치
Period : Trigger 발생 주기 설정
Widhth : Trigger 발생 폭. (1당 50nsec)
Mode : Start 위치에서 Trigger 출력 여부 설정 (0:None Trigger, 1:With Trigger)
*/
APPMOTIONAPI MC_STATUS __stdcall TR_SetIntervalTriggerData_WEN(UINT16 BoardID, UINT16 DeviceID, UINT16 TriggerIndex, double StartPos, double EndPos, double Period, UINT16 Width, bool Enable,  UINT32 Mode);

/*
OneShot Trigger 를 발생 시킨다.
BoardID, DeviceID : BoardID, FunctionModule DeviceID
TriggerIndex : Trigger Index (1~3, 5~7 선택 가능) 
Width : Trigger 출력 폭 설정 (1당 50nsec, Max 65535 설정 가능)*/
APPMOTIONAPI MC_STATUS __stdcall TR_TriggerOneShot(UINT16 BoardID, UINT16 DeviceID, UINT16 TriggerIndex, UINT16 Width);

/*
Frequency Trigger 관련 설정을 진행 한다.
BoardID, DeviceID : BoardID, FunctionModule DeviceID
TriggerIndex : Trigger Index (1~3, 5~7 선택 가능) 
Frequency : Trigger 출력 Hz 설정 ( 1 ~ 1000000 설정 가능)
Count : Trigger 출력 개수 설정
Width : Trigger 출력 폭 설정 (1당 50nsec, Max 65535 설정 가능)*/
APPMOTIONAPI MC_STATUS __stdcall TR_SetFrequencyTriggerData(UINT16 BoardID, UINT16 DeviceID, UINT16 TriggerIndex, UINT32 Frequency, UINT32 Count ,UINT16 Width);

/*
Frequency Trigger Enable, Disable
BoardID, DeviceID : BoardID, FunctionModule DeviceID
TriggerIndex : Trigger Index (1~3, 5~7 선택 가능)
Mode : Count, Infinite (0 : Count Mode, 1 : Infinite Mode)*/
APPMOTIONAPI MC_STATUS __stdcall TR_SetFrequencyTriggerEnable(UINT16 BoardID, UINT16 DeviceID, UINT16 TriggerIndex, bool Enable, UINT32 Mode);

/*
Frequency Trigger 관련 설정과 Enable/Disable 동작을 한번에 진행 한다, 해당 함수 호출시 Mode 는 자동으로 Frequency Mode 로 변경된다.
BoardID, DeviceID : BoardID, FunctionModule DeviceID
TriggerIndex : Trigger Index (1~3, 5~7 선택 가능) 
Frequency : Trigger 출력 Hz 설정 ( 1 ~ 1000000 설정 가능)
Count : Trigger 출력 개수 설정
Width : Trigger 출력 폭 설정 (1당 50nsec, Max 65535 설정 가능)
Mode : Count, Infinite (0 : Count Mode, 1 : Infinite Mode)*/
APPMOTIONAPI MC_STATUS __stdcall TR_SetFrequencyTriggerData_WEN(UINT16 BoardID, UINT16 DeviceID, UINT16 TriggerIndex, UINT32 Frequency, UINT32 Count ,UINT16 Width, bool Enable, UINT32 Mode);


APPMOTIONAPI MC_STATUS __stdcall TR_GetIntervalPulseCount(UINT16 BoardID, UINT16 DeviceID, UINT16 TriggerIndex, UINT32* PulseCount);

APPMOTIONAPI MC_STATUS __stdcall TR_GetFrequencyPulseCount(UINT16 BoardID, UINT16 DeviceID, UINT16 TriggerIndex, UINT32* PulseCount);

#ifdef __cplusplus
}
#endif	

#endif
