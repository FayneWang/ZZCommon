#pragma once

#include <ZZUtility/DLLDefines.h>


class _ZZUTILITY_EXTERN_ CZZThreadPoolWork
{
    friend class CZZThreadPool;
public:
    CZZThreadPoolWork(void);
    virtual ~CZZThreadPoolWork(void);

	virtual void WaitForWork(){};
	virtual void StopWork(){};

protected:
    virtual void Run() = 0;

};

