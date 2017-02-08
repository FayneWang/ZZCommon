#pragma once

#include <WTypes.h>
#include <DbgHelp.h>
#include <ZZUtility/DLLDefines.h>

class _ZZUTILITY_EXTERN_ CCrashDump
{
public:
	static void CreateCrashDump(LPCWSTR pwstrFilePrefix,int iMinidumpType);
};

