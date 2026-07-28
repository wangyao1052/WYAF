///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYDB_DEFS_H
#define WYDB_DEFS_H

#define NS_WYDB_BEG namespace wydb {
#define NS_WYDB_END }

#if defined(_WIN32)
    #if defined(WYDB_LIBRARY)
        #define WYDB_EXPORT __declspec(dllexport)
    #else
        #define WYDB_EXPORT __declspec(dllimport)
    #endif
#else
    #define WYDB_EXPORT __attribute__((visibility("default")))
#endif

#endif // WYDB_DEFS_H
