
#include <stdlib.h>
#include <assert.h>
#include "Buffer.h"

CBuffer::CBuffer(int iSize /*= BUFER_DEFAULT_SIZE*/,bool bInitBuffer /*= false*/) :
	m_iSize(iSize > 0 ? iSize : 0),
	m_pData(nullptr)
{
    assert(iSize > 0);
    if (bInitBuffer)
        Reserve(m_iSize);
}

CBuffer::~CBuffer(void)
{
    free(m_pData);
}

char * CBuffer::Buffer() const
{
	if (m_pData == nullptr)
	{
		m_pData = (char*)malloc(m_iSize);
	}

    return m_pData;
}

bool CBuffer::InitBuffer()
{
	if (m_pData == nullptr)
	{
		m_pData = (char*)malloc(m_iSize);
	}

	return m_pData != nullptr;
}

const int & CBuffer::GetCapacity() const
{
	return m_iSize;
}

bool CBuffer::IncreaseCapacity(int iSize)
{
	if (iSize < 0)
		return false;

    int iIncreasedSize = m_iSize + iSize;
	if (iIncreasedSize < 0 )
		return false;

    char *pNewBuffer = (char*)realloc(m_pData,iIncreasedSize);
    if (nullptr == pNewBuffer)
        return false;

    m_pData = pNewBuffer;
    m_iSize = iIncreasedSize;
    return true;
}


bool CBuffer::Reserve(int iSize)
{
	if (iSize < 0)
		return false;

	if (m_iSize == iSize)
	{
		if (nullptr == m_pData)
			m_pData = (char*)malloc(m_iSize);
	}
	else
	{
		char *pNewBuffer = (char*)realloc(m_pData,iSize);
		if (nullptr == pNewBuffer)
			return false;

		m_pData = pNewBuffer;
		m_iSize = iSize;
	}

	return nullptr != m_pData;
}