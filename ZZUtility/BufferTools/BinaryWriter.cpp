#include <string.h>
#include "Buffer.h"
#include "BinaryWriter.h"


CBinaryWriter::CBinaryWriter(CBuffer *pBuffer, int32_t iWritten,int32_t iIncreaseSize) : 
	m_pBuffer(pBuffer),
	m_iWritten(iWritten),
	m_iIncreaseSize(iIncreaseSize)
{
	if (pBuffer != nullptr)
		pBuffer->InitBuffer();
}

CBinaryWriter::~CBinaryWriter(void)
{}

char * CBinaryWriter::GetDataBuffer()
{
	if (m_pBuffer == nullptr)
		return nullptr;

	return m_pBuffer->m_pData;
}

CBuffer * CBinaryWriter::GetBuffer()
{
	return m_pBuffer;
}

const int32_t & CBinaryWriter::GetWritten()
{
	return m_iWritten;
}

bool CBinaryWriter::OffsetWrited(int32_t iWrittenSize,bool bSeekBegin)
{
	if (bSeekBegin)
	{
		if (iWrittenSize < 0)
			return false;

		if (m_pBuffer->m_iSize < iWrittenSize)
			return false;

		m_iWritten = iWrittenSize;
	}
	else
	{
		int32_t iOffset = m_iWritten;
		if (iWrittenSize > 0)
		{
			iOffset += iWrittenSize;
			if (iOffset > m_pBuffer->m_iSize)
				return false;
		}
		else
		{
			iOffset += iWrittenSize;
			if (iOffset < 0)
				return false;
		}

		m_iWritten = iOffset;
	}

	return true;
}

bool CBinaryWriter::ResetBuffer(CBuffer *pBuffer, int32_t iWrittenSize)
{
	if (iWrittenSize < 0 || pBuffer->m_iSize < iWrittenSize)
		return false;

	if (pBuffer != nullptr)
		pBuffer->InitBuffer();

	m_pBuffer = pBuffer;
	m_iWritten = iWrittenSize;

	return true;
}

bool CBinaryWriter::WriteData(const void *pData,int32_t iDataSize)
{
	if (pData==nullptr || iDataSize < 0)
		return false;

	if (!_AssertWriteCapacity(iDataSize + sizeof(int32_t)))
		return false;

	memcpy(m_pBuffer->m_pData+m_iWritten,&iDataSize,sizeof(int32_t));
	m_iWritten += sizeof(int32_t);

	memcpy(m_pBuffer->m_pData+m_iWritten,pData,iDataSize);
	m_iWritten += iDataSize;

	return true;
}

bool CBinaryWriter::WriteString(const std::string &strVal)
{
	return WriteData(strVal.c_str(), strVal.size());
}

bool CBinaryWriter::WriteWString(const std::wstring &wstrVal)
{
	return WriteData(wstrVal.c_str(), wstrVal.size()<<1);
}

bool CBinaryWriter::Write(const void *pData,int32_t iDataSize)
{
	if (pData == nullptr || 0 > iDataSize)
		return false;

	if (!_AssertWriteCapacity(iDataSize))
		return false;

	memcpy(m_pBuffer->m_pData+m_iWritten,pData,iDataSize);
	m_iWritten += iDataSize;

	return true;
}

bool CBinaryWriter::_AssertWriteCapacity(int32_t iWriteSize)
{
	if (m_iIncreaseSize == 0)
		return (m_iWritten + iWriteSize) <= m_pBuffer->m_iSize;

	int32_t iTemp = m_pBuffer->m_iSize - m_iWritten - iWriteSize;
	if (iTemp >= 0)		// ¼ì²é»º³åÇøÊÇ·ñ×ã¹»Ð´Èë¡£
	{
		return true;
	}

	iTemp = m_iWritten + iWriteSize;
	if (iTemp > INT32_MAX)
		return false;
	
	iTemp = (m_pBuffer->m_iSize+m_iIncreaseSize >= iTemp) ? 
		m_iIncreaseSize : iTemp-m_pBuffer->m_iSize;

	if (m_pBuffer->IncreaseCapacity(iTemp))
	{
		return true;
	}

	return false;
}

int32_t CBinaryWriter::GetFreeCapacity()
{
	return m_pBuffer->m_iSize - m_iWritten;
}
