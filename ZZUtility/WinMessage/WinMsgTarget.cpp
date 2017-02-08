
#include <assert.h>
#include <hash_map>
#include "WinMsgPrivateTypes.h"
#include "WinMsgServer.h"
#include "WinMsgTarget.h"

CWinMsgTarget::CWinMsgTarget(LPCWSTR lpWindowName) : m(new CMsgTargetPrivate())
{
	assert(CWinMsgServer::s_pSingleton != nullptr);
    m_hTargetWnd = CreateWindow(WND_MSG_WNDCLASS_NAME,lpWindowName,0,0,0,0,0,NULL,NULL,GetModuleHandle(NULL),NULL);
    CWinMsgServer::s_pSingleton->m->msgTargetObjMap.insert(MSG_TARGET_MAP::value_type(m_hTargetWnd,this));
}

CWinMsgTarget::~CWinMsgTarget(void)
{
    CWinMsgServer::s_pSingleton->m->msgTargetObjMap.erase(m_hTargetWnd);
    DestroyWindow(m_hTargetWnd);
    delete m;
}

void CWinMsgTarget::WaitMessage()
{
    m->dwObjBelongThreadId = GetCurrentThreadId();
    MSG msg;
    while (GetMessage(&msg, (HWND)-1, 0, 0))
    {}

    m->dwObjBelongThreadId = 0;
}

BOOL CWinMsgTarget::PostThreadMessage(UINT uMsg,void *pParam)
{
    if (m->dwObjBelongThreadId == 0)
        return FALSE;

    return ::PostThreadMessage(m->dwObjBelongThreadId, uMsg, reinterpret_cast<WPARAM>(this), reinterpret_cast<LPARAM>(pParam));
}

BOOL CWinMsgTarget::PostWndMsg(UINT uMsg,WPARAM wPamram,LPARAM lParam)
{
    return ::PostMessage(m_hTargetWnd,uMsg,wPamram,lParam);
}

void CWinMsgTarget::ExitWait()
{
    if (m->dwObjBelongThreadId == 0 )
        return;

    ::PostThreadMessage(m->dwObjBelongThreadId, WM_QUIT, NULL, NULL);
}

