
// SR1000W.h

#pragma once

// Direct Control Command ====================================================================

// Read operation control
#define SR_TriggerInputOn						"LON"			// Response : Data or ERROR
#define SR_TriggerInputOnBank					"LON,"			// + [ 01 ~ 16 : BankNumber ] Response : Data or ERROR
#define SR_TriggerInputOnBank1					"LON,01"		// Response : Data or ERROR
#define SR_TriggerInputOnBank2					"LON,02"		// Response : Data or ERROR
#define SR_TriggerInputOnBank3					"LON,03"		// Response : Data or ERROR
#define SR_TriggerInputOnBank4					"LON,04"		// Response : Data or ERROR
#define SR_TriggerInputOnBank5					"LON,05"		// Response : Data or ERROR
#define SR_TriggerInputOnBank6					"LON,06"		// Response : Data or ERROR
#define SR_TriggerInputOnBank7					"LON,07"		// Response : Data or ERROR
#define SR_TriggerInputOnBank8					"LON,08"		// Response : Data or ERROR
#define SR_TriggerInputOnBank9					"LON,09"		// Response : Data or ERROR
#define SR_TriggerInputOnBank10				"LON,10"		// Response : Data or ERROR
#define SR_TriggerInputOnBank11				"LON,11"		// Response : Data or ERROR
#define SR_TriggerInputOnBank12				"LON,12"		// Response : Data or ERROR
#define SR_TriggerInputOnBank13				"LON,13"		// Response : Data or ERROR
#define SR_TriggerInputOnBank14				"LON,14"		// Response : Data or ERROR
#define SR_TriggerInputOnBank15				"LON,15"		// Response : Data or ERROR
#define SR_TriggerInputOnBank16				"LON,16"		// Response : Data or ERROR
#define SR_TriggerInputOff						"LOFF"			// Response : Data or ERROR

// Tunning
#define SR_AdjustFocus							"FTUNE"			// Response : SUCCEEDED or FAILED
#define SR_TunningBank							"TUNE,"			// + [ 01 ~ 16 : BankNumber ] Response : SUCCEEDED or FAILED
#define SR_TunningBank1						"TUNE,01"		// Response : SUCCEEDED or FAILED
#define SR_TunningBank2						"TUNE,02"		// Response : SUCCEEDED or FAILED
#define SR_TunningBank3						"TUNE,03"		// Response : SUCCEEDED or FAILED
#define SR_TunningBank4						"TUNE,04"		// Response : SUCCEEDED or FAILED
#define SR_TunningBank5						"TUNE,05"		// Response : SUCCEEDED or FAILED
#define SR_TunningBank6						"TUNE,06"		// Response : SUCCEEDED or FAILED
#define SR_TunningBank7						"TUNE,07"		// Response : SUCCEEDED or FAILED
#define SR_TunningBank8						"TUNE,08"		// Response : SUCCEEDED or FAILED
#define SR_TunningBank9						"TUNE,09"		// Response : SUCCEEDED or FAILED
#define SR_TunningBank10						"TUNE,10"		// Response : SUCCEEDED or FAILED
#define SR_TunningBank11						"TUNE,11"		// Response : SUCCEEDED or FAILED
#define SR_TunningBank12						"TUNE,12"		// Response : SUCCEEDED or FAILED
#define SR_TunningBank13						"TUNE,13"		// Response : SUCCEEDED or FAILED
#define SR_TunningBank14						"TUNE,14"		// Response : SUCCEEDED or FAILED
#define SR_TunningBank15						"TUNE,15"		// Response : SUCCEEDED or FAILED
#define SR_TunningBank16						"TUNE,16"		// Response : SUCCEEDED or FAILED
#define SR_TunningOff							"TQUIT"			// Response : OK

// Preset data registration
#define SR_StartPresetDataRegistration			"PRON"			// Response : None
#define SR_FinishPresetDataRegistartion		"PROFF"			// Response : None

// Setting or Reading a batch-setting Bar code
#define SR_StartBatchSetBarcodeReading			"RCON"			// Response : OK,RCON 
#define SR_QuitBatchSetBarcodeReading			"RCOFF"			// Response : OK,RCOFF
#define SR_CheckBatchSetBarcodeReadingStatus	"RCCHK"			// Response : READING Data

