#pragma once

#include <ZZUtility/DLLDefines.h>

/**
 * @class CBuffer
 *
 * @brief
 * �ڴ滺�����࣬��Ҫ���ж��������ݻ���ʹ�ã��ṩ�ڴ滺��ʱʹ�õ�һЩ�����ӿڡ�
 */
class _ZZUTILITY_EXTERN_ CBuffer
{
#define BUFER_DEFAULT_SIZE 1024

	friend class CBinaryReader;
	friend class CBinaryWriter;

	friend class CStreamReader;
	friend class CStreamWriter;

public:

    /**
     * ���캯��
     * @param[in] uBlockSize    ��ʼ��ʱ��һ���ֽ��ڴ��Ĵ�С�������ֵ��Ϊ�������Ե���ʱ�Ĵ�С��
     */
    CBuffer(int iSize = BUFER_DEFAULT_SIZE,bool bInitBuffer = false);
    ~CBuffer(void);		

	bool InitBuffer();

    /**
     * ��ȡ�ڴ������ĵ�ַ
     * @return �����ڴ��ȡ�����ʼ��ַ������ڴ滺���������ʼ��ʧ��ʱ���ú������� NULL ��
     */
    char *Buffer() const;

	/**
	 * ��ȡ��ǰ�������ڴ�ռ�Ĵ�С
	 */
	const int & GetCapacity() const;
	
    /**
     * ���ݻ����������������ӻ������Ŀռ�
     * @param[in] uSize ���ӻ������ռ�Ĵ�С��
     * @return �������ռ����ӳɹ�ʱ����true,���򷵻�false.
     */
    bool IncreaseCapacity(int uSize);

    /**
     * �����ڴ滺�����Ĵ�С
     * @param[in] uSize ���õ��ڴ��С
     * @return ���óɹ�ʱ����true,���򷵻�false��
     */
    bool Reserve(int iSize = BUFER_DEFAULT_SIZE);

protected:
	mutable char *	m_pData;
	int			m_iSize;

private:
};

