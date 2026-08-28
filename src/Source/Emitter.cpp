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
    laser = new Laser(pd);
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
    float angleRad = DEG2RAD(GetWorldRotation());
    float x = sin(angleRad);
    float y = -cos(angleRad);
    Vec2 rightVector = {x, y};

    // Calculate the origin of the laser
    int width = -1;
    int height = -1;
    pd->graphics->getBitmapData(bmp, &width, &height, nullptr, nullptr, nullptr);
    Vec2 origin = {worldPosition.x + (rightVector.x * (width / 2.0f)), worldPosition.y + (rightVector.y * (height / 2.0f))};

    laser->DrawLaser(origin, GetWorldRotation());
}

void Emitter::SetModuleActive(bool activeState)
{
    Module::SetModuleActive(activeState);
}