// Test mode control
#define SR_StartReadingRatioTestMode			"TEST1"			// Response : OK,TEST1
#define SR_StartReadingRatioTestModeBank		"TEST1,"		// + [ 01 ~ 16 : BankNumber ] Response : OK,TEST1
#define SR_StartReadingRatioTestModeBank1		"TEST1,01"		// Response : OK,TEST1
#define SR_StartReadingRatioTestModeBank2		"TEST1,02"		// Response : OK,TEST1
#define SR_StartReadingRatioTestModeBank3		"TEST1,03"		// Response : OK,TEST1
#define SR_StartReadingRatioTestModeBank4		"TEST1,04"		// Response : OK,TEST1
#define SR_StartReadingRatioTestModeBank5		"TEST1,05"		// Response : OK,TEST1
#define SR_StartReadingRatioTestModeBank6		"TEST1,06"		// Response : OK,TEST1
#define SR_StartReadingRatioTestModeBank7		"TEST1,07"		// Response : OK,TEST1
#define SR_StartReadingRatioTestModeBank8		"TEST1,08"		// Response : OK,TEST1
#define SR_StartReadingRatioTestModeBank9		"TEST1,09"		// Response : OK,TEST1
#define SR_StartReadingRatioTestModeBank10		"TEST1,10"		// Response : OK,TEST1
#define SR_StartReadingRatioTestModeBank11		"TEST1,11"		// Response : OK,TEST1
#define SR_StartReadingRatioTestModeBank12		"TEST1,12"		// Response : OK,TEST1
#define SR_StartReadingRatioTestModeBank13		"TEST1,13"		// Response : OK,TEST1
#define SR_StartReadingRatioTestModeBank14		"TEST1,14"		// Response : OK,TEST1
#define SR_StartReadingRatioTestModeBank15		"TEST1,15"		// Response : OK,TEST1
#define SR_StartReadingRatioTestModeBank16		"TEST1,16"		// Response : OK,TEST1
#define SR_StartTactMeasurementTestMode		"TEST2"			// Response : OK,TEST2
#define SR_StartTactMeasurementTestModeBank	"TEST2,"		// + [ 01 ~ 16 : BankNumber ] Response : OK,TEST2
#define SR_StartTactMeasurementTestModeBank1	"TEST2,01"		// Response : OK,TEST2
#define SR_StartTactMeasurementTestModeBank2	"TEST2,02"		// Response : OK,TEST2
#define SR_StartTactMeasurementTestModeBank3	"TEST2,03"		// Response : OK,TEST2
#define SR_StartTactMeasurementTestModeBank4	"TEST2,04"		// Response : OK,TEST2
#define SR_StartTactMeasurementTestModeBank5	"TEST2,05"		// Response : OK,TEST2
#define SR_StartTactMeasurementTestModeBank6	"TEST2,06"		// Response : OK,TEST2
#define SR_StartTactMeasurementTestModeBank7	"TEST2,07"		// Response : OK,TEST2
#define SR_StartTactMeasurementTestModeBank8	"TEST2,08"		// Response : OK,TEST2
#define SR_StartTactMeasurementTestModeBank9	"TEST2,09"		// Response : OK,TEST2
#define SR_StartTactMeasurementTestModeBank10	"TEST2,10"		// Response : OK,TEST2
#define SR_StartTactMeasurementTestModeBank11	"TEST2,11"		// Response : OK,TEST2
#define SR_StartTactMeasurementTestModeBank12	"TEST2,12"		// Response : OK,TEST2
#define SR_StartTactMeasurementTestModeBank13	"TEST2,13"		// Response : OK,TEST2
#define SR_StartTactMeasurementTestModeBank14	"TEST2,14"		// Response : OK,TEST2
#define SR_StartTactMeasurementTestModeBank15	"TEST2,15"		// Response : OK,TEST2
#define SR_StartTactMeasurementTestModeBank16	"TEST2,16"		// Response : OK,TEST2
//#define SR_StartBarcodePositionTestMode		"TEST3"			// Response : None
#define SR_QuitTestMode						"QUIT"			// Response : OK,QUIT
#define SR_StartOnlineTestMode1				"#TEST1"		// Response : OK,#TEST1
#define SR_StartOnlineTestMode1Bank			"#TEST1,"		// + [ 01 ~ 16 : BankNumber ] Response : OK,#TEST1
#define SR_StartOnlineTestMode1Bank1			"#TEST1,01"		// Response : OK,#TEST1
#define SR_StartOnlineTestMode1Bank2			"#TEST1,02"		// Response : OK,#TEST1
#define SR_StartOnlineTestMode1Bank3			"#TEST1,03"		// Response : OK,#TEST1
#define SR_StartOnlineTestMode1Bank4			"#TEST1,04"		// Response : OK,#TEST1
#define SR_StartOnlineTestMode1Bank5			"#TEST1,05"		// Response : OK,#TEST1
#define SR_StartOnlineTestMode1Bank6			"#TEST1,06"		// Response : OK,#TEST1
#define SR_StartOnlineTestMode1Bank7			"#TEST1,07"		// Response : OK,#TEST1
#define SR_StartOnlineTestMode1Bank8			"#TEST1,08"		// Response : OK,#TEST1
#define SR_StartOnlineTestMode1Bank9			"#TEST1,09"		// Response : OK,#TEST1
#define SR_StartOnlineTestMode1Bank10			"#TEST1,10"		// Response : OK,#TEST1
#define SR_StartOnlineTestMode1Bank11			"#TEST1,11"		// Response : OK,#TEST1
#define SR_StartOnlineTestMode1Bank12			"#TEST1,12"		// Response : OK,#TEST1
#define SR_StartOnlineTestMode1Bank13			"#TEST1,13"		// Response : OK,#TEST1
#define SR_StartOnlineTestMode1Bank14			"#TEST1,14"		// Response : OK,#TEST1
#define SR_StartOnlineTestMode1Bank15			"#TEST1,15"		// Response : OK,#TEST1
#define SR_StartOnlineTestMode1Bank16			"#TEST1,16"		// Response : OK,#TEST1
#define SR_StartOnlineTestMode2				"#TEST2"		// Response : OK,#TEST2
#define SR_StartOnlineTestMode2Bank			"#TEST2,"		// + [ 01 ~ 16 : BankNumber ] Response : OK,#TEST2
#define SR_StartOnlineTestMode2Bank1			"#TEST2,01"		// Response : OK,#TEST2
#define SR_StartOnlineTestMode2Bank2			"#TEST2,02"		// Response : OK,#TEST2
#define SR_StartOnlineTestMode2Bank3			"#TEST2,03"		// Response : OK,#TEST2
#define SR_StartOnlineTestMode2Bank4			"#TEST2,04"		// Response : OK,#TEST2
#define SR_StartOnlineTestMode2Bank5			"#TEST2,05"		// Response : OK,#TEST2
#define SR_StartOnlineTestMode2Bank6			"#TEST2,06"		// Response : OK,#TEST2
#define SR_StartOnlineTestMode2Bank7			"#TEST2,07"		// Response : OK,#TEST2
#define SR_StartOnlineTestMode2Bank8			"#TEST2,08"		// Response : OK,#TEST2
#define SR_StartOnlineTestMode2Bank9			"#TEST2,09"		// Response : OK,#TEST2
#define SR_StartOnlineTestMode2Bank10			"#TEST2,10"		// Response : OK,#TEST2
#define SR_StartOnlineTestMode2Bank11			"#TEST2,11"		// Response : OK,#TEST2
#define SR_StartOnlineTestMode2Bank12			"#TEST2,12"		// Response : OK,#TEST2
#define SR_StartOnlineTestMode2Bank13			"#TEST2,13"		// Response : OK,#TEST2
#define SR_StartOnlineTestMode2Bank14			"#TEST2,14"		// Response : OK,#TEST2
#define SR_StartOnlineTestMode2Bank15			"#TEST2,15"		// Response : OK,#TEST2
#define SR_StartOnlineTestMode2Bank16			"#TEST2,16"		// Response : OK,#TEST2
#define SR_QuitOnlineTestMode					"#QUIT"			// Response : OK,#QUIT

