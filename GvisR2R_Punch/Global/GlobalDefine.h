// GlobalDefine.h : interface of the CGvisR2RView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBALDEFINE_H_INCLUDED_)
#define AFX_GLOBALDEFINE_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define TEST_MODE		1


#ifdef TEST_MODE
	#define TEST_SHOT				2
	#define PATH_REELMAP			_T("C:\\R2RSet\\Test\\ReelMap.txt")
//#define PATH_REELMAP			_T("C:\\R2RSet\\Test\\ReelMapData.txt")
	#define PATH_REELMAP_UP			PATH_REELMAP	//_T("C:\\R2RSet\\Test\\ReelMapDataUp.txt")
	#define PATH_REELMAP_DN			PATH_REELMAP	//_T("C:\\R2RSet\\Test\\ReelMapDataDn.txt")
	#define PATH_REELMAP_ALL_UP		PATH_REELMAP	//_T("C:\\R2RSet\\Test\\ReelMapDataAllUp.txt")
	#define PATH_REELMAP_ALL_DN		PATH_REELMAP	//_T("C:\\R2RSet\\Test\\ReelMapDataAllDn.txt")

	#define PATH_STRIP_RGN			_T("C:\\R2RSet\\Test\\Layer.mst")			// CAM Strip Region
	#define PATH_PCS_RGN			_T("C:\\R2RSet\\Test\\Region.Pcs")			// AOI PCS Region
	#define PATH_CELL_RGN			_T("C:\\R2RSet\\Test\\Cell.rgn")			// CAM PCS Region
	#define PATH_CELL_MST			_T("C:\\R2RSet\\Test\\1STRIP-2-050.mst")	// CAM Cell Img Region Data
	#define PATH_PCS_IMG_			_T("C:\\R2RSet\\Test\\Piece.tif")
	#define PATH_CAD_IMG			_T("C:\\R2RSet\\Test\\CORGO1.tif")
	#define PATH_DEF_IMG			_T("C:\\R2RSet\\Test\\")
	#define PATH_PCR				_T("C:\\R2RSet\\Test\\0004.pcr")
	#define PATH_PIN_IMG_			_T("C:\\R2RSet\\Test\\Pin-330.tif")

	#define PATH_PIN_IMG			_T("C:\\R2RSet\\Test\\Pin-330.TIF")
	#define PATH_ALIGN0_IMG			_T("C:\\R2RSet\\Test\\Align0.TIF")
	#define PATH_ALIGN1_IMG			_T("C:\\R2RSet\\Test\\Align1.TIF")
	#define PATH_PCS_IMG			_T("C:\\R2RSet\\Test\\Piece.tif")

	#define	USE_MIL
	#define	USE_VISION
	#define	USE_TCPIP
	#define USE_ENGRAVE
	#define USE_DTS

	//#define USE_CAM_MASTER
	//#define	USE_NMC
	//#define	USE_MPE
	//#define	USE_IRAYPLE
	//#define	USE_SR1000W
	//#define	USE_LIGHT
	//#define	USE_SMAC

#else
	#define USE_CAM_MASTER
	#define	USE_NMC
	#define	USE_LIGHT
	#define	USE_MPE
	#define	USE_SMAC

	#define	USE_MIL
	#define USE_VISION
	#define	USE_IRAYPLE
//	#define	USE_SONY
//	#define	USE_IDS
//	#define USE_CREVIS
//	#define	USE_FLUCK
//	#define	USE_XMP

// 	#define	USE_TCPIP
// 	#define	USE_SR1000W
//	#define USE_ENGRAVE
//	#define USE_DTS

	#define PATH_PIN_IMG			_T("C:\\R2RSet\\Pin\\Pin.TIF")
	#define PATH_ALIGN0_IMG			_T("C:\\R2RSet\\Align\\Align0.TIF")
	#define PATH_ALIGN1_IMG			_T("C:\\R2RSet\\Align\\Align1.TIF")
	#define PATH_PCS_IMG			_T("C:\\R2RSet\\Pcs\\Piece.tif")
#endif


#define MAX_MS				6	
#define MAX_AXIS			4	
typedef enum {MS_X0=0, MS_Y0=1, MS_X1=2, MS_Y1=3, MS_X0Y0=4, MS_X1Y1=5} MS_ID; 
typedef enum {AXIS_X0=0, AXIS_Y0=1, AXIS_X1=2, AXIS_Y1=3} AXIS_ID; 
typedef enum {BTN_UP=1, BTN_DN=2} BTN_ACT; 
typedef enum {OFF=0, ON=1} SWITCH_ACT; 
typedef enum {TO_NONE=0x00, TO_LEFT=0x01, TO_TOP=0x02, TO_RIGHT=0x04, TO_BOTTOM=0x08} POS_DIR; 
typedef enum {M_CCW=-1, M_CW=1} MOTION_DIR; 
typedef enum {YEAR=0, MONTH=1, DAY=2, HOUR=3, MINUTE=4, SEC=5} TIME_PARAM;
typedef enum {UP=0, DN=1, ALL_UP=2, ALL_DN=3, ALL=4} SEL_MAP;
#define MAX_ENC_CNT			2147483648

// Language select
typedef enum {KOREAN=0, ENGLISH=1, JAPANESE=2} LANG; 

#define LANGUAGE	KOREAN

#define Pi		3.14159265358979

#define MAX_LIGHT_CH	4
#define MAX_VOICE_COIL	2

// Define Voice Coil ID & Cmd
#define VOICE_COIL_FIRST_CAM			0
#define VOICE_COIL_SECOND_CAM			1
#define FIRST_CAM_AND_SECOND_CAM		2

#define DELAY_TIME_MSG			2000
#define DELAY_TIME_ONE_MINUTE	6000

#define MB_TIME_OUT					-1
#define DEFAULT_TIME_OUT			120000		// [mSec]


#define ROT_CCW_90		-2
#define ROT_CCW_180		-1
#define ROT_NONE		0
#define ROT_CW_90		1
#define ROT_CW_180		2

#define PATH_MP2100_IO			_T("C:\\R2RSet\\IO.ini")
#define PATH_MP2100_SIGNAL		_T("C:\\R2RSet\\Signal.ini")
#define PATH_MP2100_DATA		_T("C:\\R2RSet\\Data.ini")
// #define PATH_MP2100_IO			_T("C:\\R2RSet\\Mp2100IoList.ini")
// #define PATH_SLICE_IO			_T("C:\\R2RSet\\SliceIoList.ini")
#define PATH_MOTION_PARAM		_T("C:\\R2RSet\\MotionParam.ini")
#define PATH_CONFIG				_T("C:\\R2RSet\\Config.ini")
#define PATH_WORKING_INFO		_T("C:\\R2RSet\\WorkingInfo.ini")
#define PATH_SMAC				_T("C:\\R2RSet\\SMAC.ini")
#define PATH_ALARM				_T("C:\\R2RSet\\Alarm.ini")

#define RGB_BLACK       RGB(0,0,0)
#define RGB_WHITE       RGB(255,255,255)
#define RGB_GRAY        RGB(0x80, 0x80, 0x80)
#define RGB_RED         RGB(255,0,0)
#define RGB_YELLOW      RGB(255,255,0)
#define RGB_DARKYELLOW  RGB(128,128,0)
#define RGB_GREEN       RGB(0,255,0)
#define RGB_LTGREEN     RGB(0,255,128)
#define RGB_DARKGREEN   RGB(64,128,128)
#define RGB_CYAN        RGB(0,255,255)
#define RGB_BLUE        RGB(0,0,255)
#define RGB_LTBLUE		RGB(82, 200, 200)
#define RGB_CLOUDBLUE	RGB(128,184,223)
#define RGB_MAGENTA     RGB(255,0,255)
#define RGB_LTMAGENTA   RGB(255,64,255)
#define RGB_DARKMAGENTA RGB(193,0,130)
#define RGB_LTGRAY      RGB(192,192,192)
#define RGB_DARKGRAY    RGB(128,128,128)
#define RGB_DARKCYAN    RGB(0,128,128)
#define RGB_LTCYAN      RGB(128,255,255)
#define RGB_BOON        RGB(255,0,128)
#define RGB_DARKBOON    RGB(64,0,128)
#define RGB_DARKBLUE    RGB(0,0,128)
#define RGB_PINK        RGB(255,0,128)
#define RGB_LTPINK      RGB(255,64,128)
#define RGB_SKYBLUE		RGB(128,255,255)
#define RGB_PURPLE		RGB(128,0,255)
#define RGB_LTPURPLE	RGB(226,233,251)
#define RGB_LTPURPLE2   RGB(255,128,255)
#define RGB_YELLOWGREEN RGB(128,255,0)
#define RGB_LTYELLOW    RGB(255,255,128)
#define RGB_BROWN       RGB(255,128,0)
#define RGB_CHESTNUT	RGB(128,64,64)
#define RGB_NAVY		RGB(0,0,128)
#define RGB_LLTGREEN    RGB(128,255,128)
#define RGB_LTRED       RGB(255,128,64)
#define RGB_DARKFINGER	RGB(128,0,128)
#define RGB_LAVENDER	RGB(0xe6,0xe6,0xfa)
#define RGB_PLURM		RGB(0xdd,0xa0,0xdd)

