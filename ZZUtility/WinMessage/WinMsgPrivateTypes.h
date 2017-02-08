#pragma once

#include <hash_map>
#include <wtypes.h>

#define WND_MSG_WNDCLASS_NAME L"XMZZUtilityWndMsgClassName"

class CWinMsgTarget;
typedef std::hash_map<HWND,CWinMsgTarget*> MSG_TARGET_MAP;
struct CWinMsgServerPrivate
{
    UINT           uRefCount;
    MSG_TARGET_MAP msgTargetObjMap;

    CWinMsgServerPrivate() : uRefCount(0)
    {}
};

struct CMsgTargetPrivate
{
    DWORD   dwObjBelongThreadId;
    CMsgTargetPrivate() : dwObjBelongThreadId(0)
    {}
};