// I/O terminal control
#define SR_CheckTheInputTerminalStatusIn1		"INCHK,1"		// Response : ON (The input1 terminal is turned on.), OFF (The input1 terminal is off.)
#define SR_CheckTheInputTerminalStatusIn2		"INCHK,2"		// Response : ON (The input2 terminal is turned on.), OFF (The input2 terminal is off.)
#define SR_OutputTerminal1TurnOn				"OUTON,1"		// Response : OK,OUTON
#define SR_OutputTerminal2TurnOn				"OUTON,2"		// Response : OK,OUTON
#define SR_OutputTerminal3TurnOn				"OUTON,3"		// Response : OK,OUTON
//#define SR_OutputTerminal4TurnOn				"OUT4ON"		// Response : OK
#define SR_OutputTerminalAllTurnOn				"ALLON"			// Response : OK,ALLON
#define SR_OutputTerminal1TurnOff				"OUTOFF,1"		// Response : OK,OUTOFF
#define SR_OutputTerminal2TurnOff				"OUTOFF,2"		// Response : OK,OUTOFF
#define SR_OutputTerminal3TurnOff				"OUTOFF,3"		// Response : OK,OUTOFF
//#define SR_OutputTerminal4TurnOff			"OUT4OFF"		// Response : OK
#define SR_OutputTerminalAllTurnOff			"ALLOFF"		// Response : OK,ALLOFF

