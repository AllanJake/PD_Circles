#ifndef GAME_H
#define GAME_H

#pragma once

extern "C" {
    #include "pd_api.h"
}
#include "Circle.h"

class Game
{
public:
    Game(PlaydateAPI* pd);
    ~Game();
    void Init();
    void Update();

private:
    PlaydateAPI* pd;
    std::vector<GameObject*> gameObjects;
};

#endif