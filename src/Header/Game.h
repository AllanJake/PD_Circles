#ifndef GAME_H
#define GAME_H

#pragma once

extern "C" {
    #include "pd_api.h"
}
#include "Circle.h"
#include "Emitter.h"
#include <string>
#include <vector>

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

    PlaydateAPI* pd;
    GameState state = GameState::LevelSelect;

    std::vector<std::string> levelFiles;
    int selectedLevelIndex = 0;
    
    std::vector<GameObject*> gameObjects;
};

#endif