// Resetting the SR - 1000 Series
#define SR_ResetSr1000w						"RESET"			// Response : OK

// Clearing the send buffer
#define SR_ClearSendBuffer						"BCLR"			// Response : OK

// Reading history check
#define SR_ReadingHistory						"NUM"			// Response : OK/NG/ERROR/STABLE/TIMING = a/b/c/d/e [a = Reading (verification) OK count, b = Verification NG count, c = Read ERROR count, d = Read STABLE count, e = Trigger input count : (0 to 65535)]
#define SR_ReadingHistoryBank					"NUMB"			// Response : OK,NUMB,b1,b2,....,b16,n : n is Trigger(Timing) input count

// Capture Image on Bank
#define SR_CaptureImageBank					"SHOT,"				// + [ 01 ~ 16 : BankNumber ] Response : OK
#define SR_CaptureImageBank1					"SHOT,01"			// Response : OK
#define SR_CaptureImageBank2					"SHOT,02"			// Response : OK
#define SR_CaptureImageBank3					"SHOT,03"			// Response : OK
#define SR_CaptureImageBank4					"SHOT,04"			// Response : OK
#define SR_CaptureImageBank5					"SHOT,05"			// Response : OK
#define SR_CaptureImageBank6					"SHOT,06"			// Response : OK
#define SR_CaptureImageBank7					"SHOT,07"			// Response : OK
#define SR_CaptureImageBank8					"SHOT,08"			// Response : OK
#define SR_CaptureImageBank9					"SHOT,09"			// Response : OK
#define SR_CaptureImageBank10					"SHOT,10"			// Response : OK
#define SR_CaptureImageBank11					"SHOT,11"			// Response : OK
#define SR_CaptureImageBank12					"SHOT,12"			// Response : OK
#define SR_CaptureImageBank13					"SHOT,13"			// Response : OK
#define SR_CaptureImageBank14					"SHOT,14"			// Response : OK
#define SR_CaptureImageBank15					"SHOT,15"			// Response : OK
#define SR_CaptureImageBank16					"SHOT,16"			// Response : OK

// Forced control capture image
#define SR_CancelCaptureImage					"CANCEL"			// Response : OK,CANCEL

// Forced laser OFF/Resetting laser OFF
#define SR_ForcedLaserOff						"LOCK"			// Response : OK,LOCK
#define SR_ResetLaserOff						"UNLOCK"		// Response : OK,UNLOCK
#define SR_CheckLaserOffStatus					"RLOCK"			// Response : OK,RLOCK,m [m is LOCK(Forced laser off active) or UNLOCK (Forced laser off reset)]

// Control Pointer
#define SR_PointerOn							"AMON"			// Response : OK,AMON
#define SR_PointerOff							"AMOFF"			// Response : OK,AMOFF

// Set Time
#define SR_SetTime								"TMSET,"		// + [ YYYY=4Byte MM=2Byte DD=2Byte hh=2Byte mm=2Byte ss=2Byte ] Response : OK,TMSET
#define SR_GetTime								"TMGET"			// Response : OK,TMGET,t  : t is [ YYYY=4Byte MM=2Byte DD=2Byte hh=2Byte mm=2Byte ss=2Byte ] 

