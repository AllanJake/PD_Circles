#ifndef EMITTER_H
#define EMITTER_H

#pragma once

#include "Module.h"
#include "Raycast.h"

class Emitter : public Module
{
public:
    Emitter(PlaydateAPI* pd);
    ~Emitter();
    void Init(float startAngle) override;
    void LoadImage();
    void Update() override;
    void Draw() override;
    void SetModuleActive(bool) override;
    bool GetLastModuleSet() { return lastModule != nullptr; }

    // This is a non-owning pointer since the emitter only reads this list.
    std::vector<GameObject*>* gos;

protected:
private:
    void DrawLaser();
    Vec2 worldPosition;
    const char* imagePath = "images/Emitter.png";
    Module* lastModule = nullptr;
};

#endif