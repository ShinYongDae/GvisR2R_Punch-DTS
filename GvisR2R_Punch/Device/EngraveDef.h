// EngraveDef.h

#pragma once

typedef enum _stPcId {
	_Engrave = 0, _AoiUp, _AoiDn, _Punch, _Vs
}_PcId;

typedef enum _stCmdCode {
	_None_ = 0, _GetSig, _SetSig, _GetData, _SetData, _Rtn, _Status
}_CmdCode;

typedef enum _stSigInx {
	// Normal
	_Idle = 0, _Busy, _Connect,
	// Menu01
	__TempStop, __LotCut, _MkPosMv, _MkVerify, _ReelmapUp, _ReelmapDn, _ReelmapTot, 
	_RemainMode, _EndWork, _ReMk, _2Layer, _1LnMk, _2LnMk, _3LnMk, _4LnMk, _TotLnMk,
	// Menu02
	_2DEngLenMvCw, _2DEngLenMvCcw, _2DAoiLenMvCw, _2DAoiLenMvCcw, _2DMkLenMvCw, _2DMkLenMvCcw, _2DOneShotLenCw, _2DOneShotLenCcw,
	_EngBuffJogCw, _EngBuffJogCcw, _EngBuffOrgMv, _EngBuffInitPosMv, _EngBuffPosSave,
	// Menu03
	_Ready, _Run, _Reset, _Stop, _Auto, _OneCycle, _Manual, _MkTq, _AoiTq, _EngTq, _CcwModRe, _CcwModUn, _Core150Re, _Core150Un, 
	_Relation, _JoinSelRe, _MvCwRe, _MvCcwRe, _PrdChuckRe, _DancerUpRe, _PasteUpLfRe, _PasteUpRtRe, _PasteVacRe, _PprChuckRe,
	_PprCwRe, _PprCcwRe, _DoRe, _PrdPprRe,
	_JoinSelMk, _MvCwMk, _MvCcwMk, _FdVacMk, _PushUpMk, _TblBlwMk, _TblVacMk, _FdClampMk, _TensClampMk, _OnePnlMk, _LsrMk,
	_DancerUpMk, _TqVacMk,
	_JoinSelAoiDn, _MvCwAoiDn, _MvCcwAoiDn, _FdVacAoiDn, _PushUpAoiDn, _TblBlwAoiDn, _TblVacAoiDn, _FdClampAoiDn, _TensClampAoiDn, 
	_OnePnlAoiDn, _LsrAoiDn, _ClrRollAoiDn, _ClrSonicAoiDn,
	_TestAoiDn, _ResetAoiDn, _LotEndAoiDn,
	_JoinSelAoiUp, _MvCwAoiUp, _MvCcwAoiUp, _FdVacAoiUp, _PushUpAoiUp, _TblBlwAoiUp, _TblVacAoiUp, _FdClampAoiUp, _TensClampAoiUp,
	_OnePnlAoiUp, _LsrAoiUp, _ClrRollAoiUp,
	_TestAoiUp, _ResetAoiUp, _LotEndAoiUp,
	_JoinSelEng, _MvCwEng, _MvCcwEng, _FdVacEng, _PushUpEng, _TblBlwEng, _TblVacEng, _FdClampEng, _TensClampEng, _OnePnlEng, _LsrEng,
	_DancerUpEng, _ClrSonicEng,
	_JoinSelUn, _MvCwUn, _MvCcwUn, _PrdChuckUn, _DancerUpUn, _PasteUpLfUn, _PasteUpRtUn, _PasteVacUn, _PprChuckUn,
	_PprCwUn, _PprCcwUn, _ClrRollUpUn, _ClrRollPushUn,
	_EmgAoi,
	// Menu04
	_MkResetLf, _MkResetRt, _MkBuffJogCw, _MkBuffJogCcw, _MkBuffOrgMv, _MkBuffInitPosMv, _MkBuffPosSave,
}_SigInx;

typedef enum _stPageInx {
	_DlgHigh = 0, _Dlg01, _Dlg02, _Dlg03, _Dlg04, _Dlg05, _DlgInfo
}_PageInx;

