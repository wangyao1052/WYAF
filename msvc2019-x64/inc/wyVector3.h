///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WY_VECTOR3_H
#define WY_VECTOR3_H

#include <cmath>
#include <algorithm>
#include <cassert>
#include <wyDefs.h>
#include <wyMath.h>

NS_WY_BEG

class WY_EXPORT Vector3
{
public:
    Vector3() : _x(0.0), _y(0.0), _z(0.0) {}
    Vector3(double x, double y, double z) : _x(x), _y(y), _z(z) {}

    static const Vector3 kZero;
    static const Vector3 kXAxis;
    static const Vector3 kYAxis;
    static const Vector3 kZAxis;

    inline double x() const { return _x; }
    inline double y() const { return _y; }
    inline double z() const { return _z; }

    inline void set(double x, double y, double z) { _x = x; _y = y; _z = z; }
    inline void setX(double x) { _x = x; }
    inline void setY(double y) { _y = y; }
    inline void setZ(double z) { _z = z; }

    inline double length() const { return std::sqrt(_x * _x + _y * _y + _z * _z); }
    inline double length2() const { return _x * _x + _y * _y + _z * _z; }

    inline double normalize()
    {
        double length = this->length();
        if (length >= EPS)
        {
            double inv = 1.0 / length;
            _x *= inv;
            _y *= inv;
            _z *= inv;
        }
        return length;
    }

    inline Vector3 normalized() const
    {
        double length = this->length();
        if (length >= EPS)
        {
            double inv = 1.0 / length;
            return Vector3(_x * inv, _y * inv, _z * inv);
        }
        return *this;
    }

    // Dot product.
    inline double dot(const Vector3& rhs) const
    {
        return _x * rhs._x + _y * rhs._y + _z * rhs._z;
    }

    // Cross product.
    inline const Vector3 cross(const Vector3& rhs) const
    {
        return Vector3(
            _y * rhs._z - _z * rhs._y,
            _z * rhs._x - _x * rhs._z,
            _x * rhs._y - _y * rhs._x);
    }

    inline Vector3& operator+=(const Vector3& rhs);
    inline Vector3& operator-=(const Vector3& rhs);
    inline Vector3& operator*=(double scalar);
    inline Vector3& operator/=(double scalar);

    friend inline bool operator==(const Vector3& lhs, const Vector3& rhs);
    friend inline bool operator!=(const Vector3& lhs, const Vector3& rhs);
    friend inline bool operator<(const Vector3& lhs, const Vector3& rhs);
    friend inline const Vector3 operator+(const Vector3& lhs, const Vector3& rhs);
    friend inline const Vector3 operator-(const Vector3& lhs, const Vector3& rhs);
    friend inline const Vector3 operator*(double factor, const Vector3& rhs);
    friend inline const Vector3 operator*(const Vector3& lhs, double factor);
    friend inline const Vector3 operator/(const Vector3& lhs, double divisor);
    friend inline const Vector3 operator-(const Vector3& vec);

    // Returns the angle between two vectors in the range [0, PI].
    inline static double angle(const Vector3& lhs, const Vector3& rhs);

private:
    double _x = 0.0;
    double _y = 0.0;
    double _z = 0.0;
};

inline Vector3& Vector3::operator+=(const Vector3& rhs)
{
    _x += rhs._x;
    _y += rhs._y;
    _z += rhs._z;
    return *this;
}

inline Vector3& Vector3::operator-=(const Vector3& rhs)
{
    _x -= rhs._x;
    _y -= rhs._y;
    _z -= rhs._z;
    return *this;
}

inline Vector3& Vector3::operator*=(double scalar)
{
    _x *= scalar;
    _y *= scalar;
    _z *= scalar;
    return *this;
}

inline Vector3& Vector3::operator/=(double scalar)
{
    assert(scalar != 0.0);
    _x /= scalar;
    _y /= scalar;
    _z /= scalar;
    return *this;
}

inline bool operator==(const Vector3& lhs, const Vector3& rhs)
{
    return lhs._x == rhs._x && lhs._y == rhs._y && lhs._z == rhs._z;
}

inline bool operator!=(const Vector3& lhs, const Vector3& rhs)
{
    return lhs._x != rhs._x || lhs._y != rhs._y || lhs._z != rhs._z;
}

inline bool operator<(const Vector3& lhs, const Vector3& rhs)
{
    if (lhs._x < rhs._x) return true;
    else if (lhs._x > rhs._x) return false;
    else if (lhs._y < rhs._y) return true;
    else if (lhs._y > rhs._y) return false;
    else return (lhs._z < rhs._z);
}

inline const Vector3 operator+(const Vector3& lhs, const Vector3& rhs)
{
    return Vector3(lhs._x + rhs._x, lhs._y + rhs._y, lhs._z + rhs._z);
}

inline const Vector3 operator-(const Vector3& lhs, const Vector3& rhs)
{
    return Vector3(lhs._x - rhs._x, lhs._y - rhs._y, lhs._z - rhs._z);
}

inline const Vector3 operator*(double factor, const Vector3& rhs)
{
    return Vector3(rhs._x * factor, rhs._y * factor, rhs._z * factor);
}

inline const Vector3 operator*(const Vector3& lhs, double factor)
{
    return Vector3(lhs._x * factor, lhs._y * factor, lhs._z * factor);
}

inline const Vector3 operator/(const Vector3& lhs, double divisor)
{
    assert(divisor != 0.0);
    return Vector3(lhs._x / divisor, lhs._y / divisor, lhs._z / divisor);
}

inline const Vector3 operator-(const Vector3& vec)
{
    return Vector3(-vec._x, -vec._y, -vec._z);
}

// Returns the angle between two vectors in the range [0, PI].
inline double Vector3::angle(const Vector3& lhs, const Vector3& rhs)
{
    double len1 = lhs.length();
    double len2 = rhs.length();
    if (len1 <= EPS || len2 <= EPS) // zero vector case
    {
        return 0.0;
    }
    double cosine = std::clamp(lhs.dot(rhs) / (len1 * len2), -1.0, 1.0);
    return std::acos(cosine);
}

NS_WY_END

#endif // WY_VECTOR3_H
