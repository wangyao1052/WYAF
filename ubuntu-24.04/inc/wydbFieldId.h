///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2026 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYDB_FIELD_ID_H
#define WYDB_FIELD_ID_H

#include <cstdint>
#include <cassert>
#include <wydbDefs.h>

NS_WYDB_BEG

class WYDB_EXPORT FieldId
{
public:
    constexpr FieldId() noexcept : _value{ 0ULL } {}
    explicit constexpr FieldId(std::uint64_t value) noexcept
        : _value(value) {}
    ~FieldId() = default;
    constexpr FieldId(const FieldId&) noexcept = default;
    constexpr FieldId& operator=(const FieldId&) noexcept = default;
    constexpr FieldId(FieldId&&) noexcept = default;
    constexpr FieldId& operator=(FieldId&&) noexcept = default;

    static constexpr FieldId make(const char* s) noexcept
    {
        if (!s)
        {
            return FieldId();
        }
        std::uint64_t hash = 14695981039346656037ull;
        while (*s != '\0')
        {
            hash ^= static_cast<unsigned char>(*s++);
            hash *= 1099511628211ull;
        }
        return FieldId(hash);
    }

    constexpr std::uint64_t value() const noexcept
    {
        return _value;
    }

    friend constexpr bool operator==(const FieldId& lhs, const FieldId& rhs) noexcept
    {
        return lhs._value == rhs._value;
    }
    friend constexpr bool operator!=(const FieldId& lhs, const FieldId& rhs) noexcept
    {
        return lhs._value != rhs._value;
    }
    friend constexpr bool operator<(const FieldId& lhs, const FieldId& rhs) noexcept
    {
        return lhs._value < rhs._value;
    }
    friend constexpr bool operator<=(const FieldId& lhs, const FieldId& rhs) noexcept
    {
        return lhs._value <= rhs._value;
    }
    friend constexpr bool operator>(const FieldId& lhs, const FieldId& rhs) noexcept
    {
        return lhs._value > rhs._value;
    }
    friend constexpr bool operator>=(const FieldId& lhs, const FieldId& rhs) noexcept
    {
        return lhs._value >= rhs._value;
    }

private:
    std::uint64_t _value{};
};

NS_WYDB_END

#endif // WYDB_FIELD_ID_H