#include "Buffer.h"
#include "StreamWriter.h"


CStreamWriter::CStreamWriter(CBuffer *pBuffer, int32_t iWrittenSize, 
	int32_t iIncreaseSize) : CBinaryWriter(pBuffer,iWrittenSize,iIncreaseSize)
{
}

CStreamWriter::~CStreamWriter(void)
{
}

bool CStreamWriter::BeginWrite()
{
	if (m_pBuffer == nullptr || 
		m_pBuffer->m_pData == nullptr || 
		m_pBuffer->m_iSize == 0)
		return false;

	m_iWritten = sizeof(int32_t);

	return true;
}

void CStreamWriter::EndWrite()
{
	if (m_pBuffer == nullptr || 
		m_pBuffer->m_pData == nullptr || 
		m_pBuffer->m_iSize == 0)
		return;

	*(int32_t*)m_pBuffer->m_pData = m_iWritten;
}