// Read script execute status
#define SR_GetTimeScriptExec					"SCPTIME"		// Response : OK,SCPTIME,t : t is [ now=Aus, max=Bus, min=Cus ] (A is Time, B is Max Time, C is Min Time)
#define SR_DebugScript							"SCPDBG,"		// + [ 0 : Debug Off , 1 : Debug On ] Response : OK,SCPDBG
#define SR_DebugScriptOn						"SCPDBG,1"		// Response : OK,SCPDBG
#define SR_DebugScriptOff						"SCPDBG,0"		// Response : OK,SCPDBG
#define SR_GetScriptError						"SCPERR"		// Response : OK,SCPERR,m : m is Error-String.
#define SR_GetScriptVersion					"SCPVER"		// Response : OK,SCPVER,m,n : m is version of script library. n is version of FmtSet.Lua.

// Initializing/saving/quitting the settings
#define SR_SaveSettingsInMemory				"SAVE"			// Response : OK,SAVE
#define SR_LoadSettingsInMemory				"LOAD"			// Response : OK,LOAD
#define SR_InitializeSettings					"DFLT"			// Response : OK,DFLT

// Others
#define SR_GetVersion							"KEYENCE"		// Response : OK,KEYENCE,SR-1000,v : v is version.
#define SR_GetCommandStatus					"CMDSTAT"		// Response : OK,CMDSTAT,m : m is [ none : 처리하지않음, wait: 설정 반영 대기, update: 갱신중 ].
#define SR_GetMacAddress						"EMAC"			// Response : OK,EMAC,n : n is MAC address (12byts).
#define SR_GetErrorStatus						"ERRSTAT"		// Response : OK,ERRSTAT,m : m is [ none : 에러없음, system: 시스템에러, update: 갱신에러, cfg: 설정값에러, ip: IP주소중복, over: 버퍼오버, plc: PLC링크에러, profinet: PROFINET에러, lua: 스크립트에러, hostconnect: 호스트접속에러 ].
#define SR_GetBusyStatus						"BUSYSTAT"		// Response : OK,BUSYSTAT,m : m is [ none : 처리하지않음, trg: TRG BUSY, update: 갱신처리중, file: 파일저장중, af: AF렌즈이동중 ].
#define SR_ClearErrorPlcLink					"PCLR"			// Response : OK,PCLR
#define SR_ClearErrorFtpComm					"HCLR"			// Response : OK,HCLR
#define SR_BackupConfig						"BSAVE,"		// + [ 1 ~ 8 : config1.ptc ~ config8.ptc ] Response : OK,BSAVE
#define SR_BackupConfig1						"BSAVE,1"		// Response : OK,BSAVE
#define SR_BackupConfig2						"BSAVE,2"		// Response : OK,BSAVE
#define SR_BackupConfig3						"BSAVE,3"		// Response : OK,BSAVE
#define SR_BackupConfig4						"BSAVE,4"		// Response : OK,BSAVE
#define SR_BackupConfig5						"BSAVE,5"		// Response : OK,BSAVE
#define SR_BackupConfig6						"BSAVE,6"		// Response : OK,BSAVE
#define SR_BackupConfig7						"BSAVE,7"		// Response : OK,BSAVE
#define SR_BackupConfig8						"BSAVE,8"		// Response : OK,BSAVE
#define SR_LoadBackupConfig					"BLOAD,"		// + [ 1 ~ 8 : config1.ptc ~ config8.ptc ] Response : OK,BLOAD
#define SR_LoadBackupConfig1					"BLOAD,1"		// Response : OK,BSAVE		
#define SR_LoadBackupConfig2					"BLOAD,2"		// Response : OK,BSAVE		
#define SR_LoadBackupConfig3					"BLOAD,3"		// Response : OK,BSAVE		
#define SR_LoadBackupConfig4					"BLOAD,4"		// Response : OK,BSAVE		
#define SR_LoadBackupConfig5					"BLOAD,5"		// Response : OK,BSAVE		
#define SR_LoadBackupConfig6					"BLOAD,6"		// Response : OK,BSAVE		
#define SR_LoadBackupConfig7					"BLOAD,7"		// Response : OK,BSAVE		
#define SR_LoadBackupConfig8					"BLOAD,8"		// Response : OK,BSAVE		

// Setting Command ====================================================================
/*
파라미터 뱅크 설정 커맨드					(WB/RB)
튜닝용 코드 설정 커맨드						(WC/RC)
동작 설정 커맨드							(WP/RP)
통신 설정 커맨드							(WN/RN)
설정/확인 커맨드의 일괄 송신				(WA/RA)

변경된 내용을 메모리에 저장					(SAVE)
변경전 내용으로 되돌리기					(RESET)
*/

