#pragma once

class Game {
public:
    Game();
    ~Game();
    void fixed_update(double delta);
    void update(double delta);
};