///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYRX_OBJECT_H
#define WYRX_OBJECT_H

#include <wyrxDefs.h>
#include <wyrxBoiler.h>
#include <wyrxClassInfo.h>

NS_WYRX_BEG

class SystemInternals;

class WYRX_EXPORT Object
{
protected:
    Object() {};
    virtual ~Object() {};
    Object(const Object&) = delete;
    Object& operator=(const Object&) = delete;
    Object(Object&&) = delete;
    Object& operator=(Object&&) = delete;
    friend class SystemInternals;

public:
    template<typename T> friend class ClassRegistrar;
    typedef void __baseClass;
    static const char* className() { return "wyrx::Object"; }
    static ClassInfo* classInfo();

    // Get the runtime class information for this object.
    virtual ClassInfo* getClassInfo() const { return Object::classInfo(); }

    // Check whether this object is an instance of the class represented by classInfo
    // or of a class derived from it.
    bool isKindOf(ClassInfo* classInfo) const;
};

inline bool Object::isKindOf(ClassInfo* classInfo) const
{
    return this->getClassInfo()->isDerivedFrom(classInfo);
}

WYRX_EXPORT void deleteObject(const Object* pObject);

NS_WYRX_END

#endif // WYRX_OBJECT_H