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
    //LCDBitmap* bmp;
protected:
    PlaydateAPI* pd;
    ModuleType _moduleType;
    LCDSprite* sprite;
    
    
private:
    float _startAngle = 0.0f;
    void SetStartPosition();
};

#endif