#include "../Header/LevelData.h"

const CircleLevelData level1[] = {
    { "ring1", nullptr, 200, 120, 20, std::vector<std::string>{} },
    { "ring2", nullptr, 200, 120, 40, std::vector<std::string>{} },
    { "ring3", nullptr, 200, 120, 60, std::vector<std::string>{} },
    { "module1", "ring2", 0, -40, 5, std::vector<std::string>{"module"} },
};
const size_t level1Size = 4;
