#pragma once

#include <wtypes.h>
#include <ZZUtility/DLLDefines.h>

struct CSingletonProcessGuardPrivate;
class _ZZUTILITY_EXTERN_ CSingleProcessGuard
{
public:
    static HANDLE GuiadHandle(LPCWSTR pGuardName,BOOL bIsGlobal = FALSE);
    static BOOL HasGuarded(LPCWSTR pGuardName,BOOL bIsGlobal = FALSE);

    BOOL GuardSingleProcess(LPCWSTR pGuardName,BOOL bIsGlobal = FALSE);
    void UnguardSingleProcess();

protected:
    CSingleProcessGuard(void);
    virtual ~CSingleProcessGuard(void) = 0;

private:
    CSingletonProcessGuardPrivate *m;
};

