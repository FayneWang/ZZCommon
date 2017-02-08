
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

	// ʹ��16λ��ֵ����¼�������Ĵ�С������Ѷ�ƫ��������ֵ��ǵĴ�С�󻺴�Ŀռ��С
	// ��������ʧ��
	if (m_iReaded+sizeof(int32_t) > m_pBuffer->m_iSize)
		return false;

	if (nullptr == pData)
	{
		// ������ȡָ���ַΪ��ʱ����ȡ���ݣ�����ָ������ݴ�С��
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
