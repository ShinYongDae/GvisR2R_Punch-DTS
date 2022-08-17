#include "stdafx.h"

#include "RGBConvert.h"
#include "GenICam/PixelType.h"
#include "Media/ImageConvert.h"
#include <assert.h>
#include "Infra/Time.h"
using namespace Dahua::GenICam;

static uint32_t gFormatTransferTbl[] =
{	 
	// Mono Format
	gvspPixelMono1p,
	gvspPixelMono8,
	gvspPixelMono10,                
	gvspPixelMono10Packed,			
	gvspPixelMono12,			
	gvspPixelMono12Packed,

	// Bayer Format		
	gvspPixelBayRG8,                
	gvspPixelBayGB8,                
	gvspPixelBayBG8,                               
	gvspPixelBayRG10,               
	gvspPixelBayGB10,               
	gvspPixelBayBG10,                             
	gvspPixelBayRG12,               
	gvspPixelBayGB12,          
	gvspPixelBayBG12,                     
	gvspPixelBayRG10Packed,			
	gvspPixelBayGB10Packed,			
	gvspPixelBayBG10Packed,				        
	gvspPixelBayRG12Packed,			
	gvspPixelBayGB12Packed,			
	gvspPixelBayBG12Packed,			       
	gvspPixelBayRG16,               
	gvspPixelBayGB16,               
	gvspPixelBayBG16,               
	gvspPixelBayRG10p,              
	gvspPixelBayRG12p,             

	gvspPixelMono1c,

	// RGB Format
	gvspPixelRGB8,				
	gvspPixelBGR8,            

	// YVR Format
	gvspPixelYUV411_8_UYYVYY,     
	gvspPixelYUV422_8_UYVY,       
	gvspPixelYUV422_8,         
	gvspPixelYUV8_UYV,                       
};
#define gFormatTransferTblLen	sizeof(gFormatTransferTbl)/sizeof(gFormatTransferTbl[0])

struct ImplData
{
	int width;
	int height;
};

static int32_t findMatchCode(uint32_t code)
{
	for (int i = 0; i < gFormatTransferTblLen; ++i)
	{
		if (gFormatTransferTbl[i] == code)
		{
			return i;
		}
	}
	return -1;
}

static void freeMem(uint8_t* mem)
{
	if (NULL != mem)
		::free(mem);
}


// è½¬ç ?½æ•°
// transcoding function
bool ConvertToBGR24(const Dahua::GenICam::CFrame& input, FrameBufferSPtr& output)
{
	int idx = findMatchCode((input.getImagePixelFormat()));
	if (idx < 0)
	{
		return false;
	}

	FrameBufferSPtr PtrFrameBuffer(new FrameBuffer(input));
	if(!PtrFrameBuffer)
	{
		TRACE("PtrFrameBuffer is null.\n");
		return false;
	}

	// Mono8? é?è½¬æ¢?´æŽ¥æºæ•°??˜¾ç¤?
	// Mono8 does not need to convert direct source data display
	if(PtrFrameBuffer->PixelFormat() == gvspPixelMono8)
	{
		memcpy(PtrFrameBuffer->bufPtr(), input.getImage(), input.getImageSize());
	}
	else
	{
		uint8_t* pSrcData = new(std::nothrow) uint8_t[input.getImageSize()];
		if(pSrcData)
		{
			memcpy(pSrcData, input.getImage(), input.getImageSize());
		}
		else
		{
			TRACE("m_pSrcData is null.\n");
			return false;
		}

		int dstDataSize = 0;
		IMGCNV_SOpenParam openParam;
		openParam.width = PtrFrameBuffer->Width();
		openParam.height = PtrFrameBuffer->Height();
		openParam.paddingX = PtrFrameBuffer->PaddingX();
		openParam.paddingY = PtrFrameBuffer->PaddingY();
		openParam.dataSize = PtrFrameBuffer->DataSize();
		openParam.pixelForamt = PtrFrameBuffer->PixelFormat();

		IMGCNV_EErr status = IMGCNV_ConvertToBGR24(pSrcData, &openParam, PtrFrameBuffer->bufPtr(), &dstDataSize);
		if (IMGCNV_SUCCESS != status)
		{
			delete[] pSrcData;
			TRACE("IMGCNV_open is failed.err=%d\n",status);
			return false;
		}

		delete[] pSrcData;
	}
	
	output = PtrFrameBuffer;
	return true;
}