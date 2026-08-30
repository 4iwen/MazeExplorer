#include "app.h"

#include "platform/window.h"
#include "platform/time.h"
#include "renderer/renderer.h"
#include "game/game.h"
#include "utils/utils.h"

void App::run() {
    constexpr uint32_t WINDOW_WIDTH = 1152;
    constexpr uint32_t WINDOW_HEIGHT = 720;

    Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "Maze Explorer");
    window.set_vsync(true);
    window.set_cursor_locked(true);
    Renderer::initialize();
    Game game(WINDOW_WIDTH, WINDOW_HEIGHT);

    set_callbacks(window, game);

    double FIXED_DELTA = 1.0 / 60.0;
    double fixed_accumulator = 0.0;
    double last_time = Time::elapsed();

    while (!window.should_close()) {
        Input::poll();
        window.poll_events();

        // Hardcoded cursor capturing.
        if (Input::is_key_pressed(Key::ESCAPE)) {
            window.set_cursor_locked(false);
        }
        if (Input::is_mouse_button_pressed(Mouse_Button::LEFT)) {
            window.set_cursor_locked(true);
        }

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
    window.set_resize_callback([&game](int32_t width, int32_t height) {
        if (width <= 0 || height <= 0) {
            return;
        }

        game.on_resize(width, height);
        Renderer::set_viewport(width, height);
    });
    window.set_key_callback([](int32_t key, int32_t scancode, int32_t action, int32_t mods) {
        UNUSED(scancode);
        UNUSED(mods);
        Input::on_key(key, action);
    });
    window.set_cursor_callback([](double x, double y) {
        Input::on_cursor(x, y);
    });
    window.set_mouse_button_callback([](int32_t button, int32_t action, int32_t mods) {
        UNUSED(mods);
        Input::on_mouse_button(button, action);
    });
}
