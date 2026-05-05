#pragma once
#include <math.h>
#include <iostream>

class Vec2{
    public:
        float x;
        float y;

        Vec2();
        Vec2(float x, float y);

        float length();

        Vec2 operator+ (const Vec2& rhs) const;
        void operator+= (const Vec2& rhs);
        Vec2 operator* (const float s) const;
};

Vec2 operator* (const float s, const Vec2& rhs);
std::ostream& operator<< (std::ostream& os, const Vec2& v);

struct rect{
    float height;
    float width;
};

template<typename T>
class Shape{
    public:
        T size;
};