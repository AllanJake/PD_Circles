#ifndef EMITTER_H
#define EMITTER_H

#pragma once

#include "Module.h"

class Emitter : public Module
{
public:
    Emitter(PlaydateAPI* pd);
    ~Emitter();
    void LoadImage();
    void Update() override;
    void Draw() override;

private:
    void DrawLaser();
    LCDBitmap* bmp;
    Vec2 worldPosition;
};

#endif