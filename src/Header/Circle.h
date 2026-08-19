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
    Circle(PlaydateAPI* pd, int radius);
    ~Circle();
    void Update() override;
    void Draw() override;
    int GetRadius() { return radius; };

private:
    PlaydateAPI* pd;
    int radius;
};

#endif