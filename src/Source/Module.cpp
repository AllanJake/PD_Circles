#include "../Header/Module.h"
#include "../Header/Circle.h"

Module::Module(PlaydateAPI* pd)
    : pd(pd)
{

}

Module::~Module()
{
    
}

void Module::Init(float startAngle) {
    _startAngle = startAngle;

    SetStartPosition();
}

void Module::LoadModuleImage(const char* path)
{
    const char* err = nullptr;
    const char* imgPath = path;
    bmp = pd->graphics->loadBitmap(imgPath, &err);
    if (bmp == nullptr)
        pd->system->logToConsole("Emitter load error");
}

bool Module::TryGetImageSize(Vec2 &outVec)
{
    if (bmp == NULL) {
        return false;
    }

    int width;
    int height;
    pd->graphics->getBitmapData(bmp, &width, &height, nullptr, nullptr, nullptr);
    outVec = Vec2(width, height);
    return true;
}

void Module::SetStartPosition()
{
    if (parent == nullptr) {
        pd->system->logToConsole("A module doesn't have a parent set, Required to find start position.");
        return;
    }

    if (auto* parentCircle = dynamic_cast<Circle*>(parent)) {
        Circle* t = static_cast<Circle*>(parent);
        int radius = t->GetRadius();
    
        float x = (sinf(DEG2RAD(_startAngle)) * radius);
        float y = (cosf(DEG2RAD(_startAngle)) * radius);
        localPosition.x += x;
        localPosition.y += y;
    }
    else {
        pd->system->logToConsole("Parent is not a circle?");
        return;
    }
}