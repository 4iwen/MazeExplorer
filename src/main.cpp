#include "platform/window.h"
#include "platform/time.h"
#include "game/game.h"

int main() {
    Window window(1080, 720, "Maze Explorer");
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

    return 0;
}
