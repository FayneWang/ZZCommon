#include "StreamReader.h"


CStreamReader::CStreamReader(CBuffer *pBuffer) : CBinaryReader(pBuffer)
{
}


CStreamReader::~CStreamReader(void)
{
}

int32_t CStreamReader::NextStream()
{
	int32_t iStreamSize = 0;
	if(ReadType(&iStreamSize))
	{
		return iStreamSize;
	}

	return 0;
}
