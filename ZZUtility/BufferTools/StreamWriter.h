#pragma once

#include <ZZUtility/BufferTools/BinaryWriter.h>

class _ZZUTILITY_EXTERN_ CStreamWriter : public CBinaryWriter
{
public:
	CStreamWriter(CBuffer *pBuffer=nullptr, int32_t iWrittenSize=0, int32_t iIncreaseSize = 1024);
	~CStreamWriter(void);

	bool BeginWrite();
	void EndWrite();
};