#define MRGB_BLACK       M_RGB888(0,0,0)
#define MRGB_WHITE       M_RGB888(255,255,255)
#define MRGB_GRAY        M_RGB888(0x80, 0x80, 0x80)
#define MRGB_RED         M_RGB888(255,0,0)
#define MRGB_YELLOW      M_RGB888(255,255,0)
#define MRGB_DARKYELLOW  M_RGB888(128,128,0)
#define MRGB_GREEN       M_RGB888(0,255,0)
#define MRGB_LTGREEN     M_RGB888(0,255,128)
#define MRGB_DARKGREEN   M_RGB888(0,64,64)
#define MRGB_CYAN        M_RGB888(0,255,255)
#define MRGB_BLUE        M_RGB888(0,0,255)
#define MRGB_MAGENTA     M_RGB888(255,0,255)
#define MRGB_DARKMAGENTA M_RGB888(193,0,130)
#define MRGB_LTGRAY      M_RGB888(192,192,192)
#define MRGB_DARKGRAY    M_RGB888(128,128,128)
#define MRGB_DARKCYAN    M_RGB888(0,128,128)
#define MRGB_LTCYAN      M_RGB888(128,255,255)
#define MRGB_BOON        M_RGB888(255,0,128)
#define MRGB_DARKBOON    M_RGB888(64,0,128)
#define MRGB_DARKBLUE    M_RGB888(0,0,128)
#define MRGB_DARKFINGER	 M_RGB888(128,0,128)

#define RGB_BLACK       RGB(0,0,0)
#define RGB_WHITE       RGB(255,255,255)
#define RGB_GRAY        RGB(0x80, 0x80, 0x80)
#define RGB_RED         RGB(255,0,0)
#define RGB_DARKRED     RGB(207,0,0)
#define RGB_YELLOW      RGB(255,255,0)
#define RGB_DARKYELLOW  RGB(128,128,0)
#define RGB_GREEN       RGB(0,255,0)
#define RGB_LTGREEN     RGB(0,255,128)
#define RGB_CYAN        RGB(0,255,255)
#define RGB_BLUE        RGB(0,0,255)
#define RGB_LTBLUE		RGB(82, 200, 200)
#define RGB_CLOUDBLUE	RGB(128,184,223)
#define RGB_MAGENTA     RGB(255,0,255)
#define RGB_DARKMAGENTA RGB(193,0,130)
#define RGB_WHGRAY      RGB(218,218,218)
#define RGB_LTGRAY      RGB(192,192,192)
#define RGB_DARKCYAN    RGB(0,128,128)
#define RGB_LTCYAN      RGB(128,255,255)
#define RGB_BOON        RGB(255,0,128)
#define RGB_DARKBOON    RGB(64,0,128)
#define RGB_DARKBLUE    RGB(0,0,128)
#define RGB_PINK        RGB(255,0,128)
#define RGB_DARKPINK	RGB(227,64,128)
#define RGB_SKYBLUE		RGB(128,255,255)
#define RGB_DKSKYBLUE	RGB(82,200,200)
#define RGB_PURPLE		RGB(128,0,255)
#define RGB_YELLOWGREEN RGB(128,255,0)
#define RGB_WHYELLOW    RGB(255,255,128)
#define RGB_ORANGE      RGB(255,128,0)
#define RGB_LTDKORANGE  RGB(255,172,89)
#define RGB_DARKORANGE  RGB(220,70,0)
#define RGB_CHESTNUT	RGB(128,64,64)
#define RGB_NAVY		RGB(0,0,128)
#define RGB_LLTGREEN    RGB(128,255,128)
#define RGB_LT_ORANGE   RGB(255,128,64)
#define RGB_DARKFINGER	RGB(128,0,128)
#define RGB_DLG_FRM		RGB(212,208,200)
#define RGB_DLG_FRM2	RGB(240,240,240)
#define RGB_DARKBROWN	RGB(64,0,0)
#define RGB_DIALOG		RGB(200,200,192)
#define RGB_POSTIT		RGB(240,233,134)
#define RGB_DARK_BLUE	RGB(68,136,204)
#define RGB_FRAME		RGB(212,208,200)

#define RGB_DARK_GREEN	RGB(0,125,60)
#define RGB_LT_PURPLE	RGB(179,179,255)
#define RGB_LT_YELLOW	RGB(255,255,150)
#define RGB_LT_PINK		RGB(255,200,200)
#define RGB_LT_GREEN	RGB(195,255,195)
#define RGB_LT_BLUE		RGB(210,255,255)
#define RGB_WH_ORANGE	RGB(255,220,190)


#define PATH_LOCAL_SPEC					_T("C:\\R2RSet\\LocalSpec")


#define MSG_KEYPAD_UP					_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Key_Up.bmp")
#define MSG_BTN_BK_DN					_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Btn_Bk_Dn.bmp")
#define MSG_BTN_BK_UP					_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Btn_Bk_Up.bmp")
#define ICO_BTN_CANCEL					_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Cancel3.ico")
#define ICO_BTN_EXIT					_T("C:\\R2RSet\\Pic\\DlgMyMsg\\EXIT2.ico")

#define MSG_SUB00_KEYPAD_UP				_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Sub00\\Key_Up.bmp")
#define MSG_SUB00_BTN_BK_DN				_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Sub00\\Btn_Bk_Dn.bmp")
#define MSG_SUB00_BTN_BK_UP				_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Sub00\\Btn_Bk_Up.bmp")
#define ICO_SUB00_BTN_CANCEL			_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Sub00\\Cancel3.ico")
#define ICO_SUB00_BTN_EXIT				_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Sub00\\EXIT2.ico")

#define MSG_SUB01_KEYPAD_UP				_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Sub01\\Key_Up.bmp")
#define MSG_SUB01_BTN_BK_DN				_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Sub01\\Btn_Bk_Dn.bmp")
#define MSG_SUB01_BTN_BK_UP				_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Sub01\\Btn_Bk_Up.bmp")
#define ICO_SUB01_BTN_CANCEL			_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Sub01\\Cancel3.ico")
#define ICO_SUB01_BTN_EXIT				_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Sub01\\EXIT2.ico")

#define MSG_SUB02_KEYPAD_UP				_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Sub02\\Key_Up.bmp")
#define MSG_SUB02_BTN_BK_DN				_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Sub02\\Btn_Bk_Dn.bmp")
#define MSG_SUB02_BTN_BK_UP				_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Sub02\\Btn_Bk_Up.bmp")
#define ICO_SUB02_BTN_CANCEL			_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Sub02\\Cancel3.ico")
#define ICO_SUB02_BTN_EXIT				_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Sub02\\EXIT2.ico")

#define ICO_CHK_DlgFrameHigh			_T("C:\\R2RSet\\Pic\\DlgFrameHigh\\button_ok.ico")
#define IMG_BK_DlgFrameHigh				_T("C:\\R2RSet\\Pic\\DlgFrameHigh\\bk.bmp")
#define IMG_BTN_DN_DlgFrameHigh			_T("C:\\R2RSet\\Pic\\DlgFrameHigh\\Btn_Bk_Dn.bmp")
#define IMG_BTN_UP_DlgFrameHigh			_T("C:\\R2RSet\\Pic\\DlgFrameHigh\\Btn_Bk_Up.bmp")
#define ICO_LED_BLU_DlgFrameHigh		_T("C:\\R2RSet\\Pic\\DlgFrameHigh\\3dBallBlue.ico")
#define ICO_LED_RED_DlgFrameHigh		_T("C:\\R2RSet\\Pic\\DlgFrameHigh\\3dBallRed.ico")
#define ICO_LED_GRY_DlgFrameHigh		_T("C:\\R2RSet\\Pic\\DlgFrameHigh\\3DBallGray.ico")

#define IMG_BTN_DN_DlgMenu01			_T("C:\\R2RSet\\Pic\\DlgMenu01\\Btn_Bk_Dn.bmp")
#define IMG_BTN_UP_DlgMenu01			_T("C:\\R2RSet\\Pic\\DlgMenu01\\Btn_Bk_Up.bmp")
#define NI_BTN_DN_DlgMenu01				_T("C:\\R2RSet\\Pic\\DlgMenu01\\Ni_Bk_Dn2.bmp")
#define NI_BTN_UP_DlgMenu01				_T("C:\\R2RSet\\Pic\\DlgMenu01\\Ni_Bk_Up1.bmp")

#define IMG_BTN_DN_DlgMenu02			_T("C:\\R2RSet\\Pic\\DlgMenu02\\Btn_Bk_Dn.bmp")
#define IMG_BTN_UP_DlgMenu02			_T("C:\\R2RSet\\Pic\\DlgMenu02\\Btn_Bk_Up.bmp")
#define IMG_JOG_DN_DlgMenu02			_T("C:\\R2RSet\\Pic\\DlgMenu02\\arrow_dn_blue.bmp")
#define IMG_JOG_UP_DlgMenu02			_T("C:\\R2RSet\\Pic\\DlgMenu02\\arrow_up_blue.bmp")
#define IMG_JOG_LF_DlgMenu02			_T("C:\\R2RSet\\Pic\\DlgMenu02\\arrow_lf_blue.BMP")
#define IMG_JOG_RT_DlgMenu02			_T("C:\\R2RSet\\Pic\\DlgMenu02\\arrow_rt_blue.BMP")

