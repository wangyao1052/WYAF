///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYDB_BOILER_H
#define WYDB_BOILER_H

#include <cstdint>
#include <cstddef>

#include <wyrxBoiler.h>
#include <wydbDefs.h>

NS_WYDB_BEG

#define WYDB_DECLARE_MEMBERS(CLASS, CLASSNAME, BASECLASS) \
protected: \
    CLASS(); \
    virtual ~CLASS(); \
    CLASS(const CLASS&) = delete; \
    CLASS& operator=(const CLASS&) = delete; \
    CLASS(CLASS&&) = delete; \
    CLASS& operator=(CLASS&&) = delete; \
public: \
    static void registerParameters(wydb::ParameterSchemaExtension* pParamSchema); \
    WYRX_DECLARE_MEMBERS(CLASS, CLASSNAME, BASECLASS)

#define WYDB_DECLARE_ABSTRACT_MEMBERS(CLASS, CLASSNAME, BASECLASS) \
protected: \
    CLASS(); \
    virtual ~CLASS(); \
    CLASS(const CLASS&) = delete; \
    CLASS& operator=(const CLASS&) = delete; \
    CLASS(CLASS&&) = delete; \
    CLASS& operator=(CLASS&&) = delete; \
public: \
    static void registerParameters(wydb::ParameterSchemaExtension* pParamSchema); \
    WYRX_DECLARE_ABSTRACT_MEMBERS(CLASS, CLASSNAME, BASECLASS)

#define WYDB_REGISTER_CLASS(CLASS) \
    static wyrx::ClassInfo* ____##CLASS_classInfo = CLASS::classInfo();

#define WYDB_IMPLEMENT_MEMBERS(CLASS) \
    WYDB_REGISTER_CLASS(CLASS) \
    static const bool ____##CLASS##Parameters##_registered = []() -> bool \
    { \
        static wydb::ParameterSchemaExtension paramSchemaExt; \
        wy::ErrorStatus error = CLASS::classInfo()->addExtension(wydb::ParameterSchemaExtension::classInfo(), &paramSchemaExt); \
        if (wy::ErrorStatus::Ok != error) \
        { \
            assert(false); \
            std::abort(); \
        } \
        CLASS::registerParameters(&paramSchemaExt); \
        return true; \
    }();

NS_WYDB_END

#endif // WYDB_BOILER_H