#include "vec2.hpp"

Vec2::Vec2(): x(0), y(0){}
Vec2::Vec2(float x, float y): x(x), y(y){}

float Vec2::length(){
    return sqrtf(x*x+y*y);
}

Vec2 Vec2::operator+ (const Vec2& rhs) const{
    return Vec2(rhs.x+x, rhs.y+y);
}

void Vec2::operator+= (const Vec2& rhs){
    x+=rhs.x;
    y+=rhs.y;
 }

Vec2 Vec2::operator* (const float s) const{
    return Vec2(x*s,y*s);
  }

Vec2 operator* (const float s, const Vec2& rhs){
    Vec2::foo++;
    return Vec2(rhs.x*s, rhs.y*s);
}

std::ostream& operator<< (std::ostream& os, const Vec2& v){
    os<<"("<<v.x<<","<<v.y<<")";
    return os;
}