typedef enum _stItemInx {
	// Frame High
	_MkFdDiffX = 0, _MkFdDiffY, _AoiDnFdDiffX, _AoiDnFdDiffY, _AoiUpFdDiffX, _AoiUpFdDiffY, _EgrFdDiffX, _EgrFdDiffY,
	// Dlg Info
	_DualTest, _SampleTest, _SampleShotNum, _TestMode, _RecoilerCcw, _UncoilerCcw, _AlignMethode,
	_DoorRecoiler, _DoorAoiUp, _DoorAoiDn, _DoorMk, _DoorEngrave, _DoorUncoiler, _SaftyMk, _CleannerAoiUp, _CleannerAoiDn, _UltraSonicAoiDn, _UltraSonicEngrave,
	_LmtTotYld, _LmtPatlYld, _StripOutRatio, _CustomNeedRatio, _NumRangeFixDef, _NumContFixDef, _UltraSonicStTim,
	// Menu01
	_OpName, _ModelUpName, _ModelDnName, _LotUpName, _LotDnName, _LayerUpName, _LayerDnName, _OrderNum, _ShotNum, _TotReelLen, _TotOpRto, _LotOpRto,
	_TotVel, _PartVel, _TempPause, _TempStopLen, _LotCut, _LotCutLen, _LotCutPosLen, _MkDoneLen, _AoiDnDoneLen, _AoiUpDoneLen, _LotSerial, //_MkNumLf, _MkNumRt,	
	_DefNumUp, _DefNumDn, _DefNumTot, _DefRtoUp, _DefRtoDn, _DefRtoTot, _GoodNumTot, _GoodNumUp, _GoodNumDn, _GoodRtoUp, 
	_GoodRtoDn, _GoodRtoTot, _TestNumUp, _TestNumDn, _TestNumTot, _MkVerfyLen, _LotStTime, _LotEdTime, _LotRunTime, 	
	_1LnGoodRtoUp, _1LnGoodRtoDn, _1LnGoodRtoTot, _2LnGoodRtoUp, _2LnGoodRtoDn, _2LnGoodRtoTot, _3LnGoodRtoUp, _3LnGoodRtoDn, 
	_3LnGoodRtoTot, _4LnGoodRtoUp, _4LnGoodRtoDn, _4LnGoodRtoTot, _AllLnGoodRtoUp, _AllLnGoodRtoDn, _AllLnGoodRtoTot, 	
	_DefNumOpen, _DefNumShort, _DefNumUshort, _DefNumLnW, _DefExtr, _DefNumProt, _DefNumEnick, _DefNumUdd1, _DefNumPhole, 
	_DefNumPad, _DefNumHopen, _DefNumHmiss, _DefNumHpos, _DefNumHdef, _DefNumEprot, _DefNumNrw, _DefNumLight, _DefNumNick, 
	_DefNumPoi, _DefNumVhOpen, _DefNumVhMiss, _DefNumVhPos, _DefNumVhd, _DefNumEspace, _DefNumWide,
	// Menu02
	_2DEngLen, _2DAoiLen, _2DMkLen, _2DMoveVel, _2DMoveAcc, _2DOneShotLen, _EngLeadPitch, _EngPushOffLen, _EngTqVal, _EngAoiLen,
	_EngFdDiffMax, _EngFdDiffRng, _EngFdDiffNum, _EngBuffInitPos, _EngBuffCurrPos,  
	// Menu04
	_FdVel, _FdAcc, _OnePnlLen, _OnePnlVel, _OnePnlAcc, _AoiLeadPitch, _MkLeadPitch, _FdDiffMax, _FdDiffRng, _FdDiffNum, 
	_AoiPushOffLen, _MkPushOffLen, _AoiTqVal, _MkTqVal, _AoiBuffShotNum, _AoiMkLen, _MkBuffInitPos, _MkBuffCurrPos,
	_MkNumLf, _MkNumRt, _MkMaxNumLf, _MkMaxNumRt,
	_MkInitPosLf, _MkInitVelLf, _MkInitAccLf, _MkFnlPosLf, _MkFnlVelLf, _MkFnlAccLf, _MkFnlTqLf,
	_MkHgtPosX1Lf, _MkHgtPosY1Lf, _MkHgtPosX2Lf, _MkHgtPosY2Lf, _MkHgtPosX3Lf, _MkHgtPosY3Lf, _MkHgtPosX4Lf, _MkHgtPosY4Lf,
	_MkInitPosRt, _MkInitVelRt, _MkInitAccRt, _MkFnlPosRt, _MkFnlVelRt, _MkFnlAccRt, _MkFnlTqRt,
	_MkHgtPosX1Rt, _MkHgtPosY1Rt, _MkHgtPosX2Rt, _MkHgtPosY2Rt, _MkHgtPosX3Rt, _MkHgtPosY3Rt, _MkHgtPosX4Rt, _MkHgtPosY4Rt,
}_ItemInx;

typedef enum _stStatusInx {
	_stErr = 0, _stReady, _stStop, _stRun, _stInitRun, _stSglSample, _stSglTest, _stDblSample, _stDblTest
}_StatusInx;

