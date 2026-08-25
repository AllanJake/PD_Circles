#include "../Header/Receiver.h"

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
}

void Receiver::Draw()
{
    GameObject::Draw();

    if (bmp == nullptr) {
        pd->system->logToConsole("Failed to draw bmp: %c", imagePath);
        return;
    }

    float rot = GetWorldRotation();
    pd->graphics->drawRotatedBitmap(bmp, worldPosition.x, worldPosition.y, rot, 0.5, 0.5, 1, 1);
}