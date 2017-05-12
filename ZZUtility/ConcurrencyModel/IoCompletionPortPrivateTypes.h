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
							  * ���������ź�ʱ����ʾIO��ɶ���δ��ʼ������ʼ����
							  *	���źš���ҪΪ��IO��ɶ�������ɶ˿��߳��а�ȫ�˳���
							  * �����ٶ���CIoCompletionHandlerAbstract ����
							  */
    BOOL        bAutoDelete; // ��ɶ˿ڴ��������ʹ�ý������Ƿ��Զ����ٵı�ǡ�
    OVERLAPPED  overlap;	 // �����������ص�ʱ�����ݽṹ��
    LONG        lOverlappedCount; /* 
								   * ��¼�����ص��Ĵ��������ڱ�֤overlapped���
								   * �ڴ涼�ܱ������ͷš�
								   */

    CIoCompletionHandlerAbstractPrivate() : bAutoDelete(FALSE),lOverlappedCount(0)
    {}
};

class CIoCompletionHandlerAbstract;
class CIoCompletionPortModelPrivate
{
	friend class CIoCompletionPortModel;
	friend class CIoCompletionHandlerAbstract;

    HANDLE          hIocp;	// ��ɶ˿�ģ�͵ľ����
    int             iNumOfHanders;	// �����Ĵ���������������
    CCriticalSectionSync attchedSetAndThreadSync;	// ����������󼯺�setIocpAttchedObj���ٽ���ͬ������
    std::vector<HANDLE>  vtrThreadHandle;	// ��ɶ˿�ģ�Ͷ���ʹ�õ��߳̾����������
    std::set<CIoCompletionHandlerAbstract*> setIocpAttchedObj;	// �����Ĵ������ļ��ϡ�

    CIoCompletionPortModelPrivate() : 
        hIocp(NULL),
        iNumOfHanders(0)
    {}
};