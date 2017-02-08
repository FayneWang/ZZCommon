#include <time.h>
#include "CrashDump.h"
#include <DbgHelp.h>

#define CMD_DUMP_FILE_SUFFIX_LENGTH sizeof("yyyyMMDDHHmmss.dmp")

static int		s_iMinidumpType = 0;
static LPWSTR s_pwstrDumpFile = nullptr;

LONG WINAPI CreateMiniDump( EXCEPTION_POINTERS* pExceptionPointers ) 
{
	time_t tNow = time(nullptr);
	wcsftime(s_pwstrDumpFile+wcslen(s_pwstrDumpFile),CMD_DUMP_FILE_SUFFIX_LENGTH,
		L"%Y%m%d%H%M%S.dmp",localtime(&tNow));

	HANDLE hFile = CreateFile( s_pwstrDumpFile, GENERIC_READ | GENERIC_WRITE, 
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	delete [] s_pwstrDumpFile;
	s_pwstrDumpFile = nullptr;

	if( ( hFile != NULL ) && ( hFile != INVALID_HANDLE_VALUE ) ) 
	{
		MINIDUMP_EXCEPTION_INFORMATION mdei; 
		mdei.ThreadId           = GetCurrentThreadId(); 
		mdei.ExceptionPointers  = pExceptionPointers; 
		mdei.ClientPointers     = FALSE; 

		MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), 
			hFile, (MINIDUMP_TYPE)s_iMinidumpType, 
			(pExceptionPointers != 0) ? &mdei : 0, nullptr, nullptr ); 

		CloseHandle( hFile );
	}

	return EXCEPTION_EXECUTE_HANDLER;
}

void CCrashDump::CreateCrashDump(LPCWSTR pwstrFilePrefix,int iMinidumpType)
{
	s_pwstrDumpFile = new WCHAR[wcslen(pwstrFilePrefix) + CMD_DUMP_FILE_SUFFIX_LENGTH];
	wcscpy(s_pwstrDumpFile,pwstrFilePrefix); 
	s_iMinidumpType = iMinidumpType;
	SetUnhandledExceptionFilter(CreateMiniDump);
}
