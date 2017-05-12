#pragma once

/**
 * @file
 * @author wangzezhou <83209400@qq.com>
 * @date 2015-05-19
 *
 * @brief Windows完成端口模型的封装类，与@see ｛CIoCompletionHandlerAbstract｝类
 * 一起使用。
 *
 */

#include <set>
#include <wtypes.h>
#include <ZZUtility/DLLDefines.h>

/**
* @class CIoCompletionPortModel
*
* @breif Windows IO Completion Model(完成端口)模型封装类，该类是一个单例实现。
*
* 该类完全线程安全，类实例与@see ｛CIoCompletionHandlerAbstract｝派生类一起协作使用。
* 内部的最大、最小并发线程数的创建和销毁是根据关联的@see ｛CIoCompletionHandlerAbstract｝实例数量或
* CPU的内核数量进行自动控制。
*
*/
class CIoCompletionHandlerAbstract;
class CIoCompletionPortModelPrivate;
class _ZZUTILITY_EXTERN_ CIoCompletionPortModel
{
    friend DWORD WINAPI IocpConcurrentProc(LPVOID lpParameter);

public:
	HANDLE	GetHandle();
    
    /**
     * 关联一个可overlaped的资源对象应用于完成端口并发模型中。
	 *
	 * @param[in] pHandler 详见：@see {CIoCompletionHandlerAbstract}
	 * 
	 * @return 关联成功时，返回TRUE.否则返回FALSE。
     */
    BOOL AttachHandler(CIoCompletionHandlerAbstract *pHandler);
    
    /**
     * 关联一个可overlapped的资源对象应用于完成端口并发模型中。
	 *
	 * @param[in] pHandler 详见：@see {CIoCompletionHandlerAbstract}
	 * 
	 * @return 关联成功时，返回TRUE.否则返回FALSE。
     */
    void DetachHandler(CIoCompletionHandlerAbstract *pHandler);
    
    /**
     * 触发一次完成端口信号，使@see {CIoCompletionHandlerAbstract}对象在完成端口模型
	 * 线程中处理 CIoCompletionHandlerAbstract 对象的当前状态的需要进行的工作。
	 * 
	 * @warning 未确定@see {CIoCompletionHandlerAbstract}对象是否需要与完成端口进行
	 * 关联。才能使用该函数调用。函数中没有判断要PostHandler的对象是否被关联状态。
     */
    BOOL PostHandlerStatus(CIoCompletionHandlerAbstract *pHandler);
    
    /**
     * 单例类对象的获取接口。在整个应用程序中，即使有众多不同的端口需要并发的，也仅
	 * 需要一个完成端口模型对象来处理。
	 * 
	 * @return 完成端口模型的单例对象。
     */
    static CIoCompletionPortModel *Instance();
    
    /**
     * 使用该接口来手动销毁单例对象，
     */
    static void Uninstance();

private:
    struct ThreadData
    {
        CIoCompletionPortModel *pIocpModel;
        HANDLE                  hThread;
    };

private:
    CIoCompletionPortModel(const CIoCompletionPortModel &);
    CIoCompletionPortModel(void);
    ~CIoCompletionPortModel(void);

private:
	void _ThreadHanleProc(HANDLE hThread);
	BOOL _IsExitProcessThread(CIoCompletionHandlerAbstract *pCompletionHandler);
    void _IocpProc();
    CIoCompletionPortModelPrivate *m;

    static CIoCompletionPortModel *s_pSinglton;
};

