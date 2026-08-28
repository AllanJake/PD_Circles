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
    void SetModuleActive(bool) override;
    bool GetCompleteStatus() { return complete; }
private:
    Vec2 worldPosition;
    const char* imagePath = "images/Receiver.png";

    bool hasLaser = false;
    const float chargeTime = 3.0f;
    const int chargeDiameter = 24;
    const int chargeThickness = 2;
    float chargeAngle = 0.0f;
    bool complete = false;

    void UpdateReceiver();
};

#endif