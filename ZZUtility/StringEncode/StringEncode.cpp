#include "StringEncode.h"

#include <Windows.h>

CStringEncode::CStringEncode(void)
{
}


CStringEncode::~CStringEncode(void)
{
}

std::wstring CStringEncode::Utf8ToWString(const char *pUtf8)
{
    wchar_t *pwstrUtf8Text = NULL;
    DWORD dwSize = MultiByteToWideChar(CP_UTF8, 0,pUtf8,-1, NULL,0);
    if(dwSize == 0)
        return NULL;

    pwstrUtf8Text = (wchar_t *)malloc((dwSize<<1)+sizeof(wchar_t));
    if (pwstrUtf8Text == NULL)
        return NULL;

    if(MultiByteToWideChar(CP_UTF8, 0,pUtf8,-1, pwstrUtf8Text,dwSize) == 0)
        return NULL;

    std::wstring wstrContent = pwstrUtf8Text;
    free(pwstrUtf8Text);

    return wstrContent;
}

char * CStringEncode::Utf16ToUtf8(const std::wstring &wstrText,int32_t *piEncodedSize)
{
    char *pUtf8Text = NULL;
    DWORD dwSize = WideCharToMultiByte(CP_UTF8, 0,wstrText.c_str(),wstrText.size(), NULL,0, NULL, NULL);
    if(dwSize == 0)
        return NULL;

    pUtf8Text = (char*)malloc(dwSize+1);
    if (pUtf8Text == NULL)
        return NULL;

    if(WideCharToMultiByte(CP_UTF8,0,wstrText.c_str(),wstrText.size(),pUtf8Text, dwSize,NULL, FALSE) == 0)
        return NULL;

    pUtf8Text[dwSize] = '\0';
    *piEncodedSize = dwSize;

    return pUtf8Text;
}

char * CStringEncode::UnicodeToLocal8Bit(const std::wstring &wstrText,int32_t &iEncodedSize)
{
    char *pUtf8Text = NULL;
    DWORD dwSize = WideCharToMultiByte(CP_ACP, 0,wstrText.c_str(),wstrText.size(), NULL,0, NULL, NULL);
    if(dwSize == 0)
        return NULL;

    pUtf8Text = (char*)malloc(dwSize+sizeof('\0'));
    if (pUtf8Text == NULL)
        return NULL;

    if(WideCharToMultiByte(CP_ACP,0,wstrText.c_str(),wstrText.size(),pUtf8Text, dwSize,NULL, FALSE) == 0)
        return NULL;

    pUtf8Text[dwSize]='\0';
    iEncodedSize = dwSize;
    return pUtf8Text;
}

std::wstring CStringEncode::Local8itToUtf16(const std::string &strContent)
{
	if (strContent.empty())
		return std::wstring();

    wchar_t *pwstrUtf8Text = NULL;
    DWORD dwSize = MultiByteToWideChar(CP_ACP, 0,strContent.c_str(),strContent.size(), NULL,0);
    if(dwSize == 0)
        return NULL;

    pwstrUtf8Text = (wchar_t *)malloc((dwSize<<1)+sizeof(wchar_t));
    if (pwstrUtf8Text == NULL)
        return NULL;

    if(MultiByteToWideChar(CP_ACP, 0,strContent.c_str(),strContent.size(), pwstrUtf8Text,dwSize) == 0)
        return NULL;

    pwstrUtf8Text[dwSize] = L'\0';

    std::wstring wstrContent = pwstrUtf8Text;
    free(pwstrUtf8Text);

    return wstrContent;
}

std::string CStringEncode::Utf16ToLocal8bit(const std::wstring &wstrContent)
{
	if (wstrContent.empty())
		return std::string();

    char *pUtf8Text = NULL;
    DWORD dwSize = WideCharToMultiByte(CP_ACP, 0,wstrContent.c_str(),wstrContent.size(), NULL,0, NULL, NULL);
    if(dwSize == 0)
        return NULL;

    pUtf8Text = (char*)malloc(dwSize+sizeof('\0'));
    if (pUtf8Text == NULL)
        return NULL;

    if(WideCharToMultiByte(CP_ACP,0,wstrContent.c_str(),wstrContent.size(),pUtf8Text, dwSize,NULL, FALSE) == 0)
        return NULL;

    pUtf8Text[dwSize]='\0';
    std::string strContent = pUtf8Text;
    free(pUtf8Text);

    return strContent;
}
