#ifndef RECEIVER_H
#define RECEIVER_H

#pragma once

#include "Module.h"

class Receiver : public Module {
    public:
    Receiver(PlaydateAPI* pd);
    ~Receiver();

    void Update() override;
    void Draw() override;

    std::vector<GameObject*> gos;

    bool TryGetImageSize(Vec2& outVec) override;

    private:
    LCDBitmap* bmp;
    Vec2 worldPosition;
};

#endif