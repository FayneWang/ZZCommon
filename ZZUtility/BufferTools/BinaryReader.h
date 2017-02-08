#pragma once

#include <stdint.h>
#include <string>
#include <ZZUtility/DLLDefines.h>

class CBuffer;
class _ZZUTILITY_EXTERN_ CBinaryReader
{
public:
	CBinaryReader(CBuffer *pBuffer = nullptr);
	~CBinaryReader(void);

	const int32_t & GetReadOffset();
	bool	OffsetReaded(int32_t iReaded);

	template<typename Type>
	bool	ReadType(Type *pVal);
	bool	ReadData(void *pData,int32_t *piReadSize);
	bool	ReadString(std::string *pstrVal);
	bool	ReadWString(std::wstring *pwstrVal);

	void	SetBuffer(CBuffer *pBuffer);

protected:

	int32_t	m_iReaded;
	CBuffer	*m_pBuffer;

private:
	bool	Read(void *pData,int32_t iReadSize);
};

template<typename Type>
bool CBinaryReader::ReadType(Type *pType)
{
	return Read(pType,sizeof(Type));
}