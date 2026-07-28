///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2024 WangYao. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WY_MATH_H
#define WY_MATH_H

#include <cmath>
#include <cassert>
#include <wyDefs.h>

NS_WY_BEG

// epsilon
const double EPS = 1e-10;
const double EPS_SQUARED = 1e-20;

#ifdef PI
#undef PI
#undef PI_2
#undef PI_4
#undef TWO_PI
#endif

const double PI     = 3.14159265358979323846;
const double PI_2   = 1.57079632679489661923;
const double PI_4   = 0.78539816339744830962;
const double TWO_PI = 6.28318530717958647692;

const double E      =  2.71828182845904523536;   // e
const double LOG2E  =  1.44269504088896340736;   // log2(e)
const double LOG10E =  0.434294481903251827651;  // log10(e)
const double LN2    =  0.693147180559945309417;  // ln(2)
const double LN10   =  2.30258509299404568402;   // ln(10)

inline double degreesToRadians(double angle)
{
    return angle * PI / 180.0;
}

inline double radiansToDegrees(double rad)
{
    return rad * 180.0 / PI;
}

// Normalizes a radian value to the range [0, 2PI).
// @param rad Radian value to normalize. Must be finite.
// @return The normalized radian value in the range [0, 2PI).
static inline double normalizeRadian(double rad)
{
    assert(std::isfinite(rad));

    double value = std::fmod(rad, TWO_PI);
    if (value < 0.0)
    {
        value += TWO_PI;
        if (value == TWO_PI)
        {
            value = 0.0;
        }
    }
    return value;
}

// Computes the counterclockwise angular difference from startAngle to endAngle.
//
// @param startAngle Start angle in radians. Must be finite.
// @param endAngle End angle in radians. Must be finite.
//
// @return The counterclockwise angular difference in the range [0, 2PI).
static inline double computeTotalAngle(double startAngle, double endAngle)
{
    startAngle = normalizeRadian(startAngle);
    endAngle = normalizeRadian(endAngle);
    if (endAngle < startAngle)
    {
        endAngle += TWO_PI;
    }
    double totalAngle = endAngle - startAngle;
    if (totalAngle == TWO_PI)
    {
        totalAngle = 0.0;
    }
    return totalAngle;
}

// Converts an ellipse polar angle in radians to a parametric angle in radians.
//
// @param polarRad Polar angle in radians.
// @param a Semi-major axis length. Must be greater than 0.
// @param b Semi-minor axis length. Must be greater than 0.
//
// @return The parametric angle normalized to the range [0, 2PI).
static inline double ellipsePolarAngleToParametricAngle(double polarRad, double a, double b)
{
    assert(a > 0.0);
    assert(b > 0.0);
    double theta = std::atan2(a * std::sin(polarRad), b * std::cos(polarRad));
    if (theta < 0.0)
    {
        theta += TWO_PI;
        if (theta == TWO_PI)
        {
            theta = 0.0;
        }
    }
    return theta;
}

NS_WY_END

#endif // WY_MATH_H