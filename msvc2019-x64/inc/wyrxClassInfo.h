///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYRX_CLASS_INFO_H
#define WYRX_CLASS_INFO_H

#include <string>
#include <cassert>
#include <wyErrors.h>
#include <wyrxDefs.h>

NS_WYRX_BEG

class Object;
class ObjectFactory;
class ClassInfoImpl;
class ClassExtension;

class WYRX_EXPORT ClassInfo
{
public:
    // Get the name of the class represented by this ClassInfo object.
    const std::string& className() const;

    // Get the ClassInfo object for the direct base class of the class represented by this ClassInfo object.
    // Returns nullptr if the represented class has no base class.
    ClassInfo* parent() const;

    // Check whether the class represented by this ClassInfo object is derived from or the same as
    // the class represented by otherClassInfo.
    bool isDerivedFrom(const ClassInfo* other) const;

    bool operator==(const ClassInfo& rhs) const noexcept
    {
        return this == &rhs;
    }
    bool operator!=(const ClassInfo& rhs) const noexcept
    {
        return !(*this == rhs);
    }

    // Creates a new instance of the class that this ClassInfo object represents.
    // Returns nullptr if the class is not creatable.
    Object* create() const;

    // Add an extension to this ClassInfo object.
    // @param key Extension type key.
    // @param pExtension Extension instance to add.
    // @return
    //   - Ok: succeeded.
    //   - NullInputPointer: key or pExtension is nullptr.
    //   - InvalidInput: pExtension is not compatible with key, or pExtension already has a host.
    //   - DuplicateKey: an extension for key already exists.
    wy::ErrorStatus addExtension(ClassInfo* key, ClassExtension* pExtension);

    // Remove the extension associated with the specified key.
    // @param key Extension type key.
    // @return
    //   - Ok: succeeded.
    //   - NullInputPointer: key is nullptr.
    //   - KeyNotFound: no extension is registered for key.
    wy::ErrorStatus removeExtension(ClassInfo* key);

    // Get the extension associated with the specified key.
    // @param key Extension type key.
    // @return
    //   - Pointer to the extension if found.
    //   - nullptr if key is nullptr or no extension is registered for key.
    const ClassExtension* getExtension(ClassInfo* key) const;

    // Returns the ClassInfo object registered for the given class name.
    // Returns nullptr if the class name is not registered.
    static ClassInfo* fromName(const std::string& className);

private:
    ClassInfo(
        const std::string& name,
        ClassInfo* pParent,
        const ObjectFactory* pObjFactory);
    ~ClassInfo();
    ClassInfo(const ClassInfo&) = delete;
    ClassInfo& operator=(const ClassInfo&) = delete;
    ClassInfo(ClassInfo&&) = delete;
    ClassInfo& operator=(ClassInfo&&) = delete;

    friend class ClassInfoManager;
    template<typename T> friend class ClassRegistrar;
    template<typename T> friend class AbstractClassRegistrar;

    static ClassInfo* registerClass(
        const std::string& name,
        ClassInfo* pParent,
        const ObjectFactory* pObjFactory);

private:
    ClassInfoImpl* _pImpl;
};

class ObjectFactory
{
public:
    virtual ~ObjectFactory() = default;
    virtual Object* createObject() const { return nullptr; }
};

template <typename T>
inline ClassInfo* classInfoOf()
{
    return T::classInfo();
}

template <>
inline ClassInfo* classInfoOf<void>()
{
    return nullptr;
}

template <typename T>
class ClassRegistrar : public ObjectFactory
{
public:
    static ClassInfo* classInfo()
    {
        static ClassRegistrar<T> registrar;
        static ClassInfo* pClassInfo = ClassInfo::registerClass(
            T::className(),
            classInfoOf<typename T::__baseClass>(),
            &registrar);
        assert(pClassInfo);
        return pClassInfo;
    }

    virtual Object* createObject() const override
    {
        return new T();
    };
};

template <typename T>
class AbstractClassRegistrar : public ObjectFactory
{
public:
    static ClassInfo* classInfo()
    {
        static AbstractClassRegistrar<T> registrar;
        static ClassInfo* pClassInfo = ClassInfo::registerClass(
            T::className(),
            classInfoOf<typename T::__baseClass>(),
            nullptr);
        assert(pClassInfo);
        return pClassInfo;
    }
};

NS_WYRX_END

#endif // WYRX_CLASS_INFO_H