
#include "../Header/Game.h"

static Game* game = nullptr;

extern "C" {
    
#ifdef _WIN32
__declspec(dllexport)
#endif
    
    int eventHandler(PlaydateAPI* pd, PDSystemEvent event, uint32_t arg) {
        if (event == kEventInit) {
            game = new Game(pd);
            game->Init();

            pd->system->setUpdateCallback([](void*) -> int {
                game->Update();
                return 1;
            }, nullptr);
        }
        return 0;
    }
}