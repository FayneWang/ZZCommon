#pragma once

#include <string>
#include <wtypes.h>
#include <ZZUtility/DLLDefines.h>

class _ZZUTILITY_EXTERN_ CGeneralAppData
{
public: // static function
	static std::wstring GetProgramDataPath(LPCWSTR pwstrAppName);
	static std::wstring GetUserLocalAppDataPath(LPCWSTR pwstrAppName);
	static HKEY CreateLocalMachineRegHKey(LPCWSTR pwstrAppName);
	static HKEY CreateAppUninstallRegHKey(LPCWSTR pwstrAppName);

protected:
	CGeneralAppData(LPCWSTR pwstrCompanyName);
    ~CGeneralAppData(void);

};

