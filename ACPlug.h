/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2007 Adobe Systems Incorporated                       */
/* All Rights Reserved.                                            */
/*                                                                 */
/* NOTICE:  All information contained herein is, and remains the   */
/* property of Adobe Systems Incorporated and its suppliers, if    */
/* any.  The intellectual and technical concepts contained         */
/* herein are proprietary to Adobe Systems Incorporated and its    */
/* suppliers and may be covered by U.S. and Foreign Patents,       */
/* patents in process, and are protected by trade secret or        */
/* copyright law.  Dissemination of this information or            */
/* reproduction of this material is strictly forbidden unless      */
/* prior written permission is obtained from Adobe Systems         */
/* Incorporated.                                                   */
/*                                                                 */
/*******************************************************************/

/*
	ACPlug.h
*/

#pragma once

#define PF_TABLE_BITS	12
#define PF_TABLE_SZ_16	4096

#define PF_DEEP_COLOR_AWARE 1	// make sure we get 16bpc pixels; 
								// AE_Effect.h checks for this.

#include "AEConfig.h"

#ifdef AE_OS_WIN
	typedef unsigned short PixelType;
	#include <Windows.h>
#endif

#include "entry.h"
#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include "AE_Macros.h"
#include "Param_Utils.h"
//#include "AE_EffectCBSuites.h"
//#include "AE_GeneralPlug.h"
//#include "AEFX_ChannelDepthTpl.h"
#include "AEGP_SuiteHandler.h"


#include<string>
	const std::string
		StrName{ "ACPlug" },
		StrDescription{ "ÓÉPLAYONES-ARECHEN¿ª·¢\r Copyright 2019 PLAYONES.COM." },
		StrAmount_Param_Name{ "Amount" },
		StrFeather_Param_Name{ "Feather" },
		StrRoundness_Param_Name{ "Roundness" },
		StrSize_Param_Name{ "Size" };

/* Versioning information */

#define	MAJOR_VERSION	1
#define	MINOR_VERSION	0
#define	BUG_VERSION		0
#define	STAGE_VERSION	PF_Stage_DEVELOP
#define	BUILD_VERSION	1


/* Parameter defaults */
	const double
		AC_AMOUNT_MIN = -100.0,
		AC_AMOUNT_MAX = 100.0,
		AC_AMOUNT_DFLT = 0.0,
		AC_SIZE_RANGE_MIN = 0.0,
		AC_SIZE_RANGE_MAX = 250.0,
		AC_SIZE_MIN = 50.0,
		AC_SIZE_MAX = 150.0,
		AC_SIZE_DFLT = 100.0,
		AC_ROUNDNESS_MIN = 0.0,
		AC_ROUNDNESS_MAX = 100.0,
		AC_ROUNDNESS_DFLT = 0.0,
		AC_FEATHER_MIN = 0.0,
		AC_FEATHER_MAX = 100.0,
		AC_FEATHER_DFLT = 30.0;

#define	SKELETON_GAIN_MIN		0
#define	SKELETON_GAIN_MAX		100
#define	SKELETON_GAIN_DFLT		10

enum {
	AC_INPUT = 0,
	AC_AMOUNT,
	AC_SIZE,
	AC_ROUNDNESS,
	AC_FEATHER,
	AC_NUM_PARAMS
};

enum {
	AMOUNT_DISK_ID = 1,
	SIZE_DISK_ID,
	ROUNDNESS_DISK_ID,
	FEATHER_DISK_ID,
};

typedef struct ACInfo{
	PF_FpLong	AmountF;
	PF_FpLong	FeatherF;
	PF_FpLong	SizeF;
	PF_FpLong	RoundnessF;
	PF_FpLong	ellipse_aaF;
	PF_FpLong	ellipse_bbF;
	PF_FpLong	ellipse_aabbF;
	A_long  x_t;
	A_long  y_t;
	A_long  innerRect_aL;
	A_long  innerRect_bL;
	A_long  outerRect_aL;
	A_long  outerRect_bL;
	A_Boolean innerouterCheck;
} ACInfo;


extern "C" {

	DllExport
	PF_Err
	EffectMain(
		PF_Cmd			cmd,
		PF_InData		*in_data,
		PF_OutData		*out_data,
		PF_ParamDef		*params[],
		PF_LayerDef		*output,
		void			*extra);

}
