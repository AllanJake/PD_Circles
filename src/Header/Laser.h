#ifndef LASER_H
#define LASER_H

#pragma once

#include "Vector2.h"
#include "Module.h"


class Laser 
{
    public:
        Laser(PlaydateAPI* pd) : pd(pd) {}
        void DrawLaser(Vec2 origin, float rotation);
    private:
        PlaydateAPI* pd;
        float _laserLength = 120.0f;
        Vec2 _origin;
        Module* lastModule = nullptr;

        float M_PI = 3.14f;
};

#endif