#define IMG_CIR_BLU_OFF_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_BLU_OFF.bmp")
#define IMG_CIR_BLU_ON_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_BLU_ON.bmp")
#define IMG_CIR_BLUE_OFF_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_BLUE_OFF.bmp")
#define IMG_CIR_BLUE_ON_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_BLUE_ON.bmp")
#define IMG_CIR_DGN_OFF_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_DGN_OFF.bmp")
#define IMG_CIR_DGN_ON_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_DGN_ON.bmp")
#define IMG_CIR_GRN_OFF_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_GRN_OFF.bmp")
#define IMG_CIR_GRN_ON_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_GRN_ON.bmp")
#define IMG_CIR_GRY_OFF_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_GRY_OFF.bmp")
#define IMG_CIR_GRY_ON_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_GRY_ON.bmp")
#define IMG_CIR_ORG_OFF_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_ORG_OFF.bmp")
#define IMG_CIR_ORG_ON_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_ORG_ON.bmp")
#define IMG_CIR_PNK_OFF_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_PNK_OFF.bmp")
#define IMG_CIR_PNK_ON_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_PNK_ON.bmp")
#define IMG_CIR_RED_OFF_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_RED_OFF.bmp")
#define IMG_CIR_RED_ON_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_RED_ON.bmp")
#define IMG_CIR_YLO_OFF_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_YLO_OFF.bmp")
#define IMG_CIR_YLO_ON_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_YLO_ON.bmp")

#define IMG_BTN_DN_DlgMenu04			_T("C:\\R2RSet\\Pic\\DlgMenu04\\Btn_Bk_Dn.bmp")
#define IMG_BTN_UP_DlgMenu04			_T("C:\\R2RSet\\Pic\\DlgMenu04\\Btn_Bk_Up.bmp")
#define IMG_JOG_UP_DlgMenu04			_T("C:\\R2RSet\\Pic\\DlgMenu04\\arrow_up_blue.bmp")
#define IMG_JOG_DN_DlgMenu04			_T("C:\\R2RSet\\Pic\\DlgMenu04\\arrow_dn_blue.bmp")
#define IMG_CIR_GRN_OFF_DlgMenu04		_T("C:\\R2RSet\\Pic\\DlgMenu04\\BTN_CIR_GRN_OFF.bmp")
#define IMG_CIR_GRN_ON_DlgMenu04		_T("C:\\R2RSet\\Pic\\DlgMenu04\\BTN_CIR_GRN_ON.bmp")

#define IMG_BTN_DN_DlgMyPassword		_T("C:\\R2RSet\\Pic\\DlgMyPassword\\Btn_Bk_Dn.bmp")
#define IMG_BTN_UP_DlgMyPassword		_T("C:\\R2RSet\\Pic\\DlgMyPassword\\Btn_Bk_Up.bmp")
#define IMG_BK_LT_YELLOW				_T("C:\\R2RSet\\Pic\\DlgMyPassword\\bk_light_yellow.bmp")
#define IMG_BK_LT_RED					_T("C:\\R2RSet\\Pic\\DlgMyPassword\\bk_light_red.bmp")

#define IMG_BTN_DN_DlgUtil02			_T("C:\\R2RSet\\Pic\\DlgUtil02\\Btn_Bk_Dn.bmp")
#define IMG_BTN_UP_DlgUtil02			_T("C:\\R2RSet\\Pic\\DlgUtil02\\Btn_Bk_Up.bmp")
#define IMG_JOG_DN_DlgUtil02			_T("C:\\R2RSet\\Pic\\DlgUtil02\\arrow_dn_blue.bmp")
#define IMG_JOG_UP_DlgUtil02			_T("C:\\R2RSet\\Pic\\DlgUtil02\\arrow_up_blue.bmp")
#define IMG_JOG_LF_DlgUtil02			_T("C:\\R2RSet\\Pic\\DlgUtil02\\arrow_lf_blue.BMP")
#define IMG_JOG_RT_DlgUtil02			_T("C:\\R2RSet\\Pic\\DlgUtil02\\arrow_rt_blue.BMP")

#define IMG_BTN_DN_DlgUtil03			_T("C:\\R2RSet\\Pic\\DlgUtil03\\Btn_Bk_Dn.bmp")
#define IMG_BTN_UP_DlgUtil03			_T("C:\\R2RSet\\Pic\\DlgUtil03\\Btn_Bk_Up.bmp")

#define IMG_BTN_DN_DlgUtil04			_T("C:\\R2RSet\\Pic\\DlgUtil04\\Btn_Bk_Dn.bmp")
#define IMG_BTN_UP_DlgUtil04			_T("C:\\R2RSet\\Pic\\DlgUtil04\\Btn_Bk_Up.bmp")
#define ICO_LED_GRY_DlgUtil04			_T("C:\\R2RSet\\Pic\\DlgUtil04\\3DBallGray.ico")
#define ICO_LED_BLU_DlgUtil04			_T("C:\\R2RSet\\Pic\\DlgUtil04\\3dBallBlue.ico")
#define ICO_LED_RED_DlgUtil04			_T("C:\\R2RSet\\Pic\\DlgUtil04\\3DBallRed.ico")

#define IMG_BTN_DN_DlgUtil05			_T("C:\\R2RSet\\Pic\\DlgUtil05\\Btn_Bk_Dn.bmp")
#define IMG_BTN_UP_DlgUtil05			_T("C:\\R2RSet\\Pic\\DlgUtil05\\Btn_Bk_Up.bmp")
#define ICO_LED_GRY_DlgUtil05			_T("C:\\R2RSet\\Pic\\DlgUtil05\\3DBallGray.ico")
#define ICO_LED_RED_DlgUtil05			_T("C:\\R2RSet\\Pic\\DlgUtil05\\3DBallRed.ico")

#define IMG_BTN_DN_DlgInfo				_T("C:\\R2RSet\\Pic\\DlgInfo\\Btn_Bk_Dn.bmp")
#define IMG_BTN_UP_DlgInfo				_T("C:\\R2RSet\\Pic\\DlgInfo\\Btn_Bk_Up.bmp")

#define IMG_BK_BLACK					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\bk_black.bmp")
#define IMG_KEYPAD_BK					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\background2.bmp")
#define IMG_KEYPAD_NUM0					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Num_0_Up.bmp")
#define IMG_KEYPAD_NUM1					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Num_1_Up.bmp")
#define IMG_KEYPAD_NUM2					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Num_2_Up.bmp")
#define IMG_KEYPAD_NUM3					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Num_3_Up.bmp")
#define IMG_KEYPAD_NUM4					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Num_4_Up.bmp")
#define IMG_KEYPAD_NUM5					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Num_5_Up.bmp")
#define IMG_KEYPAD_NUM6					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Num_6_Up.bmp")
#define IMG_KEYPAD_NUM7					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Num_7_Up.bmp")
#define IMG_KEYPAD_NUM8					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Num_8_Up.bmp")
#define IMG_KEYPAD_NUM9					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Num_9_Up.bmp")

#define IMG_KEYPAD_DEL					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Key_Del_Up.bmp")
#define IMG_KEYPAD_DOT					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Key_Dot_Up.bmp")
#define IMG_KEYPAD_ENTER				_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Key_Enter_Up.bmp")
#define IMG_KEYPAD_DN					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Key_Dn.bmp")
#define IMG_KEYPAD_UP					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Key_Up.bmp")
#define IMG_KEYPAD_ESC					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Key_Esc_Up.bmp")
#define IMG_KEYPAD_LEFT					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Key_Left_Up1.bmp")
#define IMG_KEYPAD_RIGHT				_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Key_Right_Up1.bmp")
#define IMG_KEYPAD_MINUS				_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Key_Minus_Up0.bmp")
#define IMG_KEYPAD_BS					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Key_Bs_Up.bmp")

#define IMG_BTN_UP_GRAY_L				_T("C:\\R2RSet\\Pic\\BtnImg\\UpGrayL.bmp")
#define IMG_BTN_UP_GRAY_S				_T("C:\\R2RSet\\Pic\\BtnImg\\UpGrayS.bmp")
#define IMG_BTN_UP_RED_L				_T("C:\\R2RSet\\Pic\\BtnImg\\UpRedL.bmp")
#define IMG_BTN_UP_RED_S				_T("C:\\R2RSet\\Pic\\BtnImg\\UpRedS.bmp")
#define IMG_BTN_UP_PURPLE_L				_T("C:\\R2RSet\\Pic\\BtnImg\\UpPurpleL.bmp")
#define IMG_BTN_UP_PURPLE_S				_T("C:\\R2RSet\\Pic\\BtnImg\\UpPurpleS.bmp")

#define IMG_BTN_DN_RED_L				_T("C:\\R2RSet\\Pic\\BtnImg\\DnRedL.bmp")
#define IMG_BTN_DN_RED_S				_T("C:\\R2RSet\\Pic\\BtnImg\\DnRedS.bmp")
#define IMG_BTN_DN_PURPLE_L				_T("C:\\R2RSet\\Pic\\BtnImg\\DnPurpleL.bmp")
#define IMG_BTN_DN_PURPLE_S				_T("C:\\R2RSet\\Pic\\BtnImg\\DnPurpleS.bmp")
#define IMG_BTN_DN_BLUE_L				_T("C:\\R2RSet\\Pic\\BtnImg\\DnBlueL.bmp")
#define IMG_BTN_DN_BLUE_S				_T("C:\\R2RSet\\Pic\\BtnImg\\DnBlueS.bmp")

#define WM_MYBTN_DOWN					(WM_USER+1)
#define WM_MYBTN_UP						(WM_USER+2)
#define WM_MYEDIT_VSCROLL				(WM_USER+3)
#define WM_MYNETWORK_END				(WM_USER+4)
#define WM_MYGROUP_REDRAW				(WM_USER+5)
#define WM_REFRESH						(WM_USER+6)
#define WM_MY_KEYPADNUM					(WM_USER+7)
#define WM_MYSTATIC_REDRAW				(WM_USER+8)
#define WM_MYSTATIC_MOUSE_MOVE			(WM_USER+9)

