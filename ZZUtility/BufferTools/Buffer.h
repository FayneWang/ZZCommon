#pragma once

#include <stdint.h>
#include <ZZUtility/DLLDefines.h>

/**
 * @class CBuffer
 *
 * @brief
 * 内存缓冲区类，主要进行二进制数据缓冲使用，提供内存缓冲时使用的一些便利接口。
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
     * 构造函数
     * @param[in] uBlockSize    初始化时，一个字节内存块的大小；这个块值作为缓冲区自递增时的大小。
     */
    CBuffer(int32_t iSize = BUFER_DEFAULT_SIZE,bool bInitBuffer = false);
    ~CBuffer(void);		

	bool InitBuffer();

    /**
     * 获取内存获冲区的地址
     * @return 返回内存获取冲的起始地址。如果内存缓冲区对象初始化失败时，该函数返回 NULL 。
     */
    char *Buffer() const;

	/**
	 * 获取当前缓冲区内存空间的大小
	 */
	const int32_t & GetCapacity() const;
	
    /**
     * 根据缓冲区对象递增块添加缓冲区的空间
     * @param[in] uSize 增加缓冲区空间的大小。
     * @return 缓冲区空间增加成功时返回true,否则返回false.
     */
    bool IncreaseCapacity(int32_t uSize);

    /**
     * 重置内存缓冲区的大小
     * @param[in] uSize 重置的内存大小
     * @return 调用成功时返回true,否则返回false。
     */
    bool Reserve(int32_t iSize = BUFER_DEFAULT_SIZE);

protected:
	mutable char *	m_pData;
	int32_t			m_iSize;

private:
};

