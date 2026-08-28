#include "app.h"

#include "platform/window.h"
#include "platform/time.h"
#include "renderer/renderer.h"
#include "game/game.h"

void App::run() {
    constexpr uint32_t WINDOW_WIDTH = 1152;
    constexpr uint32_t WINDOW_HEIGHT = 720;

    Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "Maze Explorer");
    Renderer::initialize();
    Game game(WINDOW_WIDTH, WINDOW_HEIGHT);

    set_callbacks(window, game);

    double FIXED_DELTA = 1.0 / 60.0;
    double fixed_accumulator = 0.0;
    double last_time = Time::elapsed();

    while (!window.should_close()) {
        double current_time = Time::elapsed();
        double frame_time = current_time - last_time;
        last_time = current_time;

        frame_time = std::min(frame_time, 0.25);

        fixed_accumulator += frame_time;
        while (fixed_accumulator >= FIXED_DELTA) {
            game.fixed_update(FIXED_DELTA);
            fixed_accumulator -= FIXED_DELTA;
        }

        game.update(frame_time);
        window.present();
    }
}

void App::set_callbacks(Window &window, Game &game) {
    window.set_resize_callback([&game](uint32_t width, uint32_t height) {
        game.on_resize(width, height);
        Renderer::set_viewport(width, height);
    });
}
