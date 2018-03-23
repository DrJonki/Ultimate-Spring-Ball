#pragma once

#ifdef WTF_STATIC

  #define WTF_API

#else

  #ifdef WIN32

    #define WTF_API_EXPORT __declspec(dllexport)
    #define WTF_API_IMPORT __declspec(dllimport)

  #endif

  #ifdef WTF_EXPORTS
    #define WTF_API WTF_API_EXPORT
  #else
    #define WTF_API WTF_API_IMPORT
  #endif

#endif
