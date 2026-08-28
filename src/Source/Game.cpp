#include "../Header/Game.h"

#include "../Header/LevelLoader.h"
#include "../Header/LevelData.h"
#include "../Header/Raycast.h"
#include <string>
#include <vector>
#include <algorithm>
#include <format>

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

    std::string CreateRingString(int idx) {
        std::string s = "ring";
        s.append(std::to_string(idx));
        return s;
    }
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


#pragma region Level Select
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

    // Create all objects
    for (const CircleLevelData& d : levelData) {
        Circle* circle = new Circle(pd, static_cast<int>(d.radius));
        circle->localPosition = {d.x, d.y};
        circle->tags = d.tags;
        circle->SetRadius(d.radius);
        int ringState = d.ringState;
        if (d.ringState == 1) {
            if (selectedStringId.empty() == false)
            {
                pd->system->logToConsole("$s: tried to set itself to selected. But %s is already selected",
                                                d.id.c_str(), selectedStringId.c_str());
                ringState = 0;
            }
            else {
                selectedStringId = d.id;
            }
        }
        circle->SetCircleState((Circle::STATE)ringState);

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
            Module* module = nullptr;
            switch (moduleData.kind) {
                case ModuleKind::Emitter:
                {
                    Emitter* e = new Emitter(pd);
                    e->localRotation = moduleData.rotation;
                    e->localPosition = {moduleData.x, moduleData.y};
                    e->tags = moduleData.tags;
                    e->LoadImage();
                    
                    moduleObject = e;
                    module = e;
                    emitters.push_back(e);
                    e = nullptr;
                    delete e;
                    break;
                }
                case ModuleKind::Blocker:
                {
                    pd->system->logToConsole("Blocker module parsed, but Blocker class not implemented");
                    break;
                }
                case ModuleKind::Receiver:
                {
                    Receiver* r = new Receiver(pd);
                    r->localRotation = moduleData.rotation;
                    r->localPosition = {moduleData.x, moduleData.y};
                    r->tags = moduleData.tags;
                    r->LoadImage();
                    r->SetRadius(8.0f);

                    moduleObject = r;
                    module = r;
                    receivers.push_back(r);
                    r = nullptr;
                    delete r;
                    break;
                }
                case ModuleKind::Unknown:
                {
                        pd->system->logToConsole("Unknown module type in level JSON");
                    break;
                }
            }

            moduleObject->SetParent(parentCircle);
            if (module != nullptr) 
            {
                module->Init(moduleData.startAngle);
            }
            gameObjects.push_back(moduleObject);
            
        }
    }
    state = GameState::Playing;

    // If no ring is marked as selected. select the first unlocked ring.
    if (selectedStringId.empty()) {
        for (auto c : circleMap) {
            if (c.second->GetCircleState() == Circle::STATE::LOCKED) continue;

            selectedStringId = c.first;
            break;
        }
    }
    circleMap[selectedStringId]->SetCircleState(Circle::STATE::SELECTED);

    // Pass a reference of all game objects to 
    // for (Emitter* em : emitters)
    // {
    //     em->gos = &gameObjects;
    // }
    Raycast::gameObjects = &gameObjects;
}

void Game::ClearLevel() 
{
    for (GameObject* go : gameObjects) {
        delete go;
    }

    gameObjects.clear();
}

#pragma endregion // Level Select

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

void Game::Update()
{
    if (state == GameState::LevelSelect) {
        UpdateLevelSelect();
        return;
    }


    pd->graphics->clear(kColorWhite);

    bool gameComplete = CheckReceivers();
    if (gameComplete) {
        UpdateLevelComplete();
    }
    else {
        UpdateInput();
    }

    for (auto& go : gameObjects)
    {
        go->Update();
        go->Draw();
    }

}

void Game::UpdateInput() 
{
    PDButtons current;
    PDButtons pushed;
    PDButtons released;
    pd->system->getButtonState(&current, &pushed, &released);

    if (pushed & kButtonUp) {
        UpdateSelectedRing(1);
    }

    if (pushed & kButtonDown) {
        UpdateSelectedRing(-1);        
    }

    float angle = pd->system->getCrankAngle();
    float angleDelta = angle - previousAngle;
    circleMap[selectedStringId]->localRotation += angleDelta;
    previousAngle = angle;

}

void Game::UpdateSelectedRing(int direction) {
    if (direction > 1 || direction < -1 || direction == 0) return;

    int ringIdx = selectedRingIdx;
    std::string newCircleId = CreateRingString(ringIdx);
    int ringCount = static_cast<int>(circleMap.size());

    // Attempt to select the next ring in the direction of choice
    // Skip the ring if it is locked.
    for (int attempt = 0; attempt < 10; attempt++) {
        ringIdx += direction;
        if (ringIdx > ringCount) ringIdx = 1;
        if (ringIdx < 1) ringIdx = ringCount;
        newCircleId = CreateRingString(ringIdx);

        Circle* tempCircle = circleMap[newCircleId];
        Circle::STATE tempState = tempCircle->GetCircleState();
        if (tempCircle != nullptr &&
            tempState != Circle::STATE::LOCKED) {
            break;
        }
    }

    for (const std::pair<std::string, Circle*> c : circleMap)
    {
        if (c.second->GetCircleState() == Circle::STATE::LOCKED) continue;
        bool isSelected = c.first == newCircleId;
        Circle::STATE newState = isSelected ? Circle::STATE::SELECTED : Circle::STATE::UNSELECTED;
        c.second->SetCircleState(newState);
    }
    selectedRingIdx = ringIdx;
    selectedStringId = newCircleId;
}

bool Game::CheckReceivers()
{
    return std::all_of(receivers.begin(), receivers.end(), [](Receiver* r) { return r->GetCompleteStatus(); });
}

void Game::UpdateLevelComplete() {
    if (timeComplete == 0) {
        timeComplete = pd->system->getCurrentTimeMilliseconds();

    } 
    else {
        uint32_t currentTime = pd->system->getCurrentTimeMilliseconds();
        if (currentTime - timeComplete >= winGracePeriod) {
            ShowLevelSelect();
        }
    }
}