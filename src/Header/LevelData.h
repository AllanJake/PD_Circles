#pragma once
#include <cstddef>
#include <vector>
#include <string>

struct CircleLevelData {
    std::string id;
    std::string parentId;
    float x = 0.0f;
    float y = 0.0f;
    float radius = 0.0f;
    std::vector<std::string> tags;
};