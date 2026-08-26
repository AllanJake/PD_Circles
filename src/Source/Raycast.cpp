#include "../Header/Raycast.h"
#include <algorithm>

std::vector<GameObject*>* Raycast::gameObjects = nullptr;
bool Raycast::LineTrace(const Vec2 &origin, const Vec2 &direction, float maxDistance, RaycastHit &outHit)
{
    bool hasHit = false;
    float closestDistance = maxDistance;
    if (gameObjects == nullptr) {
        return false;
    }

    for (GameObject* go : *gameObjects) {
        if (static_cast<int>(go->tags.size()) == 0) continue;
        float distance;
        Vec2 center = go->GetWorldPosition();
        float radius = go->GetCollisionRadius();

        if (IntersectCircle(origin, direction, center, radius, distance)) {
            if (distance <= closestDistance) {
                closestDistance = distance;
                hasHit = true;

                outHit.hitObject = go;
                outHit.distance = distance;
                outHit.point = {
                    origin.x + direction.x * distance,
                    origin.y + direction.y * distance
                };
                outHit.normal = (outHit.point - center).Normalized();
            }
        }
    }
    
    return hasHit;
}

bool Raycast::IntersectCircle(const Vec2 &origin, const Vec2 &dir, const Vec2 &center, float radius, float &outIntersectDistance)
{
    Vec2 oc = {origin.x - center.x, origin.y - center.y};
    float b = 2.0f * (oc.x * dir.x + oc.y * dir.y);
    float c = (oc.x * oc.x + oc.y * oc.y) - radius * radius;
    float discriminant = b * b - 4 * c;

    if (discriminant < 0) return false;
    float sqrtDisc = sqrt(discriminant);
    float nearIntersectDistance = (-b - sqrtDisc) / 2.0f;
    float farIntersectDistance = (-b + sqrtDisc) / 2.0f;

    if (nearIntersectDistance >= 0) {
        outIntersectDistance = nearIntersectDistance;
        return true;
    }
    if (farIntersectDistance >= 0) {
        outIntersectDistance = farIntersectDistance;
        return true;
    }
    return false;
}

bool Raycast::IntersectAABB(const Vec2 &origin, const Vec2 &dir, const Vec2 &min, const Vec2 &max, float &outIntersectDistance)
{
    float tMin = (min.x - origin.x) / dir.x;
    float tMax = (max.x - origin.x) / dir.x;
    if (tMin > tMax) std::swap(tMin, tMax);

    float tyMin = (min.y - origin.y) / dir.y;
    float tyMax = (max.y - origin.y) / dir.y;
    if (tyMin > tyMax) std::swap(tyMin, tyMax);

    if ((tMin > tyMax) || (tyMin > tMax)) return false;

    tMin = std::max(tMin, tyMin);
    tMax = std::min(tMax, tyMax);

    if (tMax < 0) return false;

    outIntersectDistance = (tMin >= 0) ? tMin : tMax;
    return outIntersectDistance >= 0;
}
