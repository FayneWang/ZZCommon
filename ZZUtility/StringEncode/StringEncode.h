#pragma once


#include <stdint.h>
#include <string>
#include <ZZUtility/DLLDefines.h>

class _ZZUTILITY_EXTERN_ CStringEncode
{
public:
    CStringEncode(void);
    ~CStringEncode(void);

public:
	static std::wstring Utf8ToWString(const char *pUtf8);

    static char *Utf16ToUtf8(const std::wstring &wstrText,int32_t *piEncodedSize);
    static char *UnicodeToLocal8Bit(const std::wstring &wstrText,int32_t &iEncodedSize);
    static std::wstring Local8itToUtf16(const std::string &strContent);
    static std::string  Utf16ToLocal8bit(const std::wstring &wstrContent);
};

