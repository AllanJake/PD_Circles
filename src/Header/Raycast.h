#ifndef RAYCAST_H
#define RAYCAST_H

#pragma once

#include "Vector2.h"
#include "GameObject.h"

struct RaycastHit {
    GameObject* hitObject = nullptr;

    Vec2 point = {0.f, 0.f};
    Vec2 normal = {0.f, 0.f};

    float distance = 0.f;
};

class Raycast {
    public:
    static bool LineTrace(const Vec2& origin, const Vec2& direction, float maxDistance, RaycastHit& outHit);

    static std::vector<GameObject*>* gameObjects;

    private:
    static bool IntersectCircle(const Vec2& origin, const Vec2& dir, const Vec2& center, float radius, float& outIntersectDistance);
    static bool IntersectAABB(const Vec2& origin, const Vec2& dir, const Vec2& min, const Vec2& max, float& outIntersectDistance);
};

#endif