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

//����̽��� ��� Status ����
//BoardID, DeviceID : Board ID, Status ������ ���� �� Device Array
//Size : Device Count
//Mode : 0 Init, 1 Boot, 2 PreOP, 3 SafeOP, 4 OP
//TimeOut : TimeOut ó�� �ð� (msec), �ش� ���� �ð����� Status ������ �Ϸ� ���� ���� ��� Return Error
APPMOTIONAPI MC_STATUS __stdcall MC_StatusChange_OP(UINT16 BoardID, UINT16* DeviceID, UINT16 Size,UINT16 Mode, UINT32 TimmeOut);

/*Trigger Configuration ����, ���� Trigger Index �� Trigger ����� ���� �⺻ �Ķ���͸�  ���� �Ѵ�.
BoardID, DeviceID : BoardID, FunctionModule DeviceID
TriggerIndex : Trigger Index (1~3, 5~7 ���� ����) 
EncSorceIndex : Encoder Sorce (0~7 ���� ����)
InvertEncDir : Trigger Position ���� �� ���� ����. (false : NoInvert, true : Invert)
EncRatio : Trigger Position ���� �� Ratio �ݿ�, Encoder Ratio �� ����
VLevel : (0:24V, 1:5V, 2:Both)  (5V ��ȣ�� �� �� Axis Cable 11,12 Pin, 24V ��ȣ�� DIO Cable ���� ��� �ȴ�)
ActiveLevel : Trigger ��� Active Level ���� (true : High, false : Low)
Mode : 0:Interval, 1:Table, 2:OneShot, 3 : Frequency*/
APPMOTIONAPI MC_STATUS __stdcall TR_SetTriggerConfig(UINT16 BoardID, UINT16 DeviceID ,UINT16 TriggerIndex, UINT16 EncSorceIndex, bool InvertEncDir, double EncRatio, UINT32 VLevel, bool ActiveLevel, UINT32 Mode);  

/*
Encoder Counter �� Clear �Ѵ�.*/
APPMOTIONAPI MC_STATUS __stdcall TR_TriggerEncoderCounterClear(UINT16 BoardID, UINT16 DeviceID, UINT16 EncSorceIndex);

/*Table Trigger ������ ���� �Ѵ�.
BoardID, DeviceID : BoardID, FunctionModule DeviceID
TriggerIndex : Trigger Index (1~3, 5~7 ���� ����) 
Trigger ��� Position
Mode : Signal ��� Mode (0:High, 1:LOW, 2:High_Width, 3:Low_Width, 4:Toggle)
Width : Trigger ��� �� ���� (1�� 100usec, Max 16383 ���� ����)
Count : Trigger ���� (Max 128 ��) */
APPMOTIONAPI MC_STATUS __stdcall TR_SetTableTriggerData(UINT16 BoardID, UINT16 DeviceID, UINT16 TriggerIndex, double* Pos, UINT32* Mode, UINT16 Width, UINT16 Count);

/*
Table Trigger Data �� Clear �Ѵ�.*/
APPMOTIONAPI MC_STATUS __stdcall TR_TableTriggerDataClear(UINT16 BoardID, UINT16 DeviceID, UINT16 TriggerIndex, bool TriggerOff);

/*
Interval Trigger ���� ������ ���� �Ѵ�.
BoardID, DeviceID : BoardID, FunctionModule DeviceID
TriggerIndex : Trigger Index (1~3, 5~7 ���� ����) 
StartPos, EndPos : Interval Trigger ���� ��ġ ���� ��ġ
Period : Trigger �߻� �ֱ� ����
Widhth : Trigger �߻� ��. (1�� 50nsec)*/
APPMOTIONAPI MC_STATUS __stdcall TR_SetIntervalTriggerData(UINT16 BoardID, UINT16 DeviceID, UINT16 TriggerIndex, double StartPos, double EndPos, double Period, UINT16 Width);

/*
Interval Trigger Enable, Disable
BoardID, DeviceID : BoardID, FunctionModule DeviceID
TriggerIndex : Trigger Index (1~3, 5~7 ���� ����) 
Mode : Start ��ġ���� Trigger ��� ���� ���� (0:None Trigger, 1:With Trigger) */
APPMOTIONAPI MC_STATUS __stdcall TR_SetIntervalTriggerEnable(UINT16 BoardID, UINT16 DeviceID, UINT16 TriggerIndex, bool Enable, UINT32 Mode);


