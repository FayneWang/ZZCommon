#pragma once

#include <WTypes.h>
#include <ZZUtility/DLLDefines.h>

class _ZZUTILITY_EXTERN_ CWinMsgDispatcher
{
public:
    CWinMsgDispatcher(HWND hMsgWnd = nullptr,UINT uMsgId=0);
    ~CWinMsgDispatcher(void);
    
    void    AttachMsgTarget(HWND hMsgWnd,UINT uMsgId);

    BOOL    PostMsg(WPARAM wParam,LPARAM lParam = NULL);

    LRESULT SendMsg(WPARAM wParam,LPARAM lParam);

protected:
    HWND    m_hMsgWnd;
    UINT    m_uMsgId;
};

