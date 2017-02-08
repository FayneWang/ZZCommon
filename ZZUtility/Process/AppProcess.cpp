
#include <windows.h>
#include "AppProcess.h"

CAppProcess::CAppProcess(void)
{
}


CAppProcess::~CAppProcess(void)
{
}

BOOL CAppProcess::SetExeModuleDirForCurrentDir()
{
    wchar_t wszFile[MAX_PATH] = {0};
    if(GetModuleFileName(GetModuleHandle(NULL),wszFile,MAX_PATH) != 0)
    {
        *wcsrchr(wszFile,L'\\') = L'\0';
        if(SetCurrentDirectory(wszFile))
        {
            return TRUE;
        }
    }

    return FALSE;
}

std::wstring CAppProcess::GetExeModuleFile()
{
    wchar_t wszFile[MAX_PATH] = {0};
    if(GetModuleFileName(GetModuleHandle(NULL),wszFile,MAX_PATH) != 0)
    {
        return std::wstring(wszFile);
    }

    return wszFile;
}

BOOL CAppProcess::Reboot(LPCWSTR lpParameters,int iExitCode)
{
	UnguardSingleProcess();
    if((int)ShellExecute(NULL,L"open",GetExeModuleFile().c_str(),lpParameters,NULL,SW_SHOW) <= 32)
    {
        return FALSE;
    }

    PostQuitMessage(iExitCode);
    return TRUE;
}
