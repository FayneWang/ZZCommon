#pragma once

#include <wtypes.h>
#include <ZZUtility/DLLDefines.h>

class _ZZUTILITY_EXTERN_ CSingleProcessGuard
{
public:
	CSingleProcessGuard(void);
	virtual ~CSingleProcessGuard(void);

    BOOL Guard(LPCWSTR pGuardName,BOOL bIsGlobal = FALSE);
	void Unguard();

public:
	static BOOL IsGuarded(LPCWSTR pGuardName, BOOL bIsGlobal = FALSE);

private:
	HANDLE m_hGurad;

};

