// GetWindowsVersion.cpp : 定义控制台应用程序的入口点。
//

#include <windows.h>
#include "SystemInfo.h"

CSystemInfo::SYSTEM_VERSION CSystemInfo::SystemName()
{ 
	OSVERSIONINFOEX osver = {0}; 
	osver.dwOSVersionInfoSize = sizeof(osver); 
    if (!::GetVersionEx((OSVERSIONINFO*)&osver))
    {
		return SV_Unknown;
	} 
	if (osver.dwMajorVersion == 10 && osver.dwMinorVersion == 0)
	{
		return osver.wProductType == VER_NT_WORKSTATION ? SV_Win10 : SV_WinServer2016;
	}
	else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 0) //Version number 6.0
	{
		switch (osver.dwMinorVersion)
		{
		case 0:
			return osver.wProductType == VER_NT_WORKSTATION ? SV_WinVista : SV_WinServer2008;
		case 1:
			return osver.wProductType == VER_NT_WORKSTATION ? SV_Win7 : SV_WinServer2008_R2;
		case 2:
			return osver.wProductType == VER_NT_WORKSTATION ? SV_Win8 : SV_WinServer_2012;
		case 3:
			return osver.wProductType == VER_NT_WORKSTATION ? SV_Win8_1 : SV_WinServer2012_R2;
		default:
			break;
		}
	}
	else if(osver.dwMajorVersion == 5 && osver.dwMinorVersion == 0)//Version number 5.0
	{ 
		switch (osver.dwMinorVersion)
		{
		default:
			break;
		case 0:	
			return SV_Win2000;
		case 1:	
			return SV_WinXP;
		case 2:
			{
				SYSTEM_INFO si = { 0 };
				::GetSystemInfo(&si);
				if ((osver.wProductType == VER_NT_WORKSTATION)
					&& si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
				{
					return SV_WinXPProfessionalX64;
				}
				else
				{
					if (osver.wSuiteMask & VER_SUITE_WH_SERVER)
						return SV_WinHomeServer;
					else
						return GetSystemMetrics(SM_SERVERR2) == 0 ? SV_WinServer2003 : SV_WinServer2003_R2;
				}
			}
		}

	} 

	return SV_Unknown;
} 

BOOL CSystemInfo::IsWow64()
{
    typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
    LPFN_ISWOW64PROCESS fnIsWow64Process;
    BOOL bIsWow64 = FALSE;

    //IsWow64Process is not available on all supported versions of Windows.
    //Use GetModuleHandle to get a handle to the DLL that contains the function
    //and GetProcAddress to get a pointer to the function if available.
    fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(GetModuleHandle(TEXT("kernel32")),"IsWow64Process");
    if(NULL != fnIsWow64Process)
    {
		fnIsWow64Process(GetCurrentProcess(), &bIsWow64);
    }

    return bIsWow64;
}