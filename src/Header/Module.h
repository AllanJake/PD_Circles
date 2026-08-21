#ifndef MODULE_H
#define MODULE_H

#pragma once

#include "GameObject.h"

extern "C" {
    #include "pd_api.h"
}

enum ModuleType {
    module_Emitter,
    module_Blocker,
    module_Reciever
};

class Module : public GameObject
{
public:
    Module(PlaydateAPI* pd);
    ~Module();
    void Init(float startAngle);
    virtual bool TryGetImageSize(Vec2& outVec);
protected:
    PlaydateAPI* pd;
    ModuleType _moduleType;
    LCDSprite* sprite;
    // LCDBitmap* bmp;
    
private:
    float _startAngle = 0.0f;
    void SetStartPosition();
};

#endif