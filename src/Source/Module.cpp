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

bool Module::TryGetImageSize(Vec2 &outVec)
{
    return false;
}

void Module::SetStartPosition()
{
    pd->system->logToConsole("Hello");
    if (parent == nullptr) {
        pd->system->logToConsole("A module doesn't have a parent set, Required to find start position.");
        return;
    }

    if (auto* parentCircle = dynamic_cast<Circle*>(parent)) {
        Circle* t = static_cast<Circle*>(parent);
        int radius = t->GetRadius();
    
        float x = sinf(DEG2RAD(_startAngle)) * radius;
        float y = cosf(DEG2RAD(_startAngle)) * radius;
        pd->system->logToConsole("{%f, %f}", x, y);
        localPosition = {x, y};
    }
    else {
        pd->system->logToConsole("Parent is not a circle?");
        return;
    }
}