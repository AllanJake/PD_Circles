#include "../Header/Emitter.h"
#include "../Header/Receiver.h"

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

    RaycastHit hit;
    if (Raycast::LineTrace(origin, rightVector, laserLength, hit)) {
        int size = static_cast<int>(hit.hitObject->tags.size());
        if (size >= 0) {
            bool moduleHit = std::count(hit.hitObject->tags.begin(), hit.hitObject->tags.end(), "module") > 0;
            bool emitterHit = std::count(hit.hitObject->tags.begin(), hit.hitObject->tags.end(), "emitter") > 0;
            bool receiverHit = std::count(hit.hitObject->tags.begin(), hit.hitObject->tags.end(), "receiver") > 0;
            
            if (moduleHit) {
                closestIntersectDistance = hit.distance;
                Module* hitModule = dynamic_cast<Module*>(hit.hitObject);
                if (receiverHit) {
                    hitModule->SetModuleActive(true);
                    lastModule = hitModule;
                }
            }             
        }
    }
    else {
        if (lastModule != nullptr) {
            lastModule->SetModuleActive(false);
            lastModule = nullptr;
        }
    }
    
    Vec2 dest = {origin.x + rightVector.x * closestIntersectDistance, origin.y + rightVector.y * closestIntersectDistance};
    pd->graphics->drawLine(origin.x, origin.y, dest.x, dest.y, 2.5f, kColorBlack);
}

void Emitter::SetModuleActive(bool activeState)
{
    Module::SetModuleActive(activeState);
}