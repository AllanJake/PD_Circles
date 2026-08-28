#ifndef VECTOR2_H
#define VECTOR2_H

#pragma once

#include <cmath>

class Vec2 {
    public:
    float x = 0.f;
    float y = 0.f;

    Vec2() : x(0.0f), y(0.0f) {}
    Vec2(float x, float y) : x(x), y(y) {}
    Vec2(int x, int y) : x(x), y(y) {}

    float Magnitude() const {
        return sqrtf(x * x + y * y);
    }

    float SqrMagnitude() const {
        return x * x + y * y;
    }

    Vec2 Normalized() const {
        float magnitude = Magnitude();

        if (magnitude == 0.f) {
            return Vec2(0.f, 0.f);
        }

        return Vec2 (x / magnitude, y / magnitude);
    }

    void Normalize() 
    {
        float magnitude = Magnitude();

        if (magnitude == 0.f) {
            return;
        }

        x /= magnitude;
        y /= magnitude;
    }

    float Dot(const Vec2& other) const {
        return x * other.x + y * other.y;
    }

    float DistanceTo(const Vec2& other) const {
        return (*this - other).Magnitude();
    }

    float SqrDistanceTo(const Vec2& other) const {
        return (*this - other).SqrMagnitude();
    }

    Vec2 operator+(const Vec2& other) const {
        return Vec2(x + other.x, y + other.y);
    }

    Vec2 operator-(const Vec2& other) const {
        return Vec2(x - other.x, y - other.y);
    }

    Vec2 operator*(float scalar) const {
        return Vec2(x * scalar, y * scalar);
    }
    
    Vec2 operator/(float scalar) const {
        return Vec2(x / scalar, y / scalar);
    }

    Vec2& operator+=(const Vec2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vec2& operator-=(const Vec2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vec2& operator*=(float scalar) {
        x += scalar;
        y += scalar;
        return *this;
    }

    Vec2& operator-=(float scalar) {
        x -= scalar;
        y -= scalar;
        return *this;
    }

    bool operator==(const Vec2& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Vec2& other) const {
        return !(*this == other);
    }
};

#endif