//// Motor control
//#define MotorStop							"MOTOROFF"		// Response : OK
//#define CancelMotorStop					"MOTORON"		// Response : OK
//#define CheckMotorStatus					"RMOTOR"		// Response : MOTOROFF (Motor is stopped.), MOTORON (Motor is operating.)
//
//// Changing the laser scanning width (The maximum scanning angle is 60 degree.)
//#define SR_SetScanningStartAngle0			"SDEG0"			// + [ "0" to "400" (Unit: 0.1) ] : default is "0".  Response : OK
//#define SR_SetScanningStartAngle				"SDEG"			// + [ "0" to "400" (Unit: 0.1) ] : default is "0".  Response : OK
//#define CheckScanningStartAngle			"SDEG"			// Response : Degree [ "0" to "400" (Unit: 0.1) ]
//#define SR_SetScanningAngle					"WDEG"			// + [ "200" to "600" (Unit: 0.1) ] : default is "600".  Response : OK
//#define CheckScanningAngle				"WDEG"			// Response : Degree [ "200" to "600" (Unit: 0.1) ]

//// Starts/Finishes auto-trigger setting
//#define SR_StartAutoTrigger					"WTEACHON"		// Response : None
//#define FinishAutoTrigger					"WTEACHOFF"		// Response : WTEACHOK (Setting completed), WTEACHNG (Setting failed)

//// Shifting in or out of the setting mode
//#define SR_ShiftSettingMode					"SSET"			// Response : OK
//#define QuitSettingMode					"SEND"			// Response : OK


