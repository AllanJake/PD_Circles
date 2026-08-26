#ifndef RECEIVER_H
#define RECEIVER_H

#pragma once

#include "Module.h"

class Receiver : public Module {
    public:
    Receiver(PlaydateAPI* pd);
    ~Receiver();
    void Init(float startAngle) override;
    void LoadImage();
    void Update() override;
    void Draw() override;

    private:
    Vec2 worldPosition;
    const char* imagePath = "images/Receiver.png";
};

#endif