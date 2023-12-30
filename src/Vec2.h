#pragma once

#include <math.h>
#include <iostream>

class Vec2
{
public:

    float x;
    float y;

    Vec2(float x, float y) :
        x(x),
        y(y)
    {}

    static const Vec2 UP;
    static const Vec2 DOWN;
    static const Vec2 LEFT;
    static const Vec2 RIGHT;
    static const Vec2 ZERO;

    bool operator == (const Vec2& rhs) const;
    bool operator != (const Vec2& rhs) const;

    Vec2 operator + (const Vec2& rhs) const;
    Vec2 operator - (const Vec2& rhs) const;
    Vec2 operator * (const float value) const;
    Vec2 operator / (const float value) const;

    void operator += (const Vec2& rhs);
    void operator -= (const Vec2& rhs);
    void operator *= (const float value);
    void operator /= (const float value);

    float dist(const Vec2& rhs) const;
    float dot(const Vec2& rhs) const;
    float length() const;

    Vec2& normalize();
    Vec2& scale(const float value);
    Vec2& rotate(const float angle);
    Vec2& flipX();
    Vec2& flipY();

    friend std::ostream& operator << (std::ostream& os, const Vec2& v);
};