/*
Interval Trigger ���� ������ Enable/Disable ������ �ѹ��� ���� �Ѵ�, �ش� �Լ� ȣ�� �� Trigger Mode �� �ڵ����� Interval Mode�� ���� �ȴ�.
BoardID, DeviceID : BoardID, FunctionModule DeviceID
TriggerIndex : Trigger Index (1~3, 5~7 ���� ����) 
StartPos, EndPos : Interval Trigger ���� ��ġ ���� ��ġ
Period : Trigger �߻� �ֱ� ����
Widhth : Trigger �߻� ��. (1�� 50nsec)
Mode : Start ��ġ���� Trigger ��� ���� ���� (0:None Trigger, 1:With Trigger)
*/
APPMOTIONAPI MC_STATUS __stdcall TR_SetIntervalTriggerData_WEN(UINT16 BoardID, UINT16 DeviceID, UINT16 TriggerIndex, double StartPos, double EndPos, double Period, UINT16 Width, bool Enable,  UINT32 Mode);

/*
OneShot Trigger �� �߻� ��Ų��.
BoardID, DeviceID : BoardID, FunctionModule DeviceID
TriggerIndex : Trigger Index (1~3, 5~7 ���� ����) 
Width : Trigger ��� �� ���� (1�� 50nsec, Max 65535 ���� ����)*/
APPMOTIONAPI MC_STATUS __stdcall TR_TriggerOneShot(UINT16 BoardID, UINT16 DeviceID, UINT16 TriggerIndex, UINT16 Width);

/*
Frequency Trigger ���� ������ ���� �Ѵ�.
BoardID, DeviceID : BoardID, FunctionModule DeviceID
TriggerIndex : Trigger Index (1~3, 5~7 ���� ����) 
Frequency : Trigger ��� Hz ���� ( 1 ~ 1000000 ���� ����)
Count : Trigger ��� ���� ����
Width : Trigger ��� �� ���� (1�� 50nsec, Max 65535 ���� ����)*/
APPMOTIONAPI MC_STATUS __stdcall TR_SetFrequencyTriggerData(UINT16 BoardID, UINT16 DeviceID, UINT16 TriggerIndex, UINT32 Frequency, UINT32 Count ,UINT16 Width);

/*
Frequency Trigger Enable, Disable
BoardID, DeviceID : BoardID, FunctionModule DeviceID
TriggerIndex : Trigger Index (1~3, 5~7 ���� ����)
Mode : Count, Infinite (0 : Count Mode, 1 : Infinite Mode)*/
APPMOTIONAPI MC_STATUS __stdcall TR_SetFrequencyTriggerEnable(UINT16 BoardID, UINT16 DeviceID, UINT16 TriggerIndex, bool Enable, UINT32 Mode);

/*
Frequency Trigger ���� ������ Enable/Disable ������ �ѹ��� ���� �Ѵ�, �ش� �Լ� ȣ��� Mode �� �ڵ����� Frequency Mode �� ����ȴ�.
BoardID, DeviceID : BoardID, FunctionModule DeviceID
TriggerIndex : Trigger Index (1~3, 5~7 ���� ����) 
Frequency : Trigger ��� Hz ���� ( 1 ~ 1000000 ���� ����)
Count : Trigger ��� ���� ����
Width : Trigger ��� �� ���� (1�� 50nsec, Max 65535 ���� ����)
Mode : Count, Infinite (0 : Count Mode, 1 : Infinite Mode)*/
APPMOTIONAPI MC_STATUS __stdcall TR_SetFrequencyTriggerData_WEN(UINT16 BoardID, UINT16 DeviceID, UINT16 TriggerIndex, UINT32 Frequency, UINT32 Count ,UINT16 Width, bool Enable, UINT32 Mode);


APPMOTIONAPI MC_STATUS __stdcall TR_GetIntervalPulseCount(UINT16 BoardID, UINT16 DeviceID, UINT16 TriggerIndex, UINT32* PulseCount);

APPMOTIONAPI MC_STATUS __stdcall TR_GetFrequencyPulseCount(UINT16 BoardID, UINT16 DeviceID, UINT16 TriggerIndex, UINT32* PulseCount);

#ifdef __cplusplus
}
#endif	

#endif
