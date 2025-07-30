#pragma once
#include <cstddef>

struct CircleLevelData {
    const char* id;
    const char* parentId;
    float x;
    float y;
    float radius;
};

extern const CircleLevelData level1[];
extern const size_t level1Size;
