#pragma once

#include <Windows.h>
#include <vector>
#include <set>
#include <ZZUtility/Synchronization/EventSync.h>
#include <ZZUtility/Synchronization/CriticalSectionSync.h>

class CCriticalSectionSync;
class CIoCompletionHandlerAbstractPrivate
{
	friend class CIoCompletionPortModel;
	friend class CIoCompletionHandlerAbstract;

	CCriticalSectionSync csCriticalLock;
    CEventSync  esModelTie;	 /* 
							  * 当对象有信号时，表示IO完成对象还未初始化；初始化后，
							  *	无信号。主要为了IO完成对象在完成端口线程中安全退出，
							  * 再销毁对象CIoCompletionHandlerAbstract 对象。
							  */
    BOOL        bAutoDelete; // 完成端口处理对象在使用结束后是否自动销毁的标记。
    OVERLAPPED  overlap;	 // 处理对象进行重叠时的数据结构。
    LONG        lOverlappedCount; /* 
								   * 记录对象被重叠的次数，用于保证overlapped后的
								   * 内存都能被正常释放。
								   */

    CIoCompletionHandlerAbstractPrivate() : bAutoDelete(FALSE),lOverlappedCount(0)
    {}
};

class CIoCompletionHandlerAbstract;
class CIoCompletionPortModelPrivate
{
	friend class CIoCompletionPortModel;
	friend class CIoCompletionHandlerAbstract;

    HANDLE          hIocp;	// 完成端口模型的句柄。
    int             iNumOfHanders;	// 关联的处理类对象的数量。
    CCriticalSectionSync attchedSetAndThreadSync;	// 关联处理对象集合setIocpAttchedObj的临界区同步对象
    std::vector<HANDLE>  vtrThreadHandle;	// 完成端口模型对象使用的线程句柄容器对象
    std::set<CIoCompletionHandlerAbstract*> setIocpAttchedObj;	// 关联的处理对象的集合。

    CIoCompletionPortModelPrivate() : 
        hIocp(NULL),
        iNumOfHanders(0)
    {}
};