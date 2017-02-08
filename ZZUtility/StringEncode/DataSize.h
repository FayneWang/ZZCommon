#pragma once

#include <stdint.h>
#include <string>
#include <ZZUtility/DLLDefines.h>

class _ZZUTILITY_EXTERN_ CDataSize
{
public:
    static std::wstring ToString(double dSize);
    static std::wstring ToSpeedPerSecond(double dSize);
};

