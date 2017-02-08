#include "DataSize.h"

#define DS_PB (1024LL*1024LL*1024LL*1024LL*1024LL)
#define DS_TB (1024LL*1024LL*1024LL*1024LL)
#define DS_GB (1024*1024*1024)
#define DS_MB (1024*1024)
#define DS_KB (1024)

std::wstring CDataSize::ToString(double dSize)
{
    wchar_t wszData[32] = L"NaUN";

    if (dSize >= DS_PB)
    {
        swprintf_s(wszData,30,L"%.2lf PB",dSize/DS_PB);
    }
    else if(dSize >= DS_TB)
    {
        swprintf_s(wszData,30,L"%.2lf TB",dSize/DS_TB);
    }
    else if(dSize >= DS_GB)
    {
        swprintf_s(wszData,30,L"%.2lf GB",dSize/DS_GB);
    }
    else if(dSize >= DS_MB)
    {
        swprintf_s(wszData,30,L"%.2lf MB",dSize/DS_MB);
    }
    else if (dSize >= DS_KB)
    {
        swprintf_s(wszData,30,L"%.2lf KB",dSize/DS_KB);        
    }
    else if (dSize >= 0)
    {
        swprintf_s(wszData,30,L"%d B",(int)dSize);   
    }

    return std::wstring(wszData);
}

std::wstring CDataSize::ToSpeedPerSecond(double dSize)
{
    wchar_t wszData[32] = L"NaUN/s";

    if(dSize >= DS_GB)
    {
        swprintf_s(wszData,30,L"%.2lfGB/s",dSize/DS_GB);
    }
    else if(dSize >= DS_MB)
    {
        swprintf_s(wszData,30,L"%.2lfMB/s",dSize/DS_MB);
    }
    else if (dSize >= DS_KB)
    {
        swprintf_s(wszData,30,L"%.2lfKB/s",dSize/DS_KB);        
    }
    else if (dSize >= 0)
    {
        swprintf_s(wszData,30,L"%dB/s",(int)dSize);   
    }

    return std::wstring(wszData);
}

#undef DS_PB
#undef DS_TB
#undef DS_GB
#undef DS_MB
#undef DS_KB