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

/*	ACPlug.cpp	

	This is a compiling husk of a project. Fill it in with interesting
	pixel processing code.
	
	Revision History

	Version		Change													Engineer	Date
	=======		======													========	======
	1.0			(seemed like a good idea at the time)					bbb			6/1/2002

	1.0			Okay, I'm leaving the version at 1.0,					bbb			2/15/2006
				for obvious reasons; you're going to 
				copy these files directly! This is the
				first XCode version, though.

	1.0			Let's simplify this barebones sample					zal			11/11/2010

	1.0			Added new entry point									zal			9/18/2017

*/

#include "ACPlug.h"

static PF_Err 
About (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	AEGP_SuiteHandler suites(in_data->pica_basicP);
	
	suites.ANSICallbacksSuite1()->sprintf(	out_data->return_msg,
											"%s v%d.%d\r%s",
											StrName, 
											MAJOR_VERSION, 
											MINOR_VERSION, 
											StrDescription);
	return PF_Err_NONE;
}

static PF_Err 
GlobalSetup (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	out_data->my_version = PF_VERSION(	MAJOR_VERSION, 
										MINOR_VERSION,
										BUG_VERSION, 
										STAGE_VERSION, 
										BUILD_VERSION);

	out_data->out_flags =  PF_OutFlag_DEEP_COLOR_AWARE;	// just 16bpc, not 32bpc
	
	return PF_Err_NONE;
}

static PF_Err 
ParamsSetup (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err		err		= PF_Err_NONE;
	PF_ParamDef	def;	

	AEFX_CLR_STRUCT(def);

	PF_ADD_FLOAT_SLIDERX(	StrAmount_Param_Name.c_str(), 
							AC_AMOUNT_MIN,
							AC_AMOUNT_MAX,
							AC_AMOUNT_MIN,
							AC_AMOUNT_MAX,
							AC_AMOUNT_DFLT,
							PF_Precision_TENTHS,
							0,
							0,
							AMOUNT_DISK_ID);

	AEFX_CLR_STRUCT(def);

	PF_ADD_FLOAT_SLIDERX(StrSize_Param_Name.c_str(),
		AC_SIZE_RANGE_MIN,
		AC_SIZE_RANGE_MAX,
		AC_SIZE_MIN,
		AC_SIZE_MAX,
		AC_SIZE_DFLT,
		PF_Precision_TENTHS,
		0,
		0,
		SIZE_DISK_ID);

	AEFX_CLR_STRUCT(def);

	PF_ADD_FLOAT_SLIDERX(StrRoundness_Param_Name.c_str(),
		AC_ROUNDNESS_MIN,
		AC_ROUNDNESS_MAX,
		AC_ROUNDNESS_MIN,
		AC_ROUNDNESS_MAX,
		AC_ROUNDNESS_DFLT,
		PF_Precision_TENTHS,
		0,
		0,
		ROUNDNESS_DISK_ID);

	AEFX_CLR_STRUCT(def);

	PF_ADD_FLOAT_SLIDERX(StrFeather_Param_Name.c_str(),
		AC_FEATHER_MIN,
		AC_FEATHER_MAX,
		AC_FEATHER_MIN,
		AC_FEATHER_MAX,
		AC_FEATHER_DFLT,
		PF_Precision_TENTHS,
		0,
		0,
		FEATHER_DISK_ID);

	
	
	out_data->num_params = AC_NUM_PARAMS;

	return err;
}

static PF_Err
DrawAC16 (
	void		*refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err		err = PF_Err_NONE;

	return err;
}

static PF_Err
DrawEllipse8 (
	void		*refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err		err = PF_Err_NONE;
	A_long	x_tL, y_tL;
	ACInfo	*aiP	= reinterpret_cast<ACInfo*>(refcon);
	
	if (aiP){
		x_tL = xL - aiP->x_t;
		y_tL = yL - aiP->y_t;
		outP->alpha = inP->alpha;

		if (abs(x_tL)<aiP->innerRect_aL &&
			abs(y_tL)<aiP->innerRect_bL)
		{
			outP->red = 0;
			outP->green = 0;
			outP->blue = 0;
			return err;
		}

		if (aiP->innerouterCheck&&(
			y_tL > aiP->outerRect_bL ||
			y_tL < -aiP->outerRect_bL ||
			x_tL > aiP->outerRect_aL||
			x_tL< -aiP->outerRect_aL))
		{
			outP->red = PF_MAX_CHAN8;
			outP->green = PF_MAX_CHAN8;
			outP->blue = PF_MAX_CHAN8;
			return err;
		}

		if (x_tL*x_tL*aiP->ellipse_bbF + y_tL * y_tL * aiP->ellipse_aaF >= aiP->ellipse_aabbF) {
			
			outP->red = PF_MAX_CHAN8;
			outP->green = PF_MAX_CHAN8;
			outP->blue = PF_MAX_CHAN8;
		}
		else {
			outP->red = 0;
			outP->green = 0;
			outP->blue = 0;
		}
	}

	return err;
}