typedef enum _stSrCmd { SrNone=0, SrTriggerInputOn, SrTriggerInputOnBank, SrTriggerInputOnBank1, SrTriggerInputOnBank2, SrTriggerInputOnBank3, 
						SrTriggerInputOnBank4, SrTriggerInputOnBank5, SrTriggerInputOnBank6, SrTriggerInputOnBank7, SrTriggerInputOnBank8,
						SrTriggerInputOnBank9, SrTriggerInputOnBank10, SrTriggerInputOnBank11, SrTriggerInputOnBank12, SrTriggerInputOnBank13, 
						SrTriggerInputOnBank14, SrTriggerInputOnBank15, SrTriggerInputOnBank16, SrTriggerInputOff, SrAdjustFocus,
						SrTunningBank, SrTunningBank1, SrTunningBank2, SrTunningBank3, SrTunningBank4, SrTunningBank5, SrTunningBank6, 
						SrTunningBank7, SrTunningBank8, SrTunningBank9, SrTunningBank10, SrTunningBank11, SrTunningBank12, SrTunningBank13,
						SrTunningBank14, SrTunningBank15, SrTunningBank16, SrTunningOff, SrStartPresetDataRegistration,
						SrFinishPresetDataRegistartion, SrStartBatchSetBarcodeReading, SrQuitBatchSetBarcodeReading, SrCheckBatchSetBarcodeReadingStatus,
						SrStartReadingRatioTestMode, SrStartReadingRatioTestModeBank, SrStartReadingRatioTestModeBank1, SrStartReadingRatioTestModeBank2, 
						SrStartReadingRatioTestModeBank3, SrStartReadingRatioTestModeBank4, SrStartReadingRatioTestModeBank5, SrStartReadingRatioTestModeBank6, 
						SrStartReadingRatioTestModeBank7, SrStartReadingRatioTestModeBank8, SrStartReadingRatioTestModeBank9, SrStartReadingRatioTestModeBank10, 
						SrStartReadingRatioTestModeBank11, SrStartReadingRatioTestModeBank12, SrStartReadingRatioTestModeBank13, SrStartReadingRatioTestModeBank14, 
						SrStartReadingRatioTestModeBank15, SrStartReadingRatioTestModeBank16, SrStartTactMeasurementTestMode, SrStartTactMeasurementTestModeBank,
						SrStartTactMeasurementTestModeBank1, SrStartTactMeasurementTestModeBank2, SrStartTactMeasurementTestModeBank3, 
						SrStartTactMeasurementTestModeBank4, SrStartTactMeasurementTestModeBank5, SrStartTactMeasurementTestModeBank6,
						SrStartTactMeasurementTestModeBank7, SrStartTactMeasurementTestModeBank8, SrStartTactMeasurementTestModeBank9, 
						SrStartTactMeasurementTestModeBank10, SrStartTactMeasurementTestModeBank11, SrStartTactMeasurementTestModeBank12, 
						SrStartTactMeasurementTestModeBank13, SrStartTactMeasurementTestModeBank14, SrStartTactMeasurementTestModeBank15, 
						SrStartTactMeasurementTestModeBank16, SrQuitTestMode, SrStartOnlineTestMode1, SrStartOnlineTestMode1Bank, 
						SrStartOnlineTestMode1Bank1, SrStartOnlineTestMode1Bank2, SrStartOnlineTestMode1Bank3, SrStartOnlineTestMode1Bank4, 
						SrStartOnlineTestMode1Bank5, SrStartOnlineTestMode1Bank6, SrStartOnlineTestMode1Bank7, SrStartOnlineTestMode1Bank8, 
						SrStartOnlineTestMode1Bank9, SrStartOnlineTestMode1Bank10, SrStartOnlineTestMode1Bank11, SrStartOnlineTestMode1Bank12, 
						SrStartOnlineTestMode1Bank13, SrStartOnlineTestMode1Bank14, SrStartOnlineTestMode1Bank15, SrStartOnlineTestMode1Bank16,
						SrStartOnlineTestMode2Bank, SrStartOnlineTestMode2Bank1, SrStartOnlineTestMode2Bank2, SrStartOnlineTestMode2Bank3, 
						SrStartOnlineTestMode2Bank4, SrStartOnlineTestMode2Bank5, SrStartOnlineTestMode2Bank6, SrStartOnlineTestMode2Bank7, 
						SrStartOnlineTestMode2Bank8, SrStartOnlineTestMode2Bank9, SrStartOnlineTestMode2Bank10, SrStartOnlineTestMode2Bank11, 
						SrStartOnlineTestMode2Bank12, SrStartOnlineTestMode2Bank13, SrStartOnlineTestMode2Bank14, SrStartOnlineTestMode2Bank15, 
						SrStartOnlineTestMode2Bank16, SrQuitOnlineTestMode, SrCheckTheInputTerminalStatusIn1, SrCheckTheInputTerminalStatusIn2,
						SrOutputTerminal1TurnOn, SrOutputTerminal2TurnOn, SrOutputTerminal3TurnOn, SrOutputTerminalAllTurnOn, SrOutputTerminal1TurnOff,
						SrOutputTerminal2TurnOff, SrOutputTerminal3TurnOff, SrOutputTerminalAllTurnOff, SrResetSr1000w, SrClearSendBuffer, 
						SrReadingHistory, SrReadingHistoryBank, SrCaptureImageBank, SrCaptureImageBank1, SrCaptureImageBank2, SrCaptureImageBank3, 
						SrCaptureImageBank4, SrCaptureImageBank5, SrCaptureImageBank6, SrCaptureImageBank7, SrCaptureImageBank8, 
						SrCaptureImageBank9, SrCaptureImageBank10, SrCaptureImageBank11, SrCaptureImageBank12, SrCaptureImageBank13, SrCaptureImageBank14, 
						SrCaptureImageBank15, SrCaptureImageBank16, SrCancelCaptureImage, SrForcedLaserOff, SrResetLaserOff, SrCheckLaserOffStatus,
						SrPointerOn, SrPointerOff, SrSetTime, SrGetTime, SrGetTimeScriptExec, SrDebugScript, SrDebugScriptOn, SrDebugScriptOff,
						SrGetScriptError, SrGetScriptVersion, SrSaveSettingsInMemory, SrLoadSettingsInMemory, SrInitializeSettings, SrGetVersion, 
						SrGetCommandStatus, SrGetMacAddress, SrGetErrorStatus, SrGetBusyStatus, SrClearErrorPlcLink, SrClearErrorFtpComm, SrBackupConfig,
						SrBackupConfig1, SrBackupConfig2, SrBackupConfig3, SrBackupConfig4, SrBackupConfig5, SrBackupConfig6, SrBackupConfig7, SrBackupConfig8, 
						SrLoadBackupConfig, SrLoadBackupConfig1, SrLoadBackupConfig2, SrLoadBackupConfig3, SrLoadBackupConfig4, SrLoadBackupConfig5, 
						SrLoadBackupConfig6, SrLoadBackupConfig7, SrLoadBackupConfig8
						}_SrCmd;

// Command+CR
#ifndef CR
#define CR									0x0D			// Carriage Return
#endif
//===============================================================================









