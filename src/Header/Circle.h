#ifndef CIRCLE_H
#define CIRCLE_H

#pragma once

#include "GameObject.h"

extern "C" {
    #include "pd_api.h"
}

class Circle : public GameObject
{
public:
    enum STATE {
        UNSELECTED,
        SELECTED,
        LOCKED
    };
    Circle(PlaydateAPI* pd, int radius);
    ~Circle();
    void Update() override;
    void Draw() override;
    int GetRadius() { return radius; };
    void SetCircleState(STATE newState) { circleState = newState; };
    STATE GetCircleState() { return circleState; };

private:
    PlaydateAPI* pd;
    int radius;
    int defaultLineWidth = 4;
    
    STATE circleState = STATE::UNSELECTED;
};

#endif