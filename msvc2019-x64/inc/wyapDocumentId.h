#ifndef WYAP_DOCUMENT_ID_H
#define WYAP_DOCUMENT_ID_H

#include <cstdint>
#include <functional>

#include <wyapDefs.h>

NS_WYAP_BEG

class WYAP_EXPORT DocumentId
{
public:
    explicit constexpr DocumentId(std::uint32_t value = 0) noexcept : _value(value) {}
    ~DocumentId() = default;
    DocumentId(const DocumentId&) = default;
    DocumentId& operator=(const DocumentId&) = default;
    DocumentId(DocumentId&&) = default;
    DocumentId& operator=(DocumentId&&) = default;

    static const DocumentId kNull;

    constexpr std::uint32_t value() const noexcept
    {
        return _value;
    }

    constexpr bool isNull() const noexcept
    {
        return _value == 0;
    }

    friend constexpr bool operator==(DocumentId lhs, DocumentId rhs) noexcept
    {
        return lhs._value == rhs._value;
    }

    friend constexpr bool operator!=(DocumentId lhs, DocumentId rhs) noexcept
    {
        return lhs._value != rhs._value;
    }

    friend constexpr bool operator<(DocumentId lhs, DocumentId rhs) noexcept
    {
        return lhs._value < rhs._value;
    }

private:
    std::uint32_t _value = 0;
};

NS_WYAP_END

namespace std
{
    template<>
    struct hash<wyap::DocumentId>
    {
        size_t operator()(const wyap::DocumentId& id) const noexcept
        {
            return std::hash<std::uint32_t>{}(id.value());
        }
    };
}

#endif // WYAP_DOCUMENT_ID_H