static PF_Err
DrawAC8(
	void		*refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8	*inP,
	PF_Pixel8	*outP)
{
	PF_Err		err = PF_Err_NONE;

	ACInfo	*aiP = reinterpret_cast<ACInfo*>(refcon);
	auto ClipSum8 = [](const A_u_char in, const PF_FpLong modF) {
		return static_cast<A_u_char>(MAX(0, MIN(modF + in, PF_MAX_CHAN8)));
	};

	if (aiP) {
		outP->alpha = inP->alpha;
		outP->red = ClipSum8(inP->red, aiP->AmountF);
		outP->green = ClipSum8(inP->green, aiP->AmountF);
		outP->blue = ClipSum8(inP->blue, aiP->AmountF);
	}

	return err;
}

static PF_Err 
Render (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err				err		= PF_Err_NONE;
	AEGP_SuiteHandler	suites(in_data->pica_basicP);

	/*	Put interesting code here. */
	ACInfo			aiP;
	AEFX_CLR_STRUCT(aiP);
	A_long				linesL	= 0;

	linesL 		= output->extent_hint.bottom - output->extent_hint.top;
	aiP.AmountF 	= params[AC_AMOUNT]->u.fs_d.value/100.0;
	aiP.ellipse_aaF 	= params[AC_SIZE]->u.fs_d.value  *  output->width/200.0;
	aiP.ellipse_bbF 	= params[AC_SIZE]->u.fs_d.value  *  output->height/200.0;
	aiP.ellipse_bbF 	= aiP.ellipse_aaF*params[AC_ROUNDNESS]->u.fs_d.value /100.0 +
						  aiP.ellipse_bbF*(100.0  - params[AC_ROUNDNESS]->u.fs_d.value) / 100.0;

	aiP.innerRect_aL = static_cast<A_u_long>(floor(aiP.ellipse_aaF / PF_SQRT2));
	aiP.innerRect_bL = static_cast<A_u_long>(floor(aiP.ellipse_bbF / PF_SQRT2));
	aiP.outerRect_aL = static_cast<A_u_long>(ceil(aiP.ellipse_aaF));
	aiP.outerRect_bL = static_cast<A_u_long>(ceil(aiP.ellipse_bbF));

	if (params[AC_SIZE]->u.fs_d.value >= 100.0)
	{
		aiP.innerouterCheck = false;
	}
	else {
		aiP.innerouterCheck = true;
	}

	aiP.ellipse_aaF *= aiP.ellipse_aaF;
	aiP.ellipse_bbF *= aiP.ellipse_bbF;
	aiP.ellipse_aabbF = aiP.ellipse_aaF * aiP.ellipse_bbF;

	aiP.x_t = output->width / 2;
	aiP.y_t = output->height  / 2;
	
	//if (PF_WORLD_IS_DEEP(output)){
	//	aiP.AmountF *= PF_MAX_CHAN16;
	//	ERR(suites.Iterate16Suite1()->iterate(	in_data,
	//											0,								// progress base
	//											linesL,							// progress final
	//											&params[AC_INPUT]->u.ld,	// src 
	//											NULL,							// area - null for all pixels
	//											(void*)&aiP,					// refcon - your custom data pointer
	//											DrawAC16,				// pixel function pointer
	//											output));
	//} else {
		aiP.AmountF *= PF_MAX_CHAN8;
		ERR(suites.Iterate8Suite1()->iterate(	in_data,
												0,								// progress base
												linesL,							// progress final
												&params[AC_INPUT]->u.ld,	// src 
												NULL,							// area - null for all pixels
												(void*)&aiP,					// refcon - your custom data pointer
												DrawEllipse8,
												/*DrawAC8,*/
												// pixel function pointer
												output));	
	/*}*/

	return err;
}


extern "C" DllExport
PF_Err PluginDataEntryFunction(
	PF_PluginDataPtr inPtr,
	PF_PluginDataCB inPluginDataCallBackPtr,
	SPBasicSuite* inSPBasicSuitePtr,
	const char* inHostName,
	const char* inHostVersion)
{
	PF_Err result = PF_Err_INVALID_CALLBACK;

	result = PF_REGISTER_EFFECT(
		inPtr,
		inPluginDataCallBackPtr,
		"ACPlug", // Name
		"ADBE ACPlug", // Match Name
		"Sample Plug-ins", // Category
		AE_RESERVED_INFO); // Reserved Info

	return result;
}


PF_Err
EffectMain(
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extra)
{
	PF_Err		err = PF_Err_NONE;
	
	try {
		switch (cmd) {
			case PF_Cmd_ABOUT:

				err = About(in_data,
							out_data,
							params,
							output);
				break;
				
			case PF_Cmd_GLOBAL_SETUP:

				err = GlobalSetup(	in_data,
									out_data,
									params,
									output);
				break;
				
			case PF_Cmd_PARAMS_SETUP:

				err = ParamsSetup(	in_data,
									out_data,
									params,
									output);
				break;
				
			case PF_Cmd_RENDER:

				err = Render(	in_data,
								out_data,
								params,
								output);
				break;
		}
	}
	catch(PF_Err &thrown_err){
		err = thrown_err;
	}
	return err;
}

