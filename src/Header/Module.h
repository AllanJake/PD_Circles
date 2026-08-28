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
    virtual void Init(float startAngle);
    void LoadModuleImage(const char* path);
    virtual bool TryGetImageSize(Vec2& outVec);
    virtual void SetModuleActive(bool);
protected:
    PlaydateAPI* pd;
    ModuleType _moduleType;
    LCDSprite* sprite;
    LCDBitmap* bmp = nullptr;
    
private:
    float _startAngle = 0.0f;
    void SetStartPosition();
};

#endif