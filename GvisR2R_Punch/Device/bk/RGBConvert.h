
#pragma once

#include "GenICam/Frame.h"
#include "Memory/Block.h"
#include "Media/imageconvert.h"
#include "GenICam/PixelType.h"

class FrameBuffer
{
private:
	uint8_t* Buffer_;

	int Width_;

	int Height_;

	int PaddingX_;

	int PaddingY_;

	int DataSize_;

	int PixelFormat_;

	uint64_t TimeStamp_;

	uint64_t BlockId_;

public:
	FrameBuffer(Dahua::GenICam::CFrame const& frame)
	{
		if(frame.getImageSize() > 0)
		{
			if(frame.getImagePixelFormat() == Dahua::GenICam::gvspPixelMono8)
			{
				Buffer_ = new(std::nothrow) uint8_t[frame.getImageSize()];
			}
			else
			{
				Buffer_ = new(std::nothrow) uint8_t[frame.getImageWidth() * frame.getImageHeight() * 3];
			}		
			if(Buffer_)
			{
				Width_ = frame.getImageWidth();
				Height_ = frame.getImageHeight();
				PaddingX_ = frame.getImagePadddingX();
				PaddingY_ = frame.getImagePadddingY();
				DataSize_ = frame.getImageSize();
				PixelFormat_ = frame.getImagePixelFormat();
				BlockId_ = frame.getBlockId();
			}
		}
	}

	~FrameBuffer()
	{
		if(Buffer_ != NULL)
		{
			delete[] Buffer_;
			Buffer_ = NULL;
		}
	}

	bool Valid()
	{
		if( NULL != Buffer_ )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	int Width()
	{
		return Width_;
	}

	int Height()
	{
		return Height_;
	}

	int PaddingX()
	{
		return PaddingX_;
	}

	int PaddingY()
	{
		return PaddingY_;
	}

	int DataSize()
	{
		return DataSize_;
	}

	uint64_t PixelFormat()
	{
		return PixelFormat_;
	}

	uint64_t TimeStamp()
	{
		return TimeStamp_;
	}

	void setWidth(uint32_t iWidth)
	{
		Width_ = iWidth;
	}

	void setPaddingX(uint32_t iPaddingX)
	{
		PaddingX_ = iPaddingX;
	}

	uint64_t BlockId()
	{
		return BlockId_;
	}

	void setPaddingY(uint32_t iPaddingX)
	{
		PaddingY_ = iPaddingX;
	}

	void setHeight(uint32_t iHeight)
	{
		Height_ = iHeight;
	}

	void setDataSize(int dataSize)
	{
		DataSize_ = dataSize;
	}

	void setPixelFormat(uint32_t pixelFormat)
	{
		PixelFormat_ = pixelFormat;
	}

	void setTimeStamp(uint64_t timeStamp)
	{
		TimeStamp_ = timeStamp;
	}

	uint8_t* bufPtr()
	{
		return Buffer_;
	}

};
typedef Dahua::Memory::TSharedPtr<FrameBuffer> FrameBufferSPtr;

/// 转码函数
extern bool ConvertToBGR24(const Dahua::GenICam::CFrame& input, FrameBufferSPtr& output);
