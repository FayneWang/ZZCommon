#pragma once

#include <stdint.h>
#include <string>
#include <ZZUtility/DLLDefines.h>

class CBuffer;
class _ZZUTILITY_EXTERN_ CBinaryWriter
{
public:
	CBinaryWriter(CBuffer *pBuffer=nullptr, int32_t iWritten=0, int32_t iIncreaseSize = 1024);
	~CBinaryWriter(void);

	char *GetDataBuffer();

	CBuffer *GetBuffer();
	const int32_t & GetWritten();

	bool OffsetWrited(int32_t iWrittenSize,bool bSeekBegin = true);

	bool ResetBuffer(CBuffer *pBuffer, int32_t iWrittenSize = 0);

	bool WriteData(const void *pData,int32_t iDataSize);

	bool WriteString(const std::string &strVal);

	bool WriteWString(const std::wstring &wstrVal);

	template<typename T>
	bool WriteType(const T &t);

	int32_t GetFreeCapacity();

	bool Write(const void *pData,int32_t iDataSize);

protected:
	CBuffer *m_pBuffer;
	int32_t	m_iWritten;
	int32_t	m_iIncreaseSize;

private:
	bool _AssertWriteCapacity(int32_t iWriteSize);


};

template<typename T>
bool CBinaryWriter::WriteType(const T &t)
{
	return Write(&t,sizeof(t));
}

