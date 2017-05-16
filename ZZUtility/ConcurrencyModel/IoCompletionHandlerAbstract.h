#pragma once

/**
 * @file
 * @author wangzezhou <wangzezhou@163.com>
 * @date 2015-05-16
 * @brief 与完成端口模型封装类CIoCompletionPortModel关联时的基类抽象定义。
 */

#include <wtypes.h>
#include <ZZUtility/DLLDefines.h>

/**
 * @class CIoCompletionHandlerAbstract
 *
 * @breif 完成端口并发模型中的IO信号触发时，相应信号响应并处理的对象类的基类。
 *
 * 这个抽象类定义了与完成端口并发模型CIoCompletionPortModel类对象关联时所使用的抽象
 * 类型，其中定义了纯虚接口、可重定义纯接口和通用的公共接口。具体相关的各个接口介绍，
 * 请查看函数的详细说明。
 *
 */

class CIoCompletionHandlerAbstractPrivate;
class _ZZUTILITY_EXTERN_ CIoCompletionHandlerAbstract
{
    friend class CIoCompletionPortModel;
public:
    CIoCompletionHandlerAbstract(void);
    virtual ~CIoCompletionHandlerAbstract(void) = 0;

	/**
	 * 设置处理类对象被IoCompletionPortModel对象成功Detach()之后，自动销毁。
	 * 只有当对象成功被IoCompletionPortModel对象Attach()时，该设置才有效。
	 *
	 * @warning 对象必须是使用new创建的对象才能使用该函数设置为TRUE。
	 * 
	 * @param[in] bAutoDelete AutoDelete默认为FALSE，当设置AutoDelete为TRUE时，在
	 *	被完成端口模型对象Detach()之后当前对象自动delete。
	 */
    void SetAutoDelete(BOOL bAutoDelete);

protected:  // protected function
    OVERLAPPED *IocpAsyncOverlap();

	void	EnterThread();
	void	LeaveThread();

protected:  // protected member.

	/// 关联完成端口时使用的可overlap的句柄。
	union
	{
		HANDLE	m_hHandle;	///<	file handle
		SOCKET	m_socket;	///<	network socket 
	};			

// 	/// 用来标记当前对象的工作状态的对象, @see {IoCompletionHandlerStatus}。
//     int             m_iIoCompletionStatus; 


protected: // virtual function
    
    /**
	 * 销毁对象的内部资源。
	 *
	 * 使用 CIoCompletionPortModel 对象的 void DetachHandler(CIoCompletionHandlerAbstract *)来解除类实例对象时会调用这个函数。
	 * 函数内部必须实现销毁 m_hHandle 或 m_socket 句柄，并句柄赋值为 INVALID_HANDLE_VALUE 或 INVALID_SOCKET，以确保函数重入调用时造成异常。
     * 函数必须实现为可重入的，避免重入调用时造成异常。
     */
    virtual void Destroy() = 0;

    /**
     * 主要处理成员变量m_hHandle使用的资源与完成端口句柄进行重叠的内容。
     */
    virtual BOOL OverlapForIOCompletion() = 0;

	/**
	 * 当类的CIoCompletionHandlerAbstract::m_iIoCompletionStatus正常工作且
	 * 非ICS_OVERLAP_HANDLE状态时， CIoCompletionHandlerAbstract::m_hHandle有信号
	 * 被触发时的调用函数。
	 *
	 * @param[in] dwNumOfTransferedBytes CIoCompletionHandlerAbstract::m_hHandle
	 *	信号触发时CIoCompletionHandlerAbstract::m_pMemoryBuffer接收的数据量。
	 *
	 * @return 如果这一函数中有对CIoCompletionHandlerAbstract::m_hHandle进行overlap
	 * 失败时返回FALSE，成功时返回TRUE.
	 *
	 */
    virtual BOOL DataTransferTrigger(DWORD dwNumOfTransportBytes) = 0;

    /**
     * 当对象在完成端口模型中发生错误时，这个函数被会在完成端口线程中被回调。
	 *
	 * @param[in] dwErrorCode 该返回值取决于被IOCP关联时的对象被CloseHandle时的情况。
	 *						  参见：Windows的API GetLastError()。
	 *
	 * @return 当这个函数被回调处理时，函数内部有进行OVERLAPPED并且成功时，返回TRUE,
	 *			否则返回FALSE。
     */
	virtual void HandleRaiseError(DWORD dwErrorCode){};

	/**
	 * 当类对象被成功CIoCompletionPortModel::Attach时，用这函数来确保对象被成功拆离
	 * 销毁，保证对象安全。
	 *
	 * 必须在派生类的析构函数中，或销毁对象前调用这个函数，以保证对象的线程安全。
	 */
    void WaitDetachIocpModel();

private:
	/**
	 * 禁止复制实例。
	 */
	CIoCompletionHandlerAbstract(CIoCompletionHandlerAbstract &){}

    void _AttachIocpModel(); // 用于多线程安全退出
    void _DetachIocpModel(); // 用于多线程安全退出

    CIoCompletionHandlerAbstractPrivate *m;
};

