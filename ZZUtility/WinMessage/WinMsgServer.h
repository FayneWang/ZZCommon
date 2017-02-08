#pragma once

#include <wtypes.h>
#include <ZZUtility/DLLDefines.h>


class CWinMsgTarget;
struct CWinMsgServerPrivate;
class _ZZUTILITY_EXTERN_ CWinMsgServer
{
    friend class CWinMsgTarget;
    friend LRESULT CALLBACK _WndMsgProc(HWND, UINT, WPARAM, LPARAM);
public: // static member function
    static BOOL Initialize();
    static BOOL Uninitialize();

public: // member function
    

private:
    CWinMsgServer(void);
    CWinMsgServer(CWinMsgServer&);
    ~CWinMsgServer(void);

    CWinMsgServerPrivate *m;


    static CWinMsgServer *s_pSingleton;
};

