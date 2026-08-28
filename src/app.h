#pragma once

#include "platform/window.h"
#include "game/game.h"

class App {
public:
    static void run();

private:
    static void set_callbacks(Window &window, Game &game);
};
