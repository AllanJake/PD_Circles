#pragma once

extern "C" {
    #include <pd_api.h>
}

#include "LevelData.h"
#include <vector>

class LevelLoader {
    public:
    static bool LoadLevel(PlaydateAPI* pd, const char* path, std::vector<CircleLevelData>& outCircles);
};