#include "platform/window.h"
#include "platform/time.h"
#include "renderer/renderer.h"
#include "game/game.h"

#include <iostream>

int main() {
    try {
        Window window(1152, 720, "Maze Explorer");
        Renderer renderer;
        Game game;

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
    } catch (std::exception& e) {
        std::cout << "Exception thrown: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
