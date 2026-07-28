///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYRX_BOILER_H
#define WYRX_BOILER_H

#include <wyrxDefs.h>

NS_WYRX_BEG

#define WYRX_DECLARE_MEMBERS(CLASS, CLASSNAME, BASE) \
public: \
    template<typename T> friend class wyrx::ClassRegistrar; \
    typedef BASE __baseClass; \
    static inline const char* className() { return #CLASSNAME; } \
    static inline wyrx::ClassInfo* classInfo() { return wyrx::ClassRegistrar<CLASS>::classInfo(); } \
    static inline const CLASS* cast(const wyrx::Object* inPtr) \
    { \
        if (inPtr && inPtr->isKindOf(CLASS::classInfo())) \
            return static_cast<const CLASS*>(inPtr); \
        else \
            return nullptr; \
    } \
    static inline CLASS* cast(wyrx::Object* inPtr) \
    { \
        if (inPtr && inPtr->isKindOf(CLASS::classInfo())) \
            return static_cast<CLASS*>(inPtr); \
        else \
            return nullptr; \
    } \
    virtual wyrx::ClassInfo* getClassInfo() const override { return CLASS::classInfo(); }

#define WYRX_DECLARE_ABSTRACT_MEMBERS(CLASS, CLASSNAME, BASE) \
public: \
    template<typename T> friend class wyrx::AbstractClassRegistrar; \
    typedef BASE __baseClass; \
    static inline const char* className() { return #CLASSNAME; } \
    static inline wyrx::ClassInfo* classInfo() { return wyrx::AbstractClassRegistrar<CLASS>::classInfo(); } \
    static inline const CLASS* cast(const wyrx::Object* inPtr) \
    { \
        if (inPtr && inPtr->isKindOf(CLASS::classInfo())) \
            return static_cast<const CLASS*>(inPtr); \
        else \
            return nullptr; \
    } \
    static inline CLASS* cast(wyrx::Object* inPtr) \
    { \
        if (inPtr && inPtr->isKindOf(CLASS::classInfo())) \
            return static_cast<CLASS*>(inPtr); \
        else \
            return nullptr; \
    } \
    virtual wyrx::ClassInfo* getClassInfo() const override { return CLASS::classInfo(); }


#define WYRX_REGISTER_CLASS(CLASS) \
    static const auto& ____wyrxreg_##CLASS = CLASS::classInfo();

NS_WYRX_END

#endif // WYRX_BOILER_H