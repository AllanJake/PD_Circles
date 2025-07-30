#include "../Header/Game.h"

#include "../Header/LevelData.h"
#include <unordered_map>
#include <string>

Game::Game(PlaydateAPI* pd) 
    : pd(pd)
{

}

Game::~Game()
{

}

void Game::Init()
{
    std::unordered_map<std::string, Circle*> circleMap;

    // Create all objects
    for(size_t i = 0; i < level1Size; i++)
    {
        const auto& d = level1[i];
        Circle* circle = new Circle(pd, d.radius);
        circle->localPosition = {d.x, d.y};

        circleMap[d.id] = circle;
        gameObjects.push_back(circle);
    }

    // Setup hierarchy
    for (size_t i = 0; i < level1Size; i++) {
        const auto& d = level1[i];
        if (d.parentId)
        {
            Circle* child = circleMap[d.id];
            Circle* parent = circleMap[d.parentId];
            if (child && parent){
                child->SetParent(parent);
            }
        }
    }
}

void Game::Update()
{
    pd->graphics->clear(kColorWhite);
    
    float angle = pd->system->getCrankAngle();
    // mainCircle->localRotation = angle;

    // mainCircle->Update();
    for (auto& go : gameObjects)
    {
        go->Draw();
    }
}