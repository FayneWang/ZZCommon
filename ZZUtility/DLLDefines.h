#pragma once

#if defined(_WINDLL) && !defined(_ZZUtility_USE_STATIC_LIB)

    // �⵼��/�����궨��
    #ifndef ZZUTILITY_EXPORTS

        #define _ZZUTILITY_EXTERN_  __declspec(dllimport)

    #else

        #define _ZZUTILITY_EXTERN_  __declspec(dllexport)
#endif  // !ZZUTILITY_EXPORTS

#else

    #define _ZZUTILITY_EXTERN_

#endif // defined(_WINDLL) && !defined(_ZZUtility_USE_STATIC_LIB)