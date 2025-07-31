#include "../Header/Emitter.h"

Emitter::Emitter(PlaydateAPI* pd)
    : Module(pd)
{
    
}

Emitter::~Emitter()
{

}
void Emitter::LoadImage()
{
    const char* err = nullptr;
    const char* imgPath = "images/Emitter.png";
    bmp = pd->graphics->loadBitmap(imgPath, &err);
    if (bmp == nullptr)
        pd->system->logToConsole("Emitter load error");
    else
        pd->system->logToConsole("Emitter loaded?");
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
    
    Vec2 dest = {origin.x + rightVector.x * laserLength, origin.y + rightVector.y * laserLength};
    pd->graphics->drawLine(origin.x, origin.y, dest.x, dest.y, 2.5, kColorBlack);
}