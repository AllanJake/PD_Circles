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

    std::vector<GameObject*> gos;

    bool TryGetImageSize(Vec2& outVec) override;
protected:
private:
    void DrawLaser();
    bool IntersectCircle(const Vec2& origin, const Vec2& dir, const Vec2& center, float radius, float& outT);
    bool IntersectAABB(const Vec2& origin, const Vec2& dir, const Vec2& min, const Vec2& max, float& outT);
    LCDBitmap* bmp;
    Vec2 worldPosition;

};

#endif