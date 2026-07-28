///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WY_VECTOR2_H
#define WY_VECTOR2_H

#include <cmath>
#include <algorithm>
#include <cassert>
#include <wyDefs.h>
#include <wyMath.h>

NS_WY_BEG

class WY_EXPORT Vector2
{
public:
    Vector2() : _x(0.0), _y(0.0) {}
    Vector2(double x, double y) : _x(x), _y(y) {}

    static const Vector2 kZero;
    static const Vector2 kXAxis;
    static const Vector2 kYAxis;

    inline double x() const { return _x; }
    inline double y() const { return _y; }

    inline void set(double x, double y) { _x = x; _y = y; }
    inline void setX(double x) { _x = x; }
    inline void setY(double y) { _y = y; }

    inline double length() const { return std::sqrt(_x * _x + _y * _y); }
    inline double length2() const { return _x * _x + _y * _y; }

    inline double normalize()
    {
        double length = this->length();
        if (length >= EPS)
        {
            double inv = 1.0 / length;
            _x *= inv;
            _y *= inv;
        }
        return length;
    }

    inline Vector2 normalized() const
    {
        double length = this->length();
        if (length >= EPS)
        {
            double inv = 1.0 / length;
            return Vector2(_x * inv, _y * inv);
        }
        return *this;
    }

    // Dot product.
    inline double dot(const Vector2& rhs) const
    {
        return _x * rhs._x + _y * rhs._y;
    }

    // Cross product.
    inline double cross(const Vector2& rhs) const
    {
        return _x * rhs._y - _y * rhs._x;
    }

    inline Vector2& operator+=(const Vector2& rhs);
    inline Vector2& operator-=(const Vector2& rhs);
    inline Vector2& operator*=(double scalar);
    inline Vector2& operator/=(double scalar);

    friend inline bool operator==(const Vector2& lhs, const Vector2& rhs);
    friend inline bool operator!=(const Vector2& lhs, const Vector2& rhs);
    friend inline bool operator<(const Vector2& lhs, const Vector2& rhs);
    friend inline const Vector2 operator+(const Vector2& lhs, const Vector2& rhs);
    friend inline const Vector2 operator-(const Vector2& lhs, const Vector2& rhs);
    friend inline const Vector2 operator*(double factor, const Vector2& rhs);
    friend inline const Vector2 operator*(const Vector2& lhs, double factor);
    friend inline const Vector2 operator/(const Vector2& lhs, double divisor);
    friend inline const Vector2 operator-(const Vector2& vec);

    // Returns the angle between two vectors in the range [0, PI].
    // Returns 0.0 if either vector is zero.
    inline static double angle(const Vector2& lhs, const Vector2& rhs);

    // Returns the counterclockwise rotation angle from one vector to another in the range [0, 2PI).
    inline static double rotationAngle(const Vector2& from, const Vector2& to);

private:
    double _x = 0.0;
    double _y = 0.0;
};

inline Vector2& Vector2::operator+=(const Vector2& rhs)
{
    _x += rhs._x;
    _y += rhs._y;
    return *this;
}

inline Vector2& Vector2::operator-=(const Vector2& rhs)
{
    _x -= rhs._x;
    _y -= rhs._y;
    return *this;
}

inline Vector2& Vector2::operator*=(double scalar)
{
    _x *= scalar;
    _y *= scalar;
    return *this;
}

inline Vector2& Vector2::operator/=(double scalar)
{
    assert(scalar != 0.0);
    _x /= scalar;
    _y /= scalar;
    return *this;
}

inline bool operator==(const Vector2& lhs, const Vector2& rhs)
{
    return lhs._x == rhs._x && lhs._y == rhs._y;
}

inline bool operator!=(const Vector2& lhs, const Vector2& rhs)
{
    return lhs._x != rhs._x || lhs._y != rhs._y;
}

inline bool operator<(const Vector2& lhs, const Vector2& rhs)
{
    if (lhs._x < rhs._x) return true;
    else if (lhs._x > rhs._x) return false;
    else return (lhs._y < rhs._y);
}

inline const Vector2 operator+(const Vector2& lhs, const Vector2& rhs)
{
    return Vector2(lhs._x + rhs._x, lhs._y + rhs._y);
}

inline const Vector2 operator-(const Vector2& lhs, const Vector2& rhs)
{
    return Vector2(lhs._x - rhs._x, lhs._y - rhs._y);
}

inline const Vector2 operator*(double factor, const Vector2& rhs)
{
    return Vector2(rhs._x * factor, rhs._y * factor);
}

inline const Vector2 operator*(const Vector2& lhs, double factor)
{
    return Vector2(lhs._x * factor, lhs._y * factor);
}

inline const Vector2 operator/(const Vector2& lhs, double divisor)
{
    assert(divisor != 0.0);
    return Vector2(lhs._x / divisor, lhs._y / divisor);
}

inline const Vector2 operator-(const Vector2& vec)
{
    return Vector2(-vec._x, -vec._y);
}

// Returns the angle between two vectors in the range [0, PI].
// Returns 0.0 if either vector is zero.
inline double Vector2::angle(const Vector2& lhs, const Vector2& rhs)
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

// Returns the counterclockwise rotation angle from one vector to another in the range [0, 2PI).
inline double Vector2::rotationAngle(const Vector2& from, const Vector2& to)
{
    double angle = std::atan2(from.cross(to), from.dot(to)); // [-PI, PI]
    if (angle < 0.0)
    {
        angle += TWO_PI;
        if (angle == TWO_PI)
        {
            angle = 0.0;
        }
    }
    return angle;
}

NS_WY_END

#endif // WY_VECTOR2_H
