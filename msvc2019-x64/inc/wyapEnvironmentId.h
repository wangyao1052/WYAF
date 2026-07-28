#ifndef WYAP_ENVIRONMENT_ID_H
#define WYAP_ENVIRONMENT_ID_H

#include <cstdint>
#include <functional>

#include <wyapDefs.h>

NS_WYAP_BEG

class EnvironmentId
{
public:
    explicit constexpr EnvironmentId(std::uint32_t value = 0) noexcept : _value(value) {}
    ~EnvironmentId() = default;
    EnvironmentId(const EnvironmentId&) = default;
    EnvironmentId& operator=(const EnvironmentId&) = default;
    EnvironmentId(EnvironmentId&&) = default;
    EnvironmentId& operator=(EnvironmentId&&) = default;

    static const EnvironmentId kNull;

    constexpr std::uint32_t value() const noexcept
    {
        return _value;
    }

    constexpr bool isNull() const noexcept
    {
        return _value == 0;
    }

    friend constexpr bool operator==(EnvironmentId lhs, EnvironmentId rhs) noexcept
    {
        return lhs._value == rhs._value;
    }

    friend constexpr bool operator!=(EnvironmentId lhs, EnvironmentId rhs) noexcept
    {
        return lhs._value != rhs._value;
    }

    friend constexpr bool operator<(EnvironmentId lhs, EnvironmentId rhs) noexcept
    {
        return lhs._value < rhs._value;
    }

private:
    std::uint32_t _value = 0;
};

inline const EnvironmentId EnvironmentId::kNull{ 0 };

NS_WYAP_END

namespace std
{
    template<>
    struct hash<wyap::EnvironmentId>
    {
        size_t operator()(const wyap::EnvironmentId& id) const noexcept
        {
            return std::hash<std::uint32_t>{}(id.value());
        }
    };
}

#endif // WYAP_ENVIRONMENT_ID_H