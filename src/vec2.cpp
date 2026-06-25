#include "vec2.hpp"
#include <math.h>


Vec2::Vec2(){
    this->x=0;
    this->y=0;
}

Vec2::Vec2(float x, float y) : x(x), y(y){}

const Vec2 Vec2::zero = Vec2(0.0f, 0.0f);

/**
 * Computes the length of the vector
 * 
 * @return float that contains the length of the vector
 */
float Vec2::length() const{
    return sqrtf(x*x+y*y);
}

/**
 * Computes the distance between this vector and v
 * 
 * @param v vector to compute distance to
 */
float Vec2::dist(const Vec2& v) const{
    return (v-*this).length();
}

/**
 * Normalizes the vector to length 1
 * 
 * @return Vec2 with length 1
 */
Vec2 Vec2::normalize() const{
    float l=length();
    return Vec2(x/l,y/l);
}

//Operator overloads

bool Vec2::operator ==(const Vec2& rhs) const{
    return (rhs.x==x && rhs.y==y);
}
Vec2 Vec2::operator + (const Vec2& rhs) const{
    return Vec2(x+rhs.x,y+rhs.y);
}
Vec2 Vec2::operator * (float value) const{
    return Vec2(x*value,y*value);
}
void Vec2::operator +=(const Vec2& rhs){
    x+=rhs.x;
    y+=rhs.y;
}
Vec2 Vec2::operator -(const Vec2& rhs) const{
    return Vec2(x-rhs.x, y-rhs.y);
}
void Vec2::operator -=(const Vec2& rhs){
    x-=rhs.x;
    y-=rhs.y;
}
float Vec2::operator *(const Vec2& rhs) const{
    return x*rhs.x+y*rhs.y;
}
void Vec2::operator *=(const Vec2& rhs){
    x=x*rhs.x;
    y=y*rhs.y;
}