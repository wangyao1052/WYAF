///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2026 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYRX_CLASS_EXTENSION_H
#define WYRX_CLASS_EXTENSION_H

#include <wyrxDefs.h>
#include <wyrxObject.h>

NS_WYRX_BEG

// Base class for all class extensions.
// A class extension attaches extra behavior or metadata to a ClassInfo object.
// Each extension instance can be attached to at most one host ClassInfo.
class WYRX_EXPORT ClassExtension : public Object
{
    WYRX_DECLARE_ABSTRACT_MEMBERS(ClassExtension, wyrx::ClassExtension, wyrx::Object)
protected:
    ClassExtension() : _pHost(nullptr) {}
    virtual ~ClassExtension() = default;
    ClassExtension(const ClassExtension&) = delete;
    ClassExtension& operator=(const ClassExtension&) = delete;
    ClassExtension(ClassExtension&&) = delete;
    ClassExtension& operator=(ClassExtension&&) = delete;
    friend class SystemInternals;

public:
    ClassInfo* getHost() const
    {
        return _pHost;
    }

private:
    ClassInfo* _pHost;
};

NS_WYRX_END

#endif // WYRX_CLASS_EXTENSION_H