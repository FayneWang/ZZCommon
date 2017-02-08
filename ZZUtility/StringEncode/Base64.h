#pragma once

#include <ZZUtility/DLLDefines.h>
#include <string>

class _ZZUTILITY_EXTERN_ CBase64
{
public:
    CBase64(void);
    ~CBase64(void);

    static std::string EncodeBinary(const void *pData,int iDataSize);

	/**
	 * 
	 */
    static bool DecodeToBinary(const std::string &strBase64,char *pData,int *piDataSize);

private:
};

