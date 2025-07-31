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

    emitter = new Emitter(pd);
    emitter->localPosition = {-60, 0};
    emitter->localRotation = 90;
    emitter->LoadImage();
    emitter->SetParent(gameObjects[gameObjects.size() - 2]);
}

void Game::Update()
{
    pd->graphics->clear(kColorWhite);
    
    float angle = pd->system->getCrankAngle();

    for (auto& go : gameObjects)
    {
        go->Draw();
    }

    gameObjects[gameObjects.size() - 2]->localRotation = angle;
    //emitter->localRotation = angle;
    emitter->Update();
    emitter->Draw();
}