///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYAP_DEFS_H
#define WYAP_DEFS_H

#define NS_WYAP_BEG namespace wyap {
#define NS_WYAP_END }

#if defined(_WIN32)
    #if defined(WYAP_LIBRARY)
        #define WYAP_EXPORT __declspec(dllexport)
    #else
        #define WYAP_EXPORT __declspec(dllimport)
    #endif
#else
    #define WYAP_EXPORT __attribute__((visibility("default")))
#endif

#endif // WYAP_DEFS_H
