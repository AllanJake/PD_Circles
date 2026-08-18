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

    // Create modules after the circles, so every module can parent to it's owning circle
    for (const CircleLevelData& circleData : levelData) {
        Circle* parentCircle = circleMap[circleData.id];
        if (!parentCircle) continue;

        for (const ModuleLevelData& moduleData : circleData.modules) {
            GameObject* moduleObject = nullptr;
            switch (moduleData.kind) {
                case ModuleKind::Emitter:
                {
                    Emitter* e = new Emitter(pd);
                    e->localPosition = {moduleData.x, moduleData.y};
                    e->localRotation = moduleData.rotation;
                    e->tags = moduleData.tags;
                    e->LoadImage();

                    moduleObject = e;
                    break;
                }
                case ModuleKind::Blocker:
                {
                    pd->system->logToConsole("Blocker module parsed, but Blocker class not implemented");
                    break;
                }
                case ModuleKind::Receiver:
                {
                    pd->system->logToConsole("Receiver module parsed, but Receiver class not implemented");
                    break;
                }
                case ModuleKind::Unknown:
                {
                        pd->system->logToConsole("Unknown module type in level JSON");
                    break;
                }
            }

            if (moduleObject) 
            {
                moduleObject->SetParent(parentCircle);
                gameObjects.push_back(moduleObject);
            }
        }
    }
}

void Game::Update()
{
    pd->graphics->clear(kColorWhite);
    
    float angle = pd->system->getCrankAngle();

    for (auto& go : gameObjects)
    {
        go->Draw();
    }

    // gameObjects[gameObjects.size() - 2]->localRotation = angle;
    // //emitter->localRotation = angle;
    // emitter->Update();
    // emitter->Draw();
}