#define WM_DLG_INFO						(WM_USER+10)
#define WM_MY_IO						(WM_USER+11)
#define WM_DRAW_REELMAP					(WM_USER+12)
#define WM_DRAW_PCSIMG					(WM_USER+13)
#define WM_DRAW_PINIMG					(WM_USER+14)
#define WM_RECEIVE_RS232				(WM_USER+15)

#define WM_USER_INC_PROGRESS			(WM_USER +20)
#define WM_USER_TASK_COMPLETED			(WM_USER +21)
#define WM_USER_TASK_THREAD_COMPLETED	(WM_USER +22)

#define WM_MYMSG						(WM_USER+30)
#define WM_MYMSG00						(WM_USER+31)
#define WM_MYMSG01						(WM_USER+32)
#define WM_MYMSG02						(WM_USER+33)
#define WM_MYMSG_EXIT					(WM_USER+34)
//#define WM_MYMSG_RTN					(WM_USER+35)

#define WM_BUF_THREAD_DONE				(WM_USER+40)
#define WM_MY_PW						(WM_USER+41)


#define WM_CLIENT_RECEIVED				(WM_USER + 100)
#define WM_CLIENT_CLOSED				(WM_USER + 101)
#define WM_CLIENT_RECEIVED_MDX			(WM_USER + 102)
#define WM_CLIENT_RECEIVED_SR			(WM_USER + 103)

#define WM_SERVER_RECEIVED				(WM_USER + 110)
#define WM_SERVER_CLOSED				(WM_USER + 111)

#define WM_MYBTN_DBLCLK					(WM_USER + 120)


// Flow control flags

#define FC_DTRDSR       0x01
#define FC_RTSCTS       0x02
#define FC_XONXOFF      0x04

// ascii definitions

#define ASCII_BEL       0x07
#define ASCII_BS        0x08
#define ASCII_LF        0x0A
#define ASCII_CR        0x0D
#define ASCII_XON       0x11
#define ASCII_XOFF      0x13

#define DEF_UPPER					19

#define MAX_DEF						26
#define DEF_NONE					0		// NONE 	
#define DEF_NICK					1		// Nick 	
#define DEF_PROTRUSION				2		// Excessive line width
#define DEF_SPACE					3		// Minimum space violation
#define DEF_OPEN					4		// Pattern open
#define DEF_SHORT					5		// Pattern short
#define DEF_USHORT					6		// Micro short
#define DEF_PINHOLE					7		// Void, pinhole
#define DEF_HOLE_MISS				8		// Missing Hole
#define DEF_EXTRA					9		// Island
#define DEF_PAD						10		// Precision Pattern Error
#define DEF_HOLE_POSITION			11      // Location Hole 
#define DEF_POI						12		// Point of interest (Virtual defect by operator specfied)
#define DEF_VH_POSITION			    13		// Via hole positionning
#define DEF_VH_MISS					14		// Via hole missing
#define DEF_HOLE_DEFECT		    	15		// Hole burr..
#define DEF_HOLE_OPEN				16		// Hole open
#define DEF_VH_OPEN					17	    // VH open
#define DEF_VH_DEF					18	    // Reserved
#define DEF_EDGE_NICK				19		// Edge Nick
#define DEF_EDGE_PROT				20	    // Edge Protrusion
#define DEF_EDGE_SPACE				21	    // Edge Space
#define DEF_USER_DEFINE_1			22	    // User Define 1
#define DEF_NARROW					23	    // User Define 2
#define DEF_WIDE					24	    // User Define 3
#define DEF_LIGHT					25	

enum SAPP3_CODE{	SAPP3_OPEN = 0,
//					SAPP3_SHORT_USHORT = 1,
					SAPP3_SHORT = 1,
					SAPP3_NICK = 2,
					SAPP3_SPACE_EXTRA_PROTRUSION = 3,
//					SAPP3_PINHOLE_PAD = 4,
					SAPP3_PINHOLE = 4,
					SAPP3_HOPEN = 5,
					SAPP3_HMISS_HPOS_HBAD = 6,
					SAPP3_USHORT = 7,
					SAPP3_PAD = 8
				}	;
/*
enum SliceDI_0{	DI_ESTOP=0, DI_START=1, DI_FOOT=2, DI_READY=3, DI_VACUUM=4, 
				DI_FDOOR=7, DI_LDOOR=8, DI_RDOOR=9, DI_BDOOR=10 };

enum SliceDI_1{	DI_COVER1F=16, DI_COVER1B=17, DI_COVER2F=18, DI_COVER2B=19, DI_PD_ERROR=23, DI_LDI0=28, 
				DI_LDI1=29, DI_LDI2=30, DI_LDI3=31 };

enum SliceDO_2{	DO_STARTFLY=0, DO_STOPFLY=1, DO_LAMP1=2, DO_LAMP2=3, DO_LAMP3=4,
				DO_LAMP4=5, DO_LAMP5=6, DO_LAMP6=7, DO_STARTLAMP=8, DO_READYLAMP=9,
				DO_SOLSMOG=10, DO_SOL3=11, DO_SOLCOVER1=12, DO_SOLCOVER2=13 };

enum SliceDO_3{ DO_MC=16, DO_RINGBLOWER=17, DO_SOLPANEL=19, DO_TOWERR=20, DO_TOWERY=21, DO_TOWERG=22, 
				DO_DSP_PWR=25, DO_BUZZER=26, DO_LASER_OFF=27, DO_LDO0=28, DO_LDO1=29, DO_LDO2=30, DO_LDO3=31 };

enum SliceDO_4{ DO_INK_MK=0 };
*/
#define DLY_INK_MK			300

#define  MAX_PROCNODENUM			60    // Max ProcNodeNum : 600mm/2.5um = 48 
#define  MAX_MOTOR_SWATH_NUM		35    // MAX Motor swath num : 500mm/(8k*2.5um) = 25  
#define  MAX_CELL_NUM				(MAX_PROCNODENUM * MAX_MOTOR_SWATH_NUM) * 3    // 4500 ea, 3IPU


#define PATH_ERROR			-1
#define PATH_NOT_FOUND		0
#define PATH_IS_FILE		1
#define PATH_IS_FOLDER		2

#ifndef ID_NONE
#define ID_NONE			-1
#endif

#ifndef ID_SR1000W
#define ID_SR1000W		0
#endif

#ifndef ID_MDX2500
#define ID_MDX2500		1
#endif

#ifndef ID_PUNCH
#define ID_PUNCH		2
#endif

#ifndef ID_ENGRAVE
#define ID_ENGRAVE		2
#endif

typedef struct 
{
	int iStartX, iStartY;
	int iEndX, iEndY;
	int FMirror;	//0 : Defult 1 : Up to Down Mirroring  2 : Left to Right Mirroring
	int FRotate;	//0 : 0  1 : 90  2 : 180  3 : 270 [Degree]
} REGIONS_PIECE;

typedef struct {
	int nId;
	int iStartX, iStartY;
	int iEndX, iEndY;
	int FMirror;	//0 : 원본 1 : 상하미러  2 : 좌퓖E肩?
	int FRotate;	//0 : 0도  1 : 90도  2 : 180도  3 : 270도
	int Row;
	int Col;
} REGIONS_PIECE_2;


#define MYGL_GAP_PNL				5
#define MAX_DISP_PNL				6
#define MAX_PCR_PNL					24

// #define RGN_STRIP_VARIABLE_NUM	23		// REGION_STRIP 구조체에서 배열을 제외한 변수의 갯수
// #define MAX_NodeNumX			200
// #define MAX_NodeNumY			200
// #define MAX_NMSWATH				100
// 
// typedef struct {
// 	int nMode;											// 0 : Full Mode, 1 : Strip Mode
// 	int nMSwath;										// 모터가 움직일 회수.
// 	int NodeNumX, NodeNumY;								// 각 축당 셀 갯수
// 	int PanelPixelX, PanelPixelY;						// 전체 판넬 이미지의 크기
// 	int OvrXPix, OvrYPix;								// 각 축으로 오버랩되는 부분의 크기
// 	int ProcSizeX, ProcSizeY;							// 각 축당 셀의 크기
// 	int nCell;											// 전체 셀 갯수
// 	int nIPU;											// AOI의 IPU 갯수
// 	int CameraValidPixel;								// 실제 검사하기 위해 사용되는 이미지의 X축 크기
// 	int nScanMarginX, nScanMarginY;						// 실제 사용되는 이미지에 각 축방향으로 더해지는 여분 이미지의 크기 Ex) nScanMarginX = (8192 ? CameraValidPixel)/2
// 	int nRegionX, nRegionY;								// Reserved Data
// 	int nCellDivideX, nCellDivideY;						// 한 스트립당 각 축 방향 셀 갯수
// 	int nCADPinPosPixX, nCADPinPosPixY;					// Pin Position X, y
// 	int nProcMarginX, nProcMarginY;						// Reserved Data
// 	int StPosX[MAX_NodeNumX * MAX_NodeNumY];			// 각 셀의 왼쪽 윗부분의 X 위치
// 	int StPosY[MAX_NodeNumX * MAX_NodeNumY];			// 각 셀의 왼쪽 윗부분의 Y 위치
// 	int EdPosX[MAX_NodeNumX * MAX_NodeNumY];			// 각 셀의 오른쪽 아래부분의 X 위치
// 	int EdPosY[MAX_NodeNumX * MAX_NodeNumY];			// 각 셀의 오른쪽 아래부분의 Y 위치
// 	int XSwathPixPos[MAX_NMSWATH];						// 모터가 움직이기 시작하는 위치
// } REGION_STRIP;		//Total 100476 Byte


#define MENU01_STC_DEFINFO_HARF		6
#define MAX_PCR						4

#define RMAP_PCS_SCALE				0.85
#define TOT_M_IO					30

#define BUF_SZ						50

#define PNL_TOT						2500
#define PNLBUF_Y					50
#define PNLBUF_X					50

enum SEL_RMAP { RMAP_NONE=-1, RMAP_UP=0, RMAP_DN=1, RMAP_ALLUP=2, RMAP_ALLDN=3 };
enum SEL_CAM { CAM_LF=0, CAM_RT=1, CAM_BOTH=2 };
enum MAIN_BTN { MN_RST=0, MN_RDY=1, MN_STOP=2, MN_RUN=3 };

enum IMG_KIND { CAD_IMG=0, DEF_IMG=1 };
enum ALIGN_METHODE { ONE_POINT = 1, TWO_POINT, THREE_POINT, FOUR_POINT };

enum WORK_MODE { MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2 };



struct stSystem
{
	CString sMcName;
	CString sPathCamSpecDir;

	CString sPathAoiUp, sPathAoiUpCurrInfo, sPathAoiUpVrsData, sPathAoiUpOffset;
	CString sPathVrsShareUp, sPathVrsBufUp;
	CString sPathVsShareUp;
	CString sPathAoiDn, sPathAoiDnCurrInfo, sPathAoiDnVrsData, sPathAoiDnOffset;
	CString sPathVrsShareDn, sPathVrsBufDn;
	CString sPathVsShareDn;

	CString sPathOldFile;
	CString sPathSapp3;
	BOOL bSaveLog;
	BOOL bNoMk;	// 0 : 마킹모드, 1 : 비젼모드
	CString sReViewMkLen;
	BOOL bReViewMk;

	CString sMaxDispDefImg;
	CString sCamSn[2];

	CString sIpClient[3];	// ID_SR1000W, ID_MDX2500, ID_PUNCH
	CString sIpServer[3];	// ID_SR1000W, ID_MDX2500, ID_ENGRAVE
	CString sPort[3];		// ID_SR1000W, ID_MDX2500, ID_ENGRAVE(ID_PUNCH)
	BOOL bSaveMkImg;
	BOOL bStripPcsRgnBin;

	stSystem()
	{
		sMcName = _T("");
		sPathCamSpecDir = _T("");

		sPathAoiUp = _T(""); sPathAoiUpCurrInfo = _T("");
		sPathAoiUpOffset = _T(""); sPathAoiUpVrsData = _T("");
		sPathVrsShareUp = _T(""); sPathVrsBufUp = _T("");
		sPathVsShareUp = _T("");
		sPathAoiDn = _T(""); sPathAoiDnCurrInfo = _T("");
		sPathAoiDnOffset = _T(""); sPathAoiDnVrsData = _T("");
		sPathVrsShareDn = _T(""); sPathVrsBufDn = _T("");
		sPathVsShareDn = _T("");

		sPathOldFile = _T("");
		bSaveLog = FALSE;
		bNoMk = FALSE;	// 0 : 마킹모드, 1 : 비젼모드
		sReViewMkLen = _T("");
		bReViewMk = FALSE;
		sPathSapp3 = _T("");

		sCamSn[0] = _T(""); sCamSn[1] = _T("");
		sMaxDispDefImg = _T("");

		sIpClient[ID_SR1000W] = _T(""); sIpClient[ID_MDX2500] = _T(""); sIpClient[ID_PUNCH] = _T("");	// ID_SR1000W, ID_MDX2500, ID_PUNCH
		sIpServer[ID_SR1000W] = _T(""); sIpServer[ID_MDX2500] = _T(""); sIpServer[ID_ENGRAVE] = _T("");	// ID_SR1000W, ID_MDX2500, ID_ENGRAVE
		sPort[ID_SR1000W] = _T(""); sPort[ID_MDX2500] = _T(""); sPort[ID_ENGRAVE] = _T("");				// ID_SR1000W, ID_MDX2500, ID_ENGRAVE(ID_PUNCH)

		bSaveMkImg = FALSE;
		bStripPcsRgnBin = FALSE;
	}
};

struct stLastJob
{
	CString sProcessNum;
	CString sModelUp, sLayerUp, sLotUp, sSerialUp, sCompletedSerialUp;
	CString sModelDn, sLayerDn, sLotDn, sSerialDn, sCompletedSerialDn;

	CString sSelUserName, sReelTotLen, sOnePnlLen;
	BOOL bLotSep;
	CString sLotSepLen, sLotCutPosLen;
	BOOL bTempPause;	// [0]:AOI-Up , [1]:AOI-Dn
	int nMergingLayer;
	CString sTempPauseLen, sLmtTotYld, sLmtPatlYld;
	CString sStripOutRatio, sCustomNeedRatio;
	BOOL bContFixDef;
	CString sNumRangeFixDef, sNumContFixDef, sUltraSonicCleannerStTim;
	BOOL bRclDrSen, bMkDrSen, bBufDrSen, bAoiUpDrSen, bAoiDnDrSen, bEngvDrSen, bUclDrSen;
	BOOL bDispMkPcs, bStopFixDef, bMkSftySen, bAoiSftySen;
	CString sJogSpd, sLotSerial; //sLightVal, 
	BOOL bLightOn, bMkOnePnl, bAoiOnePnl, bEngraveOnePnl;
	BOOL bVerify, bReview;
	CString sVerifyLen;
	// 	CString sDistAoiMk;
	CString sFurgeNum, sFurgeRun;
	CString sMkLastShot;
	CString sAoiLastShot[2]; // [Up/Dn]
	CString sPartialSpd;
	BOOL bOneMetal, bTwoMetal;
	BOOL bDualTest, bSampleTest, nTestMode;
	BOOL bCore150Recoiler, bCore150Uncoiler;
	CString sSampleTestShotNum;
	BOOL bUse2Layer;
	CString sEngraveOrderNum, sEngraveLastShot;
	CString sEngraveOrgX, sEngraveOrgY, sEngravePosOffsetX, sEngravePosOffsetY, sEngravePosTheta;
	int nAlignMethode;
	BOOL bUseAoiUpCleanRoler, bUseAoiDnCleanRoler;
	BOOL bUseEngraveCleanner, bUseAoiDnCleanner;
	BOOL bEngraveCleanner, bAoiDnCleanner;

	stLastJob()
	{
		sProcessNum = _T("");
		sModelUp = _T(""); sLayerUp = _T(""); sLotUp = _T(""); sSerialUp = _T(""); sCompletedSerialUp = _T("");
		sModelDn = _T(""); sLayerDn = _T(""); sLotDn = _T(""); sSerialDn = _T(""); sCompletedSerialDn = _T("");

		sSelUserName = _T(""); sReelTotLen = _T(""); sOnePnlLen = _T("");
		bLotSep = FALSE;
		sLotSepLen = _T(""); sLotCutPosLen = _T("");
		bTempPause = FALSE;
		nMergingLayer = 0; 	// [0]:AOI-Up , [1]:AOI-Dn
		sTempPauseLen = _T(""); sLmtTotYld = _T(""); sLmtPatlYld = _T("");
		sStripOutRatio = _T("20.0"); // Minimum 20%
		sCustomNeedRatio = _T("");
		bContFixDef = FALSE;
		sNumRangeFixDef = _T("");
		sNumContFixDef = _T("");
		sUltraSonicCleannerStTim = _T("5.0"); // AOI_Dn : MW05940, AOI_Up : MW05942
		bRclDrSen = FALSE; bMkDrSen = FALSE; bBufDrSen = FALSE; bAoiUpDrSen = FALSE; bAoiDnDrSen = FALSE; bEngvDrSen = FALSE; bUclDrSen = FALSE;
		bDispMkPcs = FALSE; bStopFixDef = FALSE; bMkSftySen = FALSE; bAoiSftySen = FALSE;
		sJogSpd = _T(""); sLotSerial = _T(""); //sLightVal=""); 
		bLightOn = FALSE; bMkOnePnl = FALSE; bAoiOnePnl = FALSE; bEngraveOnePnl = FALSE;
		bVerify = FALSE; bReview = FALSE;
		sVerifyLen = _T("");
		// 		sDistAoiMk = _T("");
		sFurgeNum = _T(""); sFurgeRun = _T("");
		sMkLastShot = _T("0");
		sAoiLastShot[0] = _T("0");
		sAoiLastShot[1] = _T("0");
		sPartialSpd = _T("10");

		bOneMetal = FALSE; bTwoMetal = FALSE;
		bDualTest = TRUE; bSampleTest = FALSE; nTestMode = 0;
		bCore150Recoiler = FALSE; bCore150Uncoiler = FALSE;
		sSampleTestShotNum = _T("");
		bUseEngraveCleanner = FALSE; bUseAoiDnCleanner = FALSE;
		bEngraveCleanner = FALSE; bAoiDnCleanner = FALSE;

		bUse2Layer = FALSE;

		sEngraveOrderNum = _T(""); sEngraveLastShot = _T("");
		sEngraveOrgX = _T(""); sEngraveOrgY = _T(""); sEngravePosOffsetX = _T(""); sEngravePosOffsetY = _T(""); sEngravePosTheta = _T("");
		nAlignMethode = TWO_POINT;

		bUseAoiUpCleanRoler = FALSE; bUseAoiDnCleanRoler = FALSE;
	}
};

//struct stMotion
//{
//	BOOL bBufHomming;
//	CString sLmtFdAdjOffSet;	// Feeding량 Offset보정용 초과값설정. [mm]
//	CString sLmtFdOvrNum;		// Feeding량 연속 초과값 초과횟수 설정. [회]
//	CString sLmtFdErr;			// Feeding량 Offset보정(설비정지) 한계값 설정. [mm]
//	CString sMkTq;				// Tension Servo 0 Gain Torque Value. Plus Value is Feeding direction torque[Kgf].
//	CString sAoiTq;				// Tension Servo 0 Gain Torque Value. Plus Value is Feeding direction torque[Kgf].
//	CString sMkFdDist, sMkFdVel, sMkFdAcc;
//	CString sMkJogVel, sMkJogAcc;
//	CString sMkFdTotLen, sMkTotVel, sMkPatlVel; //
//	CString sAoiFdDist, sAoiFdVel, sAoiFdAcc;
//	CString sAoiJogVel, sAoiJogAcc;
//	CString sAoiFdTotLen, sAoiTotVel, sAoiPatlVel; //
//	CString sPinPosX[2], sPinPosY[2];
//	CString sStPosX[2], sStPosY[2];
//	CString sMkEdPosX[2], sMkEdPosY[2];
//	CString sStBufPos, sBufHomeSpd, sBufHomeAcc;
//	CString sMkFdLead, sAoiFdLead;
//	CString sFdInitDist, sFdAoiAoiDistShot;//, sBufStdPosDist;
//	BOOL bStNewPrdt, bMkTq, bAoiTq;
//	CString sMkFdVacOff, sAoiFdVacOff;
//	CString sSafeZone, sCollisionLength, sCollisionMargin;
//	CString sAlignResultPosX[2][2], sAlignResultPosY[2][2], sAlignResultTheta[2][2], sAlignResultScore[2][2]; // [Cam][Pos]
//
//	stMotion()
//	{
//		bBufHomming = FALSE;
//		sLmtFdAdjOffSet = _T("");		// Feeding량 Offset보정용 초과값설정. [mm]
//		sLmtFdOvrNum = _T("");		// Feeding량 연속 초과값 초과횟수 설정. [회]
//		sLmtFdErr = _T("");			// Feeding량 Offset보정(설비정지) 한계값 설정. [mm]
//		sMkTq = _T("");				// Tension Servo 0 Gain Torque Value. Plus Value is Feeding direction torque[Kgf].
//		sAoiTq = _T("");				// Tension Servo 0 Gain Torque Value. Plus Value is Feeding direction torque[Kgf].
//		sMkFdDist = _T(""); sMkFdVel = _T(""); sMkFdAcc = _T("");
//		sMkJogVel = _T(""); sMkJogAcc = _T("");
//		sMkFdTotLen = _T(""); sMkTotVel = _T(""); sMkPatlVel = _T("");
//		sAoiFdDist = _T(""); sAoiFdVel = _T(""); sAoiFdAcc = _T("");
//		sAoiJogVel = _T(""); sAoiJogAcc = _T("");
//		sAoiFdTotLen = _T(""); sAoiTotVel = _T(""); sAoiPatlVel = _T("");
//		sPinPosX[0] = _T(""); sPinPosY[0] = _T("");
//		sPinPosX[1] = _T(""); sPinPosY[1] = _T("");
//		sStPosX[0] = _T(""); sStPosY[0] = _T("");
//		sStPosX[1] = _T(""); sStPosY[1] = _T("");
//		sMkEdPosX[0] = _T(""); sMkEdPosY[0] = _T("");
//		sMkEdPosX[1] = _T(""); sMkEdPosY[1] = _T("");
//		sStBufPos = _T(""); sBufHomeSpd = _T(""); sBufHomeAcc = _T("");
//		sMkFdLead = _T(""); sAoiFdLead = _T("");
//		sFdInitDist = _T(""); sFdAoiAoiDistShot = _T("");//sBufStdPosDist="");
//		bStNewPrdt = FALSE; bMkTq = FALSE; bAoiTq = FALSE;
//		sMkFdVacOff = _T(""); sAoiFdVacOff = _T("");
//		sSafeZone = _T("");
//		sCollisionLength = _T("");
//		sCollisionMargin = _T("");
//
//		for (int k = 0; k < 2; k++)
//		{
//			for (int i = 0; i < 2; i++)
//			{
//				sAlignResultPosX[k][i] = _T("");
//				sAlignResultPosY[k][i] = _T("");
//				sAlignResultTheta[k][i] = _T("");
//				sAlignResultScore[k][i] = _T("");
//			}
//		}
//	}
//};

struct stMarking
{
	CString sWaitPos;
	CString sWaitVel;
	CString sWaitAcc;
	CString sMarkingPos;
	CString sMarkingVel;
	CString sMarkingAcc;
	CString sMarkingToq;
	CString sMarkingToqOffset;
	CString sMeasurePosX[4], sMeasurePosY[4];
	CString sAverDist;
	int nMkCnt;
	int nMkLimit;

	stMarking()
	{
		sWaitPos = _T("");
		sWaitPos = _T("");
		sWaitVel = _T("");
		sWaitAcc = _T("");
		sMarkingPos = _T("");
		sMarkingVel = _T("");
		sMarkingAcc = _T("");
		sMarkingToq = _T("");
		sMarkingToqOffset = _T("");

		for (int i = 0; i < 4; i++)
		{
			sMeasurePosX[i] = _T("");
			sMeasurePosY[i] = _T("");
		}

		sAverDist = _T("");

		nMkCnt = 0;
		nMkLimit = 0;
	}
};

struct stElecChk
{
	BOOL bUse, bStopOnOpen;
	CString sWaitPos;
	CString sWaitVel;
	CString sWaitAcc;
	CString sProbingPos;
	CString sProbingVel;
	CString sProbingAcc;
	CString sProbingToq;
	CString sMeasurePosX, sMeasurePosY;
	CString sRepeatTestNum;
	int nRepeatTest;

	stElecChk()
	{
		bUse = TRUE;
		bStopOnOpen = TRUE;
		sWaitPos = _T("");
		sWaitPos = _T("");
		sWaitVel = _T("");
		sWaitAcc = _T("");
		sProbingPos = _T("");
		sProbingVel = _T("");
		sProbingAcc = _T("");
		sProbingToq = _T("");

		sMeasurePosX = _T("");
		sMeasurePosY = _T("");
		sRepeatTestNum = _T("3");
		nRepeatTest = 0;
	}
};

struct stVocieCoil
{
	CString sPort, sBaudRate, sVal;
	BOOL bCr, bLf;
	CString sSmacHeightAtPos[4];
	CString sSmacAverageDist;
	//	CString sSmacFinalPos, sSmacFinalVel, sSmacFinalAcc, sSmacFinalToq;
	//	CString sSmacShiftPos, sSmacShiftVel, sSmacShiftAcc, sSmacShiftToq;
	stVocieCoil()
	{
		sPort = _T(""); sBaudRate = _T(""); sVal = _T("");
		bCr = FALSE; bLf = FALSE;
		sSmacAverageDist = _T("");

		for (int i = 0; i < 4; i++)
		{
			sSmacHeightAtPos[i] = _T("");
		}
	}
};

struct stLight
{
	CString sPort, sBaudRate, sCh[MAX_LIGHT_CH], sVal[MAX_LIGHT_CH];
	BOOL bCr, bLf;

	stLight()
	{
		sPort = _T(""); sBaudRate = _T("");
		bCr = FALSE; bLf = FALSE;
		for (int i = 0; i < MAX_LIGHT_CH; i++)
		{
			sCh[i] = _T("");
			sVal[i] = _T("");
		}
	}
};

struct stFluck
{
	BOOL bUse;
	CString sPort, sBaudRate;
	BOOL bCr, bLf;
	CString sThreshold;
	CString sRejectMkNum; // in Strip
	double dMeasPosX[2], dMeasPosY[2];

	stFluck()
	{
		bUse = FALSE;
		sPort = _T(""); sBaudRate = _T("");
		bCr = TRUE; bLf = FALSE;

		sThreshold = _T("");
		sRejectMkNum = _T("");

		dMeasPosX[0] = 0.0; dMeasPosY[0] = 0.0;
		dMeasPosX[1] = 0.0; dMeasPosY[1] = 0.0;
	}
};

struct stVision
{
	CString sCamPxlRes;
	CString sPinImgSz;
	CString sResX, sResY;
	CString sMkOffsetX, sMkOffsetY;
	CString sAlgnPosX[2], sAlgnPosY[2];
	CString sPtMatScr, sStdScr;

	stVision()
	{
		sCamPxlRes = _T("");
		sPinImgSz = _T("");
		sResX = _T(""); sResY = _T("");
		sMkOffsetX = _T(""); sMkOffsetY = _T("");
		sAlgnPosX[2] = _T(""); sAlgnPosY[2] = _T("");
		sPtMatScr = _T("");
		sPtMatScr = _T("");
	}
};

struct stLotTime
{
	int nYear, nMonth, nDay;
	int nHour, nMin, nSec;

	stLotTime()
	{
		nYear = 0; nMonth = 0; nDay = 0;
		nHour = 0; nMin = 0; nSec = 0;
	}
};

struct stLot
{
	stLotTime StTime, CurTime, EdTime;
	DWORD dwStTick;
	CString sTotalReelDist;
	CString sSeparateDist;
	CString sCuttingDist;
	CString sStopDist;

	stLot()
	{
		sTotalReelDist = _T("");
		sSeparateDist = _T("");
		sCuttingDist = _T("");
		sStopDist = _T("");
	}
};

#include "../Device/MotionParam.h"

