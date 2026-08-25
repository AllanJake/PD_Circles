#include "../Header/Emitter.h"

Emitter::Emitter(PlaydateAPI* pd)
    : Module(pd)
{

}

Emitter::~Emitter()
{

}

void Emitter::Init(float startAngle)
{
    Module::Init(startAngle);

    // Offset the local position so the emitter draws in the correct place.
    // This would be better served as a GameObject Pivot position but this works also.
    localPosition.y -= 3.0f;
}

void Emitter::LoadImage()
{
    Module::LoadModuleImage(imagePath);
}

void Emitter::Update()
{
    GameObject::Update();
    worldPosition = GetWorldPosition();
}

void Emitter::Draw()
{
    GameObject::Draw();
    if (bmp == NULL)
    {
        //pd->system->logToConsole("NO FUCKING BMP");
        return;
    }
    float rot = GetWorldRotation();
    pd->graphics->drawRotatedBitmap(bmp, worldPosition.x, worldPosition.y, rot, 0.5, 0.5, 1, 1);
    DrawLaser();
}

void Emitter::DrawLaser()
{
    float laserLength = 120.0f;
    float closestT = laserLength;
    
    // Calculate Right Vector
    float angleRad = GetWorldRotation() * (3.141592653589793 / 180.0f);
    float x = sin(angleRad);
    float y = -cos(angleRad);
    Vec2 rightVector = {x, y};

    // Calculate the origin of the laser
    int width = -1;
    int height = -1;
    pd->graphics->getBitmapData(bmp, &width, &height, nullptr, nullptr, nullptr);
    Vec2 origin = {worldPosition.x + (rightVector.x * (width / 2.0)), worldPosition.y + (rightVector.y * (height / 2.0))};

    // Check Collisions before setting the destination. All you should need to change is laser length
    for (GameObject* go : gos)
    {
        if (go == this) continue; // Don't hit self;

        for (std::string tag : go->tags)
        {
            if (tag == "module")
            {
                if (go->IsCircleCollision())
                {
                    Vec2 center = go->GetWorldPosition();
                    float radius = go->GetCollisionRadius();
                    float t;
                    if (IntersectCircle(origin, rightVector, center, radius, t))
                    {
                        if (t < closestT) closestT = t;
                    }
                }
                else    // Assume AABB
                {

                }
            }
        }
    }
    
    Vec2 dest = {origin.x + rightVector.x * closestT, origin.y + rightVector.y * closestT};
    pd->graphics->drawLine(origin.x, origin.y, dest.x, dest.y, 2.5f, kColorBlack);
}

bool Emitter::IntersectCircle(const Vec2& origin, const Vec2& dir, const Vec2& center, float radius, float& outT)
{    
    Vec2 oc = {origin.x - center.x, origin.y - center.y};
    float b = 2.0f * (oc.x * dir.x + oc.y * dir.y);
    float c = (oc.x * oc.x + oc.y * oc.y) - radius * radius;
    float discriminant = b * b - 4 * c;

    if (discriminant < 0) return false;
    float sqrtDisc = sqrt(discriminant);
    float t1 = (-b - sqrtDisc) / 2.0f;
    float t2 = (-b + sqrtDisc) / 2.0f;

    if (t1 >= 0) {
        outT = t1;
        return true;
    }
    if (t2 >= 0) {
        outT = t2;
        return true;
    }
    return false;
}

bool Emitter::IntersectAABB(const Vec2& origin, const Vec2& dir, const Vec2& min, const Vec2& max, float& outT)
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

    outT = (tMin >= 0) ? tMin : tMax;
    return outT >= 0;
}