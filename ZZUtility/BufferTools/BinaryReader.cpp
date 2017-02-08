
#include <stdint.h>
#include <assert.h>
#include "Buffer.h"
#include "BinaryReader.h"


CBinaryReader::CBinaryReader(CBuffer *pBuffer) :
	m_pBuffer(pBuffer),
	m_iReaded(0)
{}

CBinaryReader::~CBinaryReader(void)
{
}

const int32_t & CBinaryReader::GetReadOffset()
{
	return m_iReaded;
}

bool CBinaryReader::OffsetReaded(int32_t iReaded)
{
	if (iReaded<0 || 
		m_pBuffer->m_iSize < iReaded)
		return false;

	m_pBuffer->m_iSize = iReaded;

	return true;
}

void CBinaryReader::SetBuffer(CBuffer *pBuffer)
{
	m_pBuffer = pBuffer;
	m_iReaded = 0;
}

bool CBinaryReader::ReadString(std::string *pstrVal)
{
	if (pstrVal == false)
		return false;

	int32_t uStringBytes = 0;
	memcpy(&uStringBytes,m_pBuffer->m_pData+m_iReaded,sizeof(int32_t));
	if ((m_iReaded+sizeof(int32_t)+uStringBytes) > m_pBuffer->m_iSize)
		return false;

	m_iReaded += sizeof(int32_t);
	char *pStringBegin = (char*)(m_pBuffer->m_pData+m_iReaded);
	pstrVal->assign(pStringBegin,pStringBegin+uStringBytes);
	m_iReaded += uStringBytes;

	return true;
}

bool CBinaryReader::ReadWString(std::wstring *pwstrVal)
{
	if (pwstrVal == false)
		return false;

	int32_t iStringBytes = 0;
	memcpy(&iStringBytes,m_pBuffer->m_pData+m_iReaded,sizeof(int32_t));
	if ((m_iReaded+sizeof(int32_t)+iStringBytes) > m_pBuffer->m_iSize)
		return false;

	m_iReaded += sizeof(int32_t);
	wchar_t *pStringBegin = (wchar_t*)(m_pBuffer->m_pData+m_iReaded);
	pwstrVal->assign(pStringBegin,pStringBegin+(iStringBytes>>1));
	m_iReaded += iStringBytes;

	return true;
}

bool CBinaryReader::ReadData(void *pData,int32_t *piReadSize)
{
	if (nullptr == piReadSize)
		return false;

	// 使用16位数值来记录数据量的大小，如果已读偏移量加数值标记的大小大缓存的空间大小
	// 量读数据失败
	if (m_iReaded+sizeof(int32_t) > m_pBuffer->m_iSize)
		return false;

	if (nullptr == pData)
	{
		// 数据提取指针地址为空时不提取数据，返回指向的数据大小。
		*piReadSize = *(int32_t*)(m_pBuffer->m_pData+m_iReaded);
		return true;
	}
	else
	{
		if ((m_iReaded+*piReadSize) > m_pBuffer->m_iSize)
			return false;
		
		int32_t iDataSize = 0;
		memcpy(&iDataSize, m_pBuffer->m_pData+m_iReaded, sizeof(int32_t));
		if (*piReadSize < iDataSize)
		{
			*piReadSize = iDataSize;
			return false;
		}

		m_iReaded += sizeof(iDataSize);
		memcpy(pData,m_pBuffer->m_pData+m_iReaded,iDataSize);
		m_iReaded += iDataSize;
		*piReadSize = iDataSize;
		return true;
	}

	return false;
}

bool CBinaryReader::Read(void *pData,int32_t iReadSize)
{
	if (pData == nullptr || iReadSize <= 0)
		return false;

	if (m_iReaded+iReadSize > m_pBuffer->m_iSize)
		return false;

	memcpy(pData,m_pBuffer->m_pData+m_iReaded,iReadSize);
	m_iReaded += iReadSize;
	return true;
}
