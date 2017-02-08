#include <Windows.h>
#include "WinMsgPrivateTypes.h"
#include "WinMsgTarget.h"
#include "WinMsgServer.h"

CWinMsgServer *CWinMsgServer::s_pSingleton = NULL;

CWinMsgServer::CWinMsgServer(void) : m(new CWinMsgServerPrivate())
{}

CWinMsgServer::CWinMsgServer(CWinMsgServer&) : m(new CWinMsgServerPrivate())
{}

CWinMsgServer::~CWinMsgServer(void)
{
    delete m;
}

BOOL CWinMsgServer::Initialize()
{
    if (NULL != s_pSingleton)
    {
        ++s_pSingleton->m->uRefCount;
        return TRUE;
    }

    WNDCLASSEX wndClassEx = {0};
    wndClassEx.cbSize = sizeof(WNDCLASSEX);
    wndClassEx.style			= 0;
    wndClassEx.lpfnWndProc	    = _WndMsgProc;
    wndClassEx.cbClsExtra		= 0;
    wndClassEx.cbWndExtra		= 0;
    wndClassEx.hInstance		= GetModuleHandle(NULL);
    wndClassEx.hIcon			= NULL;
    wndClassEx.lpszClassName	= WND_MSG_WNDCLASS_NAME;
    wndClassEx.hIconSm		    = wndClassEx.hIcon;

    if (RegisterClassEx(&wndClassEx) != 0)
    {
        s_pSingleton = new CWinMsgServer();
        ++s_pSingleton->m->uRefCount;
        return TRUE;
    }

    return FALSE;
}

BOOL CWinMsgServer::Uninitialize()
{
    if (NULL != s_pSingleton)
    {
        --s_pSingleton->m->uRefCount;
        if (0 == s_pSingleton->m->uRefCount)
        {
            delete s_pSingleton;
            s_pSingleton = NULL;
            return UnregisterClass(WND_MSG_WNDCLASS_NAME,GetModuleHandle(L"ENUtility.dll"));
        }

        return TRUE;
    }

    return FALSE;
}

LRESULT CALLBACK _WndMsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (hWnd != NULL)
    {
        MSG_TARGET_MAP &targetMap = CWinMsgServer::s_pSingleton->m->msgTargetObjMap;
        MSG_TARGET_MAP::iterator itrMsgTargetObj = targetMap.find(hWnd);
        if (itrMsgTargetObj != targetMap.end())
        {
            if((*itrMsgTargetObj).second->MsgProc(message,wParam,lParam))
                return 0;
        }
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}