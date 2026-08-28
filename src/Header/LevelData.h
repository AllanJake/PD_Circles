#pragma once
#include <cstddef>
#include <vector>
#include <string>

enum class ModuleKind
{
    Emitter,
    Blocker,
    Receiver,
    Unknown
};

struct ModuleLevelData {
    std::string id;
    ModuleKind kind = ModuleKind::Unknown;
    float x = 0.0f;
    float y = 0.0f;
    float startAngle = 0.0f;
    float rotation = 0.0f;
    float radius = 0.0f;
    std::vector<std::string> tags;
};

struct CircleLevelData {
    std::string id;
    std::string parentId;
    int x = 0;
    int y = 0;
    int radius = 0;
    int ringState = 0;
    std::vector<std::string> tags;
    std::vector<ModuleLevelData> modules;
};