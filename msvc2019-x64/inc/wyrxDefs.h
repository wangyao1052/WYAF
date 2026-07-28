///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYRX_DEFS_H
#define WYRX_DEFS_H

#define NS_WYRX_BEG namespace wyrx {
#define NS_WYRX_END }

#if defined(_WIN32)
    #if defined(WYRX_LIBRARY)
        #define WYRX_EXPORT __declspec(dllexport)
    #else
        #define WYRX_EXPORT __declspec(dllimport)
    #endif
#else
    #define WYRX_EXPORT __attribute__((visibility("default")))
#endif

#endif // WYRX_DEFS_H
