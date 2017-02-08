#pragma once

#include <ZZUtility/DLLDefines.h>
#include <wtypes.h>

class _ZZUTILITY_EXTERN_ CSystemInfo
{
public:
    enum SYSTEM_VERSION
	{
		SV_Win2000,
		SV_WinXP,
		SV_WinXPProfessionalX64,
		SV_WinHomeServer,
		SV_WinServer2003,
		SV_WinServer2003_R2,
		SV_WinVista,
		SV_WinServer2008,
		SV_WinServer2008_R2,
		SV_Win7,
		SV_WinServer_2012,
		SV_Win8,
		SV_WinServer2012_R2,
		SV_Win8_1,
		SV_WinServer2016,
		SV_Win10,
		SV_Unknown
    };

public:
    static SYSTEM_VERSION SystemName();
    static BOOL        IsWow64();
};
