#pragma once

#include <string>
#include <wtypes.h>
#include <ZZUtility/DLLDefines.h>
#include <ZZUtility/Process/SingleProcessGuard.h>

class _ZZUTILITY_EXTERN_ CAppProcess : public CSingleProcessGuard
{
public:
    CAppProcess(void);
    virtual ~CAppProcess(void) = 0;

    static BOOL SetExeModuleDirForCurrentDir();

    static std::wstring GetExeModuleFile();

    BOOL Reboot(LPCWSTR lpParameters = NULL,int iExitCode = 0);
};

