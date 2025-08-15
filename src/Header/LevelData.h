#pragma once
#include <cstddef>
#include <vector>
#include <string>

struct CircleLevelData {
    const char* id;
    const char* parentId;
    float x;
    float y;
    float radius;
    std::vector<std::string> tags;
};

extern const CircleLevelData level1[];
extern const size_t level1Size;
