///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WY_DEFS_H
#define WY_DEFS_H

#define NS_WY_BEG namespace wy {
#define NS_WY_END }

#if defined(_WIN32)
    #if defined(WY_LIBRARY)
        #define WY_EXPORT __declspec(dllexport)
    #else
        #define WY_EXPORT __declspec(dllimport)
    #endif
#else
    #define WY_EXPORT __attribute__((visibility("default")))
#endif

#endif // WY_DEFS_H
