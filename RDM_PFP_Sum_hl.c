/*******************************************************************************
 * Copyright(C) 2017 Renesas Electronics Corporation.
 *
 *          RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 *
 * These instructions, statements, and programs are the confidential information
 * of Renesas Electronics Corporation. They must be used and modified solely for
 * the purpose for which it was furnished by Renesas Electronics Corporation.
 * All part of them must not be reproduced nor disclosed to others in any form,
 * without the prior written permission of Renesas Electronics Corporation.
 ******************************************************************************/

#include "pfp/RDM_PFP_Sum.h"

#include "common/RDM_internal.h"		/* T.B.D */
#include "pfp/RDM_PFP_internal.h"

/* **********************************************************
* Function name: RDM_PFP_Sum_hl(...)
* Description: Compute the sum of all elements
* Parameter output: XXX
* Parameter input: XXX
* Return value: None.
* **********************************************************/
void RDM_PFP_Sum_hl(
	RDM_PFP32* out, 
	const RDM_PFP_V16* in, 
	RAI_UINT num
)
{

/*
	total = 0;
	for (i = 0; i < num; i++)
	{
		total += (in.pValue[i] << in.pScale[i]);
	}
	(out.nValue, out.nScale) = total;

	*******************************************************
	Note:
	*******************************************************
	- Add all the elements and calculate the sum.
	- Compute N elements and output 1 result.
	- N is 16 or more, a multiple of 16.
	- The result rounds to 32 bits.

	See also PFP Function common rule.(RDM_PDP_Types.h)
*/

	/*
	* NOTE: this function is optimized for speed, input parameter checking is not performed.
	*/

	RAI_UINT dataIndex;

	const xb_vecNx16 *pvScaleIn;
	const xb_vecNx16 *pvDataIn;

	xb_vecNx40 vwSum;
	xb_vecNx16 vSumScale;

	pvScaleIn = (const xb_vecNx16 *)in->pScale;
	pvDataIn  = (const xb_vecNx16 *)in->pValue;


	/* dataIndex[0:(VEC_NUM-1)] */
	{
		/* load value(16bit -> 40bit) */
		{
			xb_vecNx16 vDataIn;

			BBE_LVNX16_IP(vDataIn, pvDataIn, sizeof(vDataIn));
			vwSum = BBE_UNPKSNX16(vDataIn);
		}
		/* load scale(16bit) */
		{
			BBE_LVNX16_IP(vSumScale, pvScaleIn, sizeof(xb_vecNx16));
		}
	}

	/* dataIndex[VEC_NUM:num] */
	for (dataIndex = RDM_VEC_NUM; dataIndex < num; dataIndex += RDM_VEC_NUM)
	{
		xb_vecNx16 vScaleIn;
		xb_vecNx40 vwDataIn;

		xb_vecNx40 vwOutput;
		xb_vecNx16 vScaleOut;

		/* load value(16bit -> 40bit) */
		{
			xb_vecNx16 vDataIn;

			BBE_LVNX16_IP(vDataIn, pvDataIn, sizeof(vDataIn));
			vwDataIn = BBE_UNPKSNX16(vDataIn);
		}

		/* load scale(16bit) */
		{
			BBE_LVNX16_IP(vScaleIn, pvScaleIn, sizeof(xb_vecNx16));
		}

		RDM_pfpAddElement_W(&vwSum, &vSumScale, &vwSum, &vSumScale, &vwDataIn, &vScaleIn);
	}

	{
		xb_int40 wSum;

		xb_vecNx16 vNormalizeScale;
		xb_vecNx16 vMaxScale;
	
		xb_vecNx40 vwAlignedSum;

		/* normalizing inputs to 34 bits (save sign & carry(4)) */
		{
			vNormalizeScale = BBE_MOVVVS(BBE_NSANX40(vwSum)) - BBE_MOVVA16(4);			/* vNormalizeScaleIn[-4,35] */

			vSumScale = vSumScale - vNormalizeScale;
		}

		{
			vboolN vbEqualToZero;
			vbEqualToZero = BBE_EQNX40(vwSum, BBE_MOVVA16(0));
			vSumScale = BBE_MOVNX16T(BBE_MOVVA16(RDM_PFP_SCALE_MIN), vSumScale, vbEqualToZero);
		}

		/* get MaxScale */
		{
			xb_int16 maxScale;
			maxScale = BBE_RMAXNX16(vSumScale);
			vMaxScale = xb_int16_rtor_xb_vecNx16(maxScale);
		}

		{
			xb_vecNx16 vScaleDiff;
			vScaleDiff = BBE_SUBSNX16(vMaxScale, vSumScale);							/* vScaleDiff[0,XX] */

			/* saturate vScaleDiff */
			{
				vsaN vsSaturationShift;

				vsSaturationShift = BBE_MOVVSV(BBE_MOVVA16(16-7), 0);
				vScaleDiff = BBE_SLSNX16(vScaleDiff, vsSaturationShift);
				vScaleDiff = BBE_SRSNX16(vScaleDiff, vsSaturationShift);				/* vScaleDiff[0,63] */
			}

			vwAlignedSum = BBE_SLSNX40(vwSum, BBE_MOVVSV(vNormalizeScale, 0));			/* vNormalizeScale[-4,35] */
			vwAlignedSum = BBE_SRSNX40(vwAlignedSum, BBE_MOVVSV(vScaleDiff, 0));		/* vScaleDiff[0,63] */
		}

		/* get Sum */
		wSum = BBE_RADDNX40(vwAlignedSum);
		
		/* Pack to 32bit */
		vwSum = xb_int40_rtor_xb_vecNx40(wSum);	
		RDM_pfpPack32(&vwSum, &vSumScale, &vwSum, &vMaxScale);

		out->nValue = BBE_MOVAW32(vwSum);
		out->nScale = BBE_MOVAV16(vSumScale);
	}
}