struct stWorkingInfo
{
	stSystem System;
	stLastJob LastJob;
	stMotionMotion Motion;
	stMarking Marking[2];
	stVocieCoil VoiceCoil[2];
	stLight Light;
	stFluck Fluck;
	stVision Vision[2];
	stLot Lot;
	stElecChk Probing[2];
};

struct stYield
{
	int nTot, nGood, nDef;
	int nTotSriptOut;
	int nDefStrip[4], nDefA[MAX_DEF], nDefPerStrip[4][MAX_DEF], nStripOut[4];

	stYield()
	{
		nTot = 0; nGood = 0; nDef = 0;
		nTotSriptOut = 0;
		nDefStrip[0] = 0; nDefStrip[1] = 0; nDefStrip[2] = 0; nDefStrip[3] = 0;
		nStripOut[0] = 0; nStripOut[1] = 0; nStripOut[2] = 0; nStripOut[3] = 0;

		for (int k = 0; k < MAX_DEF; k++)
		{
			nDefA[k] = 0;

			for (int i = 0; i < 4; i++)
			{
				nDefPerStrip[i][k] = 0;
			}
		}
	}

	~stYield()
	{
		;
	}
};


// struct stMasterInfo
// {
// 	double dPixelSize;
// 	CString strMasterLocation, strCADImgPath, strCADImgBackUpPath, strTwoMetalOppLayer;
// 	int nImageCompression;
// 	BOOL bTwoMetalInspection;
// 
// 	stMasterInfo()
// 	{
// 		dPixelSize=0.0;
// 		strMasterLocation=""); strCADImgPath=""); strCADImgBackUpPath=""); strTwoMetalOppLayer="");
// 		nImageCompression=0;
// 		bTwoMetalInspection=FALSE;
// 	}
// };

struct stSliceIo
{
	int nMaxSeg, nInSeg, nOutSeg;
	CString **pAddrIn, **pAddrOut;

	stSliceIo()
	{
		nMaxSeg = 0; nInSeg = 0; nOutSeg = 0;
		pAddrIn = NULL;
		pAddrOut = NULL;
	}

	~stSliceIo()
	{
		if (pAddrIn)
		{
			for (int k = 0; k < nInSeg; k++)
				delete[] pAddrIn[k];
			delete[] pAddrIn;
			pAddrIn = NULL;
		}

		if (pAddrOut)
		{
			for (int k = 0; k < nOutSeg; k++)
				delete[] pAddrOut[k];
			delete[] pAddrOut;
			pAddrOut = NULL;
		}
	}
};

struct stMpeIo
{
	int nMaxSeg, nInSeg, nOutSeg;
	CString **pAddrIn, **pAddrOut;
	CString **pSymIn;

	int nGrpStep;
	int nGrpIn, nGrpOut;
	int nGrpInSt, nGrpOutSt;

	stMpeIo()
	{
		pAddrIn = NULL;
		pAddrOut = NULL;
		pSymIn = NULL;
	}

	~stMpeIo()
	{
		if (pAddrIn)
		{
			for (int k = 0; k < nInSeg; k++)
				delete[] pAddrIn[k];
			delete[] pAddrIn;
			pAddrIn = NULL;
		}

		if (pAddrOut)
		{
			for (int k = 0; k < nOutSeg; k++)
				delete[] pAddrOut[k];
			delete[] pAddrOut;
			pAddrOut = NULL;
		}

		if (pSymIn)
		{
			for (int k = 0; k < nInSeg; k++)
				delete[] pSymIn[k];
			delete[] pSymIn;
			pSymIn = NULL;
		}
	}
};

struct stMkIo
{
	// 	stSliceIo SliceIo;
	stMpeIo MpeIo, MpeSignal, MpeData;
};

struct stBtnMain
{
	BOOL Ready, Run, Reset, Stop, Auto, Manual;
	stBtnMain()
	{
		Ready = FALSE; Run = FALSE; Reset = FALSE; Stop = FALSE;
		Auto = FALSE; Manual = FALSE;
	}
};

struct stBtnTqMotor
{
	BOOL Mk, Aoi, Eng;
	stBtnTqMotor()
	{
		Mk = FALSE; Aoi = FALSE; Eng = FALSE;
	}
};

struct stBtnInductMotor
{
	BOOL Uc, Rc;
	stBtnInductMotor()
	{
		Uc = FALSE; Rc = FALSE;
	}
};

struct stBtnCore150
{
	BOOL Uc, Rc;
	stBtnCore150()
	{
		Uc = FALSE; Rc = FALSE;
	}
};

struct stBtnEtc
{
	BOOL BufR, EmgAoi;
	stBtnEtc()
	{
		BufR = FALSE; EmgAoi = FALSE;
	}
};

struct stBtnRecoiler
{
	BOOL Relation, FdCw, FdCcw, ReelChuck;
	BOOL DcRlUpDn, ReelJoinL, ReelJoinR, ReelJoinVac;
	BOOL PprChuck, PprCw, PprCcw, Rewine, RewineReelPpr;
	stBtnRecoiler()
	{
		Relation = FALSE; FdCw = FALSE; FdCcw = FALSE; ReelChuck = FALSE;
		DcRlUpDn = FALSE; ReelJoinL = FALSE; ReelJoinR = FALSE; ReelJoinVac = FALSE;
		PprChuck = FALSE; PprCw = FALSE; PprCcw = FALSE; Rewine = FALSE; RewineReelPpr = FALSE;
	}
};

struct stBtnPunch
{
	BOOL Relation, FdCw, FdCcw, FdVac;
	BOOL PushUp, TblBlw, TblVac, FdClp;
	BOOL TqClp, MvOne, LsrPt;
	BOOL DcRSol;
	stBtnPunch()
	{
		Relation = FALSE; FdCw = FALSE; FdCcw = FALSE; FdVac = FALSE;
		PushUp = FALSE; TblBlw = FALSE; TblVac = FALSE; FdClp = FALSE;
		TqClp = FALSE; MvOne = FALSE; LsrPt = FALSE; 
		DcRSol = FALSE;
	}
};

struct stBtnAOIDn
{
	BOOL Relation, FdCw, FdCcw, FdVac;
	BOOL PushUp, TblBlw, TblVac, FdClp;
	BOOL TqClp, MvOne, LsrPt;
	BOOL ClrRoll, SonicBlw, Test, Reset, LotEnd;
	stBtnAOIDn()
	{
		Relation = FALSE; FdCw = FALSE; FdCcw = FALSE; FdVac = FALSE;
		PushUp = FALSE; TblBlw = FALSE; TblVac = FALSE; FdClp = FALSE;
		TqClp = FALSE; MvOne = FALSE; LsrPt = FALSE; 
		ClrRoll = FALSE; SonicBlw = FALSE; Test = FALSE; Reset = FALSE; LotEnd = FALSE;
	}
};

struct stBtnAOIUp
{
	BOOL Relation, FdCw, FdCcw, FdVac;
	BOOL PushUp, TblBlw, TblVac, FdClp;
	BOOL TqClp, MvOne, LsrPt;
	BOOL ClrRoll, TqVac, Test, Reset, LotEnd;
	stBtnAOIUp()
	{
		Relation = FALSE; FdCw = FALSE; FdCcw = FALSE; FdVac = FALSE;
		PushUp = FALSE; TblBlw = FALSE; TblVac = FALSE; FdClp = FALSE;
		TqClp = FALSE; MvOne = FALSE; LsrPt = FALSE;
		ClrRoll = FALSE; TqVac = FALSE; Test = FALSE; Reset = FALSE; LotEnd = FALSE;
	}
};

struct stBtnEngrave
{
	BOOL Relation, FdCw, FdCcw, FdVac;
	BOOL PushUp, TblBlw, TblVac, FdClp;
	BOOL TqClp, MvOne, LsrPt;
	BOOL DcRSol, SonicBlw;
	stBtnEngrave()
	{
		Relation = FALSE; FdCw = FALSE; FdCcw = FALSE; FdVac = FALSE;
		PushUp = FALSE; TblBlw = FALSE; TblVac = FALSE; FdClp = FALSE;
		TqClp = FALSE; MvOne = FALSE; LsrPt = FALSE;
		DcRSol = FALSE; DcRSol = SonicBlw;
	}
};

struct stBtnUncoiler
{
	BOOL Relation, FdCw, FdCcw, ReelChuck;
	BOOL DcRlUpDn, ReelJoinL, ReelJoinR, ReelJoinVac;
	BOOL PprChuck, PprCw, PprCcw, ClRlUpDn, ClRlPshUpDn;
	stBtnUncoiler()
	{
		Relation = FALSE; FdCw = FALSE; FdCcw = FALSE; ReelChuck = FALSE;
		DcRlUpDn = FALSE; ReelJoinL = FALSE; ReelJoinR = FALSE; ReelJoinVac = FALSE;
		PprChuck = FALSE; PprCw = FALSE; PprCcw = FALSE; ClRlUpDn = FALSE; ClRlPshUpDn = FALSE;
	}
};

struct stBtnStatus
{
	stBtnMain Main;
	stBtnTqMotor Tq;
	stBtnInductMotor Induct;
	stBtnCore150 Core150;
	stBtnEtc Etc;
	stBtnRecoiler Rc;
	stBtnPunch Mk;
	stBtnAOIDn AoiDn;
	stBtnAOIUp AoiUp;
	stBtnEngrave Eng;
	stBtnUncoiler Uc;
};

typedef enum {
	DOOR_FL_MK = 0, DOOR_FR_MK = 1,
	DOOR_BL_MK = 2, DOOR_BR_MK = 3
}  DOOR_MK;
typedef enum {
	DOOR_FM_AOI_UP = 0, DOOR_FL_AOI_UP = 1, DOOR_FR_AOI_UP = 2,
	DOOR_BM_AOI_UP = 3, DOOR_BL_AOI_UP = 4, DOOR_BR_AOI_UP = 5
}  DOOR_AOI_UP;
typedef enum {
	DOOR_FM_AOI_DN = 6, DOOR_FL_AOI_DN = 7, DOOR_FR_AOI_DN = 8,
	DOOR_BM_AOI_DN = 9, DOOR_BL_AOI_DN = 10, DOOR_BR_AOI_DN = 11
}  DOOR_AOI_DN;
typedef enum {
	DOOR_FL_UC = 0, DOOR_FR_UC = 1,
	DOOR_BL_UC = 2, DOOR_BR_UC = 3
}  DOOR_UC;
typedef enum {
	DOOR_FL_RC = 0, DOOR_FR_RC = 1,
	DOOR_S_RC = 2, DOOR_BL_RC = 3, DOOR_BR_RC = 4
}  DOOR_RC;
typedef enum {
	DOOR_FL_ENGV = 0, DOOR_FR_ENGV = 1,
	DOOR_BL_ENGV = 2, DOOR_BR_ENGV = 3
}  DOOR_ENGV;//DOOR_S_ENGV = 2, 

typedef enum { EMG_M_MK = 0, EMG_B_MK = 1 }  EMG_MK;
typedef enum { EMG_F_AOI_UP = 0, EMG_B_AOI_UP = 1 }  EMG_AOI_UP;
typedef enum { EMG_F_AOI_DN = 2, EMG_B_AOI_DN = 3 }  EMG_AOI_DN;
typedef enum { LMT_NEG = 0, LMT_POS = 1 }  SENS_LIMIT;



struct stPcrShare
{
	BOOL bExist;
	int nSerial;
	CString sModel, sLayer, sLot, sPrcsCode;

	stPcrShare()
	{
		bExist = FALSE;
		nSerial = 0;
		sModel = _T(""); sLayer = _T(""); sLot = _T(""); sPrcsCode = _T("");
	}
};

struct stStatus
{
	BOOL bAuto, bManual, bOneCycle;								// Mode 스위치
	BOOL bSwJogLeft, bSwJogFast, bSwJogStep;								// Jog 판넬 선택 스위치
	BOOL bDoorMk[4], bDoorAoi[12];								// 도어 센서
	BOOL bDoorMkF[4], bDoorAoiF[12];							// 도어 센서
	BOOL bEmgMk[2], bEmgAoi[4];									// 비상정지 스위치
	BOOL bEmgUc, bEmgRc;										// 비상정지 스위치
	BOOL bEmgEngv[2], bEmgEngvF[2];								// 비상정지 스위치
	BOOL bEmgMkF[2], bEmgAoiF[4];								// 비상정지 스위치
	BOOL bEmgUcF, bEmgRcF;										// 비상정지 스위치
	BOOL bMainAirMk, bMainAirAoi;								// 메인 에어
	BOOL bSensTblVacMk, bSensTblVacAoi;							// 테이블 진공 센서
	BOOL bSensTblPrsMk, bSensTblPrsAoi;							// 테이블 압력 센서
	BOOL bSensSaftyMk, bSensSaftyAoi;							// 안전 센서
	BOOL bSensSaftyMkF, bSensSaftyAoiF;							// 안전 센서
	BOOL bSensLmtBufMk[2], bSensLmtBufRc[2], bSensLmtBufUc[2];	// 버퍼 롤러 리미트 센서
	BOOL bSigTestDoneAoi, bSigTblAirAoi;						// 검사부 신호
	BOOL bDoorUc[4], bDoorRe[5];								// 도어 센서
	BOOL bDoorUcF[4], bDoorReF[5];								// 도어 센서
	BOOL bDoorEngv[4], bDoorEngvF[4];							// 도어 센서

	stPcrShare PcrShare[2];

	stStatus()
	{
		bAuto = FALSE; bManual = FALSE; bOneCycle = FALSE; bSwJogLeft = FALSE; bSwJogFast = FALSE; bSwJogStep = FALSE;
		bDoorEngv[0] = FALSE; bDoorEngv[1] = FALSE; bDoorEngv[2] = FALSE; bDoorEngv[3] = FALSE;
		bDoorEngvF[0] = FALSE; bDoorEngvF[1] = FALSE; bDoorEngvF[2] = FALSE; bDoorEngvF[3] = FALSE;
		bDoorMk[0] = FALSE; bDoorMk[1] = FALSE; bDoorMk[2] = FALSE; bDoorMk[3] = FALSE;
		bDoorMkF[0] = FALSE; bDoorMkF[1] = FALSE; bDoorMkF[2] = FALSE; bDoorMkF[3] = FALSE;
		bDoorAoi[0] = FALSE; bDoorAoi[1] = FALSE; bDoorAoi[2] = FALSE; bDoorAoi[3] = FALSE; bDoorAoi[4] = FALSE; bDoorAoi[5] = FALSE;
		bDoorAoiF[0] = FALSE; bDoorAoiF[1] = FALSE; bDoorAoiF[2] = FALSE; bDoorAoiF[3] = FALSE; bDoorAoiF[4] = FALSE; bDoorAoiF[5] = FALSE;
		bEmgMk[0] = FALSE; bEmgMk[1] = FALSE;
		bEmgAoi[0] = FALSE; bEmgAoi[1] = FALSE;
		bEmgUc = FALSE; bEmgRc = FALSE;
		bEmgEngv[0] = FALSE; bEmgEngvF[0] = FALSE;
		bEmgEngv[1] = FALSE; bEmgEngvF[1] = FALSE;
		bEmgMkF[0] = FALSE; bEmgMkF[1] = FALSE;
		bEmgAoiF[0] = FALSE; bEmgAoiF[1] = FALSE;
		bEmgUcF = FALSE; bEmgRcF = FALSE;
		bMainAirMk = FALSE; bMainAirAoi = FALSE;
		bSensTblVacMk = FALSE; bSensTblVacAoi = FALSE;
		bSensTblPrsMk = FALSE; bSensTblPrsAoi = FALSE;
		bSensSaftyMk = FALSE; bSensSaftyAoi = FALSE;
		bSensSaftyMkF = FALSE; bSensSaftyAoiF = FALSE;
		bSensLmtBufMk[0] = FALSE; bSensLmtBufMk[1] = FALSE;
		bSensLmtBufUc[0] = FALSE; bSensLmtBufUc[1] = FALSE;
		bSensLmtBufRc[0] = FALSE; bSensLmtBufRc[1] = FALSE;
		bSigTestDoneAoi = FALSE; bSigTblAirAoi = FALSE;
		bDoorUc[0] = FALSE; bDoorUc[1] = FALSE; bDoorUc[2] = FALSE; bDoorUc[3] = FALSE;
		bDoorUcF[0] = FALSE; bDoorUcF[1] = FALSE; bDoorUcF[2] = FALSE; bDoorUcF[3] = FALSE;
		bDoorRe[0] = FALSE; bDoorRe[1] = FALSE; bDoorRe[2] = FALSE; bDoorRe[3] = FALSE; bDoorRe[4] = FALSE;
		bDoorReF[0] = FALSE; bDoorReF[1] = FALSE; bDoorReF[2] = FALSE; bDoorReF[3] = FALSE; bDoorReF[4] = FALSE;
	}
};


// struct stAlignMark
// {
// 	float X1, Y1;
// 	float X2, Y2;
// 
// 	stAlignMark()
// 	{
// 		X1=0.0; Y1=0.0;
// 		X2=0.0; Y2=0.0;
// 	}
// };

// struct stPieceMark
// {
// 	float X, Y;
// 
// 	stPieceMark()
// 	{
// 		X=0.0; Y=0.0;
// 	}
// };

struct stMpeIoWrite
{
	int nIdx;
	CString sAddr;
	long lData;

	stMpeIoWrite()
	{
		nIdx = -1;
		sAddr = _T("");
		lData = 0;
	}
};

struct stPcrMerge
{
	int nIdx, nIdxUp, nIdxDn;

	stPcrMerge()
	{
		nIdx = -1; nIdxUp = -1; nIdxDn = -1;
	}
};

struct stModelInfo
{
	CString sModel, sLayer, sLot;

	stModelInfo()
	{
		sModel = _T("");
		sLayer = _T("");
		sLot = _T("");
	}
};

struct stListBuf
{
	int nTot;
	int nSerial[BUF_SZ];

	stListBuf()
	{
		nTot = 0;
		for (int i = 0; i < BUF_SZ; i++)
			nSerial[BUF_SZ] = 0;
	}

	BOOL stListBuf::Push(int nS)
	{
		if (nS < 1 || (nTot + 1) > BUF_SZ)
			return FALSE;
		nSerial[nTot] = nS;
		nTot++;
		return TRUE;
	}

	int stListBuf::Pop()
	{
		if (nSerial[0] < 1 || (nTot - 1) < 0)
			return 0;

		int nS = nSerial[0];
		for (int i = 0; i < (nTot - 1); i++)
			nSerial[i] = nSerial[i + 1];
		nTot--;
		return nS;
	}

	int stListBuf::GetLast()
	{
		if (nSerial[0] < 1 || (nTot - 1) < 0)
			return 0;
		return nSerial[nTot - 1];
	}

	void stListBuf::Clear()
	{
		nTot = 0;
		for (int i = 0; i < BUF_SZ; i++)
			nSerial[i] = 0;
	}

};





#endif // !defined(AFX_GLOBALDEFINE_H_INCLUDED_)
