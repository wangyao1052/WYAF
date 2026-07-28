///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WYDB_ELEMENT_ID_H
#define WYDB_ELEMENT_ID_H

#include <cstdint>
#include <wydbDefs.h>

NS_WYDB_BEG

class WYDB_EXPORT ElementId
{
public:
    ElementId() : _value(0) {}
    explicit ElementId(std::uint64_t value) : _value(value) {}
    ~ElementId() = default;
    ElementId(const ElementId&) = default;
    ElementId& operator=(const ElementId&) = default;
    ElementId(ElementId&&) = default;
    ElementId& operator=(ElementId&&) = default;

    static const ElementId kNull;

    inline bool isNull() const
    {
        return _value == 0;
    }

    inline std::uint64_t value() const
    {
        return _value;
    }

    friend inline bool operator==(const ElementId& lhs, const ElementId& rhs)
    {
        return lhs._value == rhs._value;
    }
    friend inline bool operator!=(const ElementId& lhs, const ElementId& rhs)
    {
        return lhs._value != rhs._value;
    }
    friend inline bool operator<(const ElementId& lhs, const ElementId& rhs)
    {
        return lhs._value < rhs._value;
    }
    friend inline bool operator<=(const ElementId& lhs, const ElementId& rhs)
    {
        return lhs._value <= rhs._value;
    }
    friend inline bool operator>(const ElementId& lhs, const ElementId& rhs)
    {
        return lhs._value > rhs._value;
    }
    friend inline bool operator>=(const ElementId& lhs, const ElementId& rhs)
    {
        return lhs._value >= rhs._value;
    }

private:
    std::uint64_t _value;
};

NS_WYDB_END

#endif // WYDB_ELEMENT_ID_H