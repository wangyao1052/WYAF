///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2026 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#include <string>
#include <map>
#include <cstdlib>
#include <cassert>

#include <wydbDefs.h>
#include <wydbFieldId.h>

NS_WYDB_BEG

class FieldRegistryImpl;

class WYDB_EXPORT FieldRegistry
{
private:
    FieldRegistry();
    ~FieldRegistry();
    FieldRegistry(const FieldRegistry&) = delete;
    FieldRegistry& operator=(const FieldRegistry&) = delete;
    FieldRegistry(FieldRegistry&&) = delete;
    FieldRegistry& operator=(FieldRegistry&&) = delete;

public:
    static FieldRegistry& instance();

    bool registerField(FieldId id, const char* fullName);

    bool contains(FieldId id) const;

    std::string findName(FieldId fieldId) const;

private:
    FieldRegistryImpl* _pImpl;
};

#define BEGIN_FIELD_REGISTRATION() \
namespace {

#define REGISTER_FIELD(CLASS, FIELD) \
    static constexpr wydb::FieldId k##CLASS##FIELD = wydb::FieldId::make(#CLASS "::" #FIELD); \
    static const bool ____##CLASS##FIELD##_registered = []() -> bool \
    { \
        const bool isRegistered = wydb::FieldRegistry::instance().contains(k##CLASS##FIELD); \
        if (isRegistered) \
        { \
            assert(false); \
            std::abort(); \
        } \
        const bool ok = wydb::FieldRegistry::instance().registerField( \
            k##CLASS##FIELD, #CLASS "::" #FIELD); \
        assert(ok); \
        return true; \
    }();

#define END_FIELD_REGISTRATION() \
}

NS_WYDB_END