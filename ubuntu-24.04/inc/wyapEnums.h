///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2026 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYAP_ENUMS_H
#define WYAP_ENUMS_H

#include <wyapDefs.h>

NS_WYAP_BEG

// Execution mode.
enum class ExecutionMode
{
    Sync  = 1, // Synchronous execution.
    Async = 2, // Asynchronous execution posted to the request queue.
};

NS_WYAP_END

#endif // WYAP_ENUMS_H
