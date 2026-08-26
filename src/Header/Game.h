#ifndef GAME_H
#define GAME_H

#pragma once

extern "C" {
    #include "pd_api.h"
}
#include "Circle.h"
#include "Emitter.h"
#include "Receiver.h"
#include <string>
#include <vector>
#include <unordered_map>

class Game
{
public:
    Game(PlaydateAPI* pd);
    ~Game();
    void Init();
    void Update();

    void ShowLevelSelect();
    void LoadSelectedLevel();

private:
    enum class GameState
    {
        LevelSelect,
        Playing
    };

    void ScanLevels();
    void DrawLevelSelect();
    void UpdateLevelSelect();
    void LoadLevelByPath(const std::string& path);
    void ClearLevel();
    void UpdateInput();
    void UpdateSelectedRing(int index);

    PlaydateAPI* pd;
    GameState state = GameState::LevelSelect;

    std::vector<std::string> levelFiles;
    int selectedLevelIndex = 0;
    
    std::vector<GameObject*> gameObjects;
    std::unordered_map<std::string, Circle*> circleMap;
    std::vector<Emitter*> emitters;
    std::vector<Receiver*> receivers;
    int selectedRingIdx = 1;
    std::string selectedStringId = "ring1";
    float previousAngle = 0.0f;
};

#endif