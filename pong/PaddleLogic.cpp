#include <pong/Logic.hpp>

void Paddle::update(float elapsed, Dir dir) noexcept {
    if (dir == Dir::UP) {
        posY -= elapsed * SPEED;
        if (posY < HALF_SIZE.y)
            posY = HALF_SIZE.y;
    }
    else if (dir == Dir::DOWN) {
        posY += elapsed * SPEED;
        if (posY > Game::SIZE.y - HALF_SIZE.y)
            posY = Game::SIZE.y - HALF_SIZE.y;
    }
}