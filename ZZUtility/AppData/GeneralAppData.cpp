#include <assert.h>
#include <Windows.h>
#include <ShlObj.h>
#include <ZZUtility/Macro/ZZMacro.h>
#include "GeneralAppData.h"

static LPCWSTR g_pwstrCompanyName = NULL;

CGeneralAppData::CGeneralAppData(LPCWSTR pwstrCompanyName)
{
	assert(g_pwstrCompanyName == NULL);
	g_pwstrCompanyName = pwstrCompanyName;
}

CGeneralAppData::~CGeneralAppData(void)
{}

std::wstring CGeneralAppData::GetProgramDataPath(LPCWSTR pwstrAppName)
{
	PWSTR pPathBuffer;
	WCHAR wszProgramDataPath[MAX_PATH] = {0};
	if (SHGetKnownFolderPath(FOLDERID_ProgramData, 0, NULL, &pPathBuffer) != S_OK)
	{
		return std::wstring();
	}

	wcscat_s(wszProgramDataPath, pPathBuffer);
	wcscat_s(wszProgramDataPath, L"\\");
	wcscat_s(wszProgramDataPath, g_pwstrCompanyName);
	wcscat_s(wszProgramDataPath, L"\\");
	wcscat_s(wszProgramDataPath, pwstrAppName);
	CoTaskMemFree(pPathBuffer);

	return std::wstring(wszProgramDataPath);
}

HKEY CGeneralAppData::CreateLocalMachineRegHKey(LPCWSTR pwstrAppName)
{
    WCHAR wszRegPath[MAX_PATH] = L"SOFTWARE\\";
	wcscat_s(wszRegPath, g_pwstrCompanyName);
    wcscat_s(wszRegPath,L"\\");
    wcscat_s(wszRegPath,pwstrAppName);

    HKEY hKey;
    LONG lRes;
	if (ZZ_IsX64Process()) // 64 bit OS 中读 wow64注册表路径的信息
        lRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE, wszRegPath, 0, KEY_READ|KEY_WOW64_32KEY, &hKey);
    else
        lRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE, wszRegPath, 0, KEY_READ, &hKey);

    if(lRes != ERROR_SUCCESS)
        return NULL;

    return hKey;
}

std::wstring CGeneralAppData::GetUserLocalAppDataPath(LPCWSTR pwstrAppName)
{
	PWSTR pFolderPath;
	WCHAR wszDirPath[MAX_PATH] = { 0 };
	if (SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &pFolderPath) != S_OK)
	{
		return std::wstring();
	}

	wcscat_s(wszDirPath, pFolderPath);
	wcscat_s(wszDirPath, L"\\");
	wcscat_s(wszDirPath, g_pwstrCompanyName);
	wcscat_s(wszDirPath, L"\\");
	wcscat_s(wszDirPath, pwstrAppName);
	CoTaskMemFree(pFolderPath);

	return std::wstring(wszDirPath);
}

HKEY CGeneralAppData::CreateAppUninstallRegHKey(LPCWSTR pwstrAppName)
{
	WCHAR wszRegPath[MAX_PATH] = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\";
	wcscat_s(wszRegPath, pwstrAppName);

	HKEY hKey;
	LONG lRes;
	if (ZZ_IsX64Process()) // 64 bit OS 中读 wow64注册表路径的信息
		lRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE, wszRegPath, 0, KEY_READ | KEY_WOW64_32KEY, &hKey);
	else
		lRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE, wszRegPath, 0, KEY_READ, &hKey);

	if (lRes != ERROR_SUCCESS)
		return NULL;

	return hKey;
}
