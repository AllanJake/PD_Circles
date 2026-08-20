#include "../Header/Game.h"

#include "../Header/LevelLoader.h"
#include "../Header/LevelData.h"
#include <unordered_map>
#include <string>
#include <vector>

namespace{
    bool EndsWith(const std::string& value, const char* suffix);

    void LevelListCallback(const char* filename, void* userdata);
    
    void LevelSelectMenuCallback(void* userdata);
}

Game::Game(PlaydateAPI* pd) 
    : pd(pd)
{

}

Game::~Game()
{
    ClearLevel();
}

void Game::Init()
{
    pd->system->removeAllMenuItems();
    pd->system->addMenuItem("Level Select", LevelSelectMenuCallback, this);
    
    ScanLevels();
    state = GameState::LevelSelect;
}



void Game::ShowLevelSelect()
{
    ClearLevel();
    ScanLevels();
    state = GameState::LevelSelect;
}

void Game::LoadSelectedLevel()
{
    if (levelFiles.empty()) return;
    LoadLevelByPath(levelFiles[selectedLevelIndex]);
}

void Game::DrawLevelSelect() {
    pd->graphics->clear(kColorWhite);
    pd->graphics->drawText("Select Level", 12, kASCIIEncoding, 20, 20);

    if (levelFiles.empty()) {
        pd->graphics->drawText("No levels found", 15, kASCIIEncoding, 20, 50);
        return;
    }

    for (int i = 0; i < static_cast<int>(levelFiles.size()); i++) {
        std::string line = (i == selectedLevelIndex ? "> " : "  ") + levelFiles[i];
        pd->graphics->drawText(line.c_str(), static_cast<int>(line.size()), kASCIIEncoding, 20, 50 + i * 20);
    }

    pd->graphics->drawText("A: Play  Up/Down: Select", 25, kASCIIEncoding, 20, 210);
}

void Game::UpdateLevelSelect() {
    PDButtons current;
    PDButtons pushed;
    PDButtons released;
    pd->system->getButtonState(&current, &pushed, &released);

    if (!levelFiles.empty()) {
        if (pushed & kButtonUp) {
            selectedLevelIndex--;
            if(selectedLevelIndex < 0) {
                selectedLevelIndex = static_cast<int>(levelFiles.size()) - 1;
            }
        }

        if (pushed & kButtonDown) {
            selectedLevelIndex++;
            if(selectedLevelIndex >= static_cast<int>(levelFiles.size())) {
                selectedLevelIndex = 0;
            }
        }

        if (pushed & kButtonA) {
            LoadSelectedLevel();
            return;
        }
    }
    DrawLevelSelect();
}

void Game::LoadLevelByPath(const std::string& path) {
    ClearLevel();


    std::vector<CircleLevelData> levelData;
    if (!LevelLoader::LoadLevel(pd, path.c_str(), levelData))
    {
        pd->system->logToConsole("Level load failed; Game::Init aborted");
        state = GameState::LevelSelect;
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
                Module* module = dynamic_cast<Module*>(moduleObject);
                module->Init(moduleData.startAngle);
                gameObjects.push_back(moduleObject);
            }
        }
    }
    state = GameState::Playing;
}

void Game::ClearLevel() 
{
    for (GameObject* go : gameObjects) {
        delete go;
    }

    gameObjects.clear();
}

void Game::Update()
{
    if (state == GameState::LevelSelect) {
        UpdateLevelSelect();
        return;
    }


    pd->graphics->clear(kColorWhite);
    
    float angle = pd->system->getCrankAngle();

    if (gameObjects.size() >= 3) {
        gameObjects[2]->localRotation = angle;
    }

    for (auto& go : gameObjects)
    {
        go->Update();
        go->Draw();
    }
}

void Game::ScanLevels() {
    levelFiles.clear();

    int result = pd->file->listfiles("Levels", LevelListCallback, &levelFiles, 0);
    if (result != 0) {
        pd->system->logToConsole("Couldn't list levels folder: %s", pd->file->geterr());
    }

    if (selectedLevelIndex >= static_cast<int>(levelFiles.size())) {
        selectedLevelIndex = 0;
    }
}

namespace {
    bool EndsWith(const std::string& value, const char* suffix) {
        std::string s = suffix;
        if (value.size() < s.size()) return false;
        return value.compare(value.size() - s.size(), s.size(), s) == 0;
    }

    void LevelListCallback(const char* filename, void* userdata) {
        auto* levels = static_cast<std::vector<std::string>*>(userdata);
        if (!filename) return;

        std::string name = filename;

        if (!name.empty() && name.back() == '/') return;

        if (EndsWith(name, ".json")) {
            levels->push_back(std::string("Levels/") + name);
        }
    }
    
    void LevelSelectMenuCallback(void* userdata) {
        auto* game = static_cast<Game*>(userdata);
        if (game) {
            game->ShowLevelSelect();
        }
    }
}