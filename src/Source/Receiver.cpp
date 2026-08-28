#include "../Header/Receiver.h"
#include <cmath>

Receiver::Receiver(PlaydateAPI* pd)
    : Module(pd)
{

}

Receiver::~Receiver()
{
}

void Receiver::Init(float startAngle)
{
    Module::Init(startAngle);
}

void Receiver::LoadImage()
{
    Module::LoadModuleImage(imagePath);
}

void Receiver::Update()
{
    GameObject::Update();
    worldPosition = GetWorldPosition();
    UpdateReceiver();
}

void Receiver::Draw()
{
    GameObject::Draw();

    if (bmp == nullptr) {
        pd->system->logToConsole("Failed to draw bmp: %c", imagePath);
        return;
    }

    float rot = GetWorldRotation();
    // pd->graphics->drawBitmap(bmp, worldPosition.x, worldPosition.y, LCDBitmapFlip::kBitmapUnflipped);
    pd->graphics->drawRotatedBitmap(bmp, worldPosition.x, worldPosition.y, 0.0f, 0.5, 0.5, 1, 1);

    if (hasLaser) {
        pd->graphics->drawEllipse(worldPosition.x - (chargeDiameter / 2), worldPosition.y - (chargeDiameter / 2), 
                                    chargeDiameter, chargeDiameter, 
                                    chargeThickness, 
                                    0, chargeAngle, kColorBlack);
    }
}

void Receiver::SetModuleActive(bool activeState)
{
    if (hasLaser == false && activeState == true) {
        pd->system->resetElapsedTime();
    }
    hasLaser = activeState;
}

void Receiver::UpdateReceiver()
{
    if (!hasLaser) {
        // Reset the receiver
        chargeAngle = 0.0f;
        complete = false;
        return;
    }

    float t = pd->system->getElapsedTime() / chargeTime;

    if (t < 1.0f) {
        chargeAngle = std::lerp(0, 359, t);
    }
    else {
        chargeAngle = 360.0f;
        complete = true;
    }
    
}
