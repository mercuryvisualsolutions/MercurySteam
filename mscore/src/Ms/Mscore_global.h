#ifndef MSCORE_GLOBAL_H
#define MSCORE_GLOBAL_H

#if defined(__GNUC__)//GNU GCC
    #if __GNUC__ >= 4
        #define SHARED_LIB_EXPORT __attribute__ ((visibility("default")))
        #define SHARED_LIB_IMPORT __attribute__ ((visibility("default")))
    #else
        #define SHARED_LIB_EXPORT
        #define SHARED_LIB_IMPORT
    #endif
#elif defined(_MSC_VER)//Microsoft
    #define SHARED_LIB_EXPORT __declspec(dllexport)
    #define SHARED_LIB_IMPORT __declspec(dllimport)
#endif

#endif // MSCORE_GLOBAL_H
