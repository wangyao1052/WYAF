///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYDB_TYPED_VALUE_H
#define WYDB_TYPED_VALUE_H

#include <cstdint>
#include <cstring>
#include <string>
#include <cassert>
#include <wyErrors.h>
#include <wydbDefs.h>

NS_WYDB_BEG

// TypedValue stores string values as owned null-terminated C strings.
// The string content is deeply copied on construction or assignment,
// and is automatically freed on destruction or reassignment.
// If a null pointer is provided, an empty string ("") is stored.
// Embedded '\0' characters are not preserved; only the content before
// the first null character is copied. 
class WYDB_EXPORT TypedValue
{
public:
    enum class Type : unsigned short
    {
        None = 0,
        Boolean,
        Int32,
        UInt32,
        Int64,
        UInt64,
        Float,
        Double,
        String,
    };

public:
    TypedValue() : _type(Type::None), _value() {}
    explicit TypedValue(bool value) : _type(Type::Boolean), _value(value) {}
    explicit TypedValue(std::int32_t value) : _type(Type::Int32), _value(value) {}
    explicit TypedValue(std::uint32_t value) : _type(Type::UInt32), _value(value) {}
    explicit TypedValue(std::int64_t value) : _type(Type::Int64), _value(value) {}
    explicit TypedValue(std::uint64_t value) : _type(Type::UInt64), _value(value) {}
    explicit TypedValue(float value) : _type(Type::Float), _value(value) {}
    explicit TypedValue(double value) : _type(Type::Double), _value(value) {}
    explicit TypedValue(const char* value) : _type(Type::String), _value(value) {}
    explicit TypedValue(const std::string& value) : _type(Type::String), _value(value.c_str()) {}
    ~TypedValue() { clear(); }
    TypedValue(const TypedValue& rhs);
    TypedValue& operator=(const TypedValue& rhs);
    TypedValue(TypedValue&& rhs) noexcept;
    TypedValue& operator=(TypedValue&& rhs) noexcept;

    static const TypedValue kNone;

    inline Type getType() const
    {
        return _type;
    }

    inline bool isNone() const
    {
        return _type == Type::None;
    }

    inline bool isBoolean() const
    {
        return _type == Type::Boolean;
    }

    inline bool isInt32() const
    {
        return _type == Type::Int32;
    }

    inline bool isUInt32() const
    {
        return _type == Type::UInt32;
    }

    inline bool isInt64() const
    {
        return _type == Type::Int64;
    }

    inline bool isUInt64() const
    {
        return _type == Type::UInt64;
    }

    inline bool isFloat() const
    {
        return _type == Type::Float;
    }

    inline bool isDouble() const
    {
        return _type == Type::Double;
    }

    inline bool isString() const
    {
        return _type == Type::String;
    }

    inline bool getBoolean() const
    {
        if (Type::Boolean == _type)
        {
            return _value.boolValue;
        }
        assert(false);
        return false;
    }

    inline std::int32_t getInt32() const
    {
        if (Type::Int32 == _type)
        {
            return _value.int32Value;
        }
        assert(false);
        return 0;
    }

    inline std::uint32_t getUInt32() const
    {
        if (Type::UInt32 == _type)
        {
            return _value.uint32Value;
        }
        assert(false);
        return 0;
    }

    inline std::int64_t getInt64() const
    {
        if (Type::Int64 == _type)
        {
            return _value.int64Value;
        }
        assert(false);
        return 0;
    }

    inline std::uint64_t getUInt64() const
    {
        if (Type::UInt64 == _type)
        {
            return _value.uint64Value;
        }
        assert(false);
        return 0;
    }

    inline float getFloat() const
    {
        if (Type::Float == _type)
        {
            return _value.floatValue;
        }
        assert(false);
        return 0.0f;
    }

    inline double getDouble() const
    {
        if (Type::Double == _type)
        {
            return _value.doubleValue;
        }
        assert(false);
        return 0.0;
    }

    // Returns a read-only pointer to the internally owned null-terminated string.
    // The caller must not modify or free the returned pointer.
    // The returned pointer remains valid until this object is destroyed
    // or the stored string value is changed.
    const char* getStringValue() const
    {
        if (Type::String == _type)
        {
            return _value.stringValue;
        }
        assert(false);
        return nullptr;
    }

    friend bool operator<(const TypedValue& lhs, const TypedValue& rhs);
    friend bool operator==(const TypedValue& lhs, const TypedValue& rhs);
    inline friend bool operator!=(const TypedValue& lhs, const TypedValue& rhs)
    {
        return !(lhs == rhs);
    }

private:
    inline void clear() noexcept
    {
        if (_type == Type::String)
        {
            delete[] _value.stringValue;
            _value.stringValue = nullptr;
        }
        _type = Type::None;
        _value.uint64Value = 0;
    }

    inline void swap(TypedValue& b) noexcept
    {
        std::swap(_type, b._type);
        std::swap(_value, b._value);
    }

    static char* cloneString(const char* value)
    {
        if (!value)
        {
            char* s = new char[1];
            s[0] = '\0';
            return s;
        }
        const std::size_t len = std::strlen(value);
        char* s = new char[len + 1];
        std::memcpy(s, value, len + 1);
        return s;
    }

private:
    Type _type;
    union WYDB_EXPORT Value
    {
        bool boolValue;
        std::int32_t int32Value;
        std::uint32_t uint32Value;
        std::int64_t int64Value;
        std::uint64_t uint64Value;
        float floatValue;
        double doubleValue;
        char* stringValue;

        Value() : uint64Value(0) {}
        explicit Value(bool value) : boolValue(value) {}
        explicit Value(std::int32_t value) : int32Value(value) {}
        explicit Value(std::uint32_t value) : uint32Value(value) {}
        explicit Value(std::int64_t value) : int64Value(value) {}
        explicit Value(std::uint64_t value) : uint64Value(value) {}
        explicit Value(float value) : floatValue(value) {}
        explicit Value(double value) : doubleValue(value) {}
        explicit Value(const char* value) : stringValue(cloneString(value)) {}
    } _value;
};

NS_WYDB_END

#endif // WYDB_TYPED_VALUE_H