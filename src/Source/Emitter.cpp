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
    if (bmp == nullptr)
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
    float closestIntersectDistance = laserLength;
    
    // Calculate Right Vector
    float angleRad = GetWorldRotation() * (3.141592653589793 / 180.0f);
    float x = sin(angleRad);
    float y = -cos(angleRad);
    Vec2 rightVector = {x, y};

    // Calculate the origin of the laser
    int width = -1;
    int height = -1;
    pd->graphics->getBitmapData(bmp, &width, &height, nullptr, nullptr, nullptr);
    Vec2 origin = {worldPosition.x + (rightVector.x * (width / 2.0f)), worldPosition.y + (rightVector.y * (height / 2.0f))};

    // Check Collisions before setting the destination. All you should need to change is laser length
    // for (GameObject* go : *gos)
    // {
    //     if (go == this) continue; // Don't hit self;

    //     bool isModule = std::count(go->tags.begin(), go->tags.end(), "module") > 0;
        
    //     if (isModule == false) {
    //         continue;
    //     }

    //     if (CollisionCheck(go, origin, rightVector, closestIntersectDistance) == false) {
    //         continue;
    //     }
        
    //     bool isEmitter = std::count(go->tags.begin(), go->tags.end(), "emitter") > 0;
    //     bool isReceiver = std::count(go->tags.begin(), go->tags.end(), "receiver") > 0;
        
    //     if (isEmitter) {

            
    //     }
    //     else if (isReceiver) {

    //     }
    //     else {

    //     }
    // }

    RaycastHit hit;
    if (Raycast::LineTrace(origin, rightVector, laserLength, hit)) {
        int size = static_cast<int>(hit.hitObject->tags.size());
        if (size >= 0) {
            pd->system->logToConsole("Hitting %d", size);
            bool moduleHit = std::count(hit.hitObject->tags.begin(), hit.hitObject->tags.end(), "module") > 0;
            bool emitterHit = std::count(hit.hitObject->tags.begin(), hit.hitObject->tags.end(), "emitter") > 0;
            bool receiverHit = std::count(hit.hitObject->tags.begin(), hit.hitObject->tags.end(), "receiver") > 0;
    
            if (moduleHit) {
                closestIntersectDistance = hit.distance;
                pd->system->logToConsole("Hitting Module. Distance: %f, laserLength: %f", hit.distance, laserLength);
            } 
        }
    }
    
    Vec2 dest = {origin.x + rightVector.x * closestIntersectDistance, origin.y + rightVector.y * closestIntersectDistance};
    pd->graphics->drawLine(origin.x, origin.y, dest.x, dest.y, 2.5f, kColorBlack);
}

bool Emitter::CollisionCheck(GameObject *go, Vec2 origin, Vec2 rightVector, float& outIntersectDistance)
{
    if (go->IsCircleCollision())
    {
        Vec2 center = go->GetWorldPosition();
        float radius = go->GetCollisionRadius();
        float T;
        if (IntersectCircle(origin, rightVector, center, radius, T))
        {
            if (T < outIntersectDistance) outIntersectDistance = T;
            return true;
        }
    }
    else    // Assume AABB
    {
        outIntersectDistance = outIntersectDistance;
        return false;
    }
    outIntersectDistance = outIntersectDistance;
    return false;
}