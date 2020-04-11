#include <pong/Logic.hpp>
#include <pong/Drawing.hpp>

#include <random>
#include <chrono>

#include <SDL2/SDL.h>

static auto keyboard = SDL_GetKeyboardState(nullptr);

static constexpr SDL_Scancode
    KEY_P1_DOWN = SDL_SCANCODE_S,
    KEY_P1_UP   = SDL_SCANCODE_W,
    KEY_P2_DOWN = SDL_SCANCODE_DOWN,
    KEY_P2_UP   = SDL_SCANCODE_UP,
    KEY_QUIT    = SDL_SCANCODE_ESCAPE,
    KEY_RESET   = SDL_SCANCODE_RETURN;

float getRandomAngle() noexcept {
    static std::mt19937 gen(std::chrono::steady_clock::now().time_since_epoch().count());
    static std::uniform_int_distribution<int> distQuadrant(0, 3);
    static std::normal_distribution<float> distAngle(45, 30);

    return (distAngle(gen) + 90 * distQuadrant(gen)) * (M_PI / 180.f);
}

bool Game::shouldQuit() noexcept {
    return keyboard[SDL_SCANCODE_ESCAPE];
}

void Game::init() noexcept {
    ball.pos = Ball::SPAWN;
    ball.setAngle(getRandomAngle());
    ball.speed = Ball::INIT_SPEED;
}

static float posY_intersectX(float x, SDL_FPoint from, SDL_FPoint to) {
    float a = (from.y - to.y) / (from.x - to.x);
    float b = from.y - a * from.x;
    return a * x + b;
}

void Game::update(float elapsed) noexcept {
    SDL_PumpEvents();

    Paddle::Dir dirLeft = Paddle::Dir::NONE, dirRight = Paddle::Dir::NONE;

    if (keyboard[KEY_P1_DOWN] && keyboard[KEY_P1_UP])
        /* do nothing if both pressed */;
    else if (keyboard[KEY_P1_DOWN])
        dirLeft = Paddle::Dir::DOWN;
    else if (keyboard[KEY_P1_UP])
        dirLeft = Paddle::Dir::UP;

    if (keyboard[KEY_P2_DOWN] && keyboard[KEY_P2_UP])
        /* do nothing if both pressed */;
    else if (keyboard[KEY_P2_DOWN])
        dirRight = Paddle::Dir::DOWN;
    else if (keyboard[KEY_P2_UP])
        dirRight = Paddle::Dir::UP;

    left.update(elapsed, dirLeft);
    right.update(elapsed, dirRight);

    SDL_FPoint next = {
        ball.pos.x + ball.velX * ball.speed * elapsed,
        ball.pos.y + ball.velY * ball.speed * elapsed };

    // first check anim
    if (flashAnim.timeLeft > 0) { // if not finished yet
        flashAnim.timeLeft -= elapsed;
        if (flashAnim.timeLeft <= 0) { // if just finised
            flashAnim.timeLeft = 0;
            init();
        } else { // if not finished
            ball.pos = next; // let the ball continue outside the game world
        }
    }
    
    SDL_Point topLeft     { Paddle::HALF_SIZE.x, Ball::RADIUS },
              bottomRight { Game::SIZE.x - topLeft.x, Game::SIZE.y - topLeft.y };
    SDL_Rect rect { topLeft.x, topLeft.y, bottomRight.x - topLeft.x, bottomRight.y - topLeft.y };

    if (next.x < topLeft.x) { // maybe collision with left
        float y = posY_intersectX(topLeft.x, ball.pos, next);
        if (left.posY - Paddle::HALF_SIZE.y <= y + Ball::RADIUS && y - Ball::RADIUS <= left.posY + Paddle::HALF_SIZE.y) {
            // there is a collision with the left paddle
            next.x = 2*topLeft.x - next.x;
            ball.velX *= -1;
            ball.speed *= 1.1f;
        }
        else {
            // no collision with the paddle
            ball.speed = 0;
            ball.pos = Ball::SPAWN;
            flashAnim.color = COLOR_P1;
            flashAnim.timeLeft = FlashAnim::DURATION;
            return;
        }
    }
    else if (next.x > bottomRight.x) { // right paddle
        float y = posY_intersectX(bottomRight.x, ball.pos, next);
        if (right.posY - Paddle::HALF_SIZE.y <= y + Ball::RADIUS && y - Ball::RADIUS <= right.posY + Paddle::HALF_SIZE.y) {
            // there is a collision with the right paddle
            next.x = 2*bottomRight.x - next.x;
            ball.velX *= -1;
            ball.speed *= 1.1f;
        }
        else {
            // no collision with the paddle
            ball.speed = 0;
            ball.pos = Ball::SPAWN;
            flashAnim.color = COLOR_P2;
            flashAnim.timeLeft = FlashAnim::DURATION;
            return;
        }
    }

    if (next.y < topLeft.y) {
        next.y = 2*topLeft.y - next.y;
        ball.velY *= -1;
    }
    if (next.y > bottomRight.y) {
        next.y = 2*bottomRight.y - next.y;
        ball.velY *= -1;
    }

    ball.pos = next;
}