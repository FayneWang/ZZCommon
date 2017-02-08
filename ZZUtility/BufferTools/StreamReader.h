#pragma once

#include <ZZUtility/BufferTools/BinaryReader.h>

class _ZZUTILITY_EXTERN_ CStreamReader : public CBinaryReader
{
public:
	CStreamReader(CBuffer *pBuffer);
	~CStreamReader(void);

	int32_t NextStream();
};
