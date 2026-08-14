#include "../Header/Game.h"

#include "../Header/LevelLoader.h"
// #include "../Header/LevelData.h"
#include <unordered_map>
#include <string>
#include <vector>

Game::Game(PlaydateAPI* pd) 
    : pd(pd)
{

}

Game::~Game()
{

}

void Game::Init()
{
    std::vector<CircleLevelData> levelData;
    if (!LevelLoader::LoadLevel(pd, "Levels/level1.json", levelData))
    {
        pd->system->logToConsole("Level load failed; Game::Init aborted");
        return;
    }

    std::unordered_map<std::string, Circle*> circleMap;

    // Create all objects
    for (const CircleLevelData& d : levelData) {
        Circle* circle = new Circle(pd, static_cast<int>(d.radius));
        circle->localPosition = {d.x, d.y};
        circle->tags = d.tags;
        circle->SetRadius(d.radius);

        circleMap[d.id] = circle;
        gameObjects.push_back(circle);
    }

    // Setup hierarchy
    for (size_t i = 0; i < levelData.size(); i++) {
        const auto& d = levelData[i];
        if (!d.parentId.empty())
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
    emitter->gos = gameObjects;
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