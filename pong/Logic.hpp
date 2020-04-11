#ifndef PONG_LOGIC
#define PONG_LOGIC

#include <cmath>
#include <cstdint>
#include <utility>

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_pixels.h>

float getRandomAngle() noexcept;

float posY_intersectX(float x, SDL_FPoint from, SDL_FPoint to);

struct Ball {
    static SDL_FPoint const SPAWN;
    static float const RADIUS;
    static float const INIT_SPEED;
    
    SDL_FPoint pos = SPAWN;
    float velX = 1, velY = 0;
    float speed = 0;
    
    void setAngle(float radians) noexcept {
        velX = std::cos(radians);
        velY = std::sin(radians);
    }
};

struct Paddle {
    enum class Dir { NONE, UP, DOWN };
    static SDL_FPoint const HALF_SIZE;
    static float const SPEED;
    static float const SPAWN_Y;
    
    float posY = SPAWN_Y;
    
    void update(float elapsed, Dir dir) noexcept;
};

struct FlashAnim {
    static constexpr float DURATION = 1;
    SDL_Color color;
    float timeLeft = 0;
};

struct Game {
    static SDL_FPoint const SIZE;

    Game() noexcept { init(); }

    Paddle left, right;
    Ball ball;
    FlashAnim flashAnim;
    
    void init() noexcept;
    void update(float elapsed) noexcept;
    static bool shouldQuit() noexcept;
};

inline constexpr SDL_FPoint Game::SIZE { 300, 200 };

inline constexpr SDL_FPoint Ball::SPAWN { Game::SIZE.x / 2, Game::SIZE.y / 2 };
inline constexpr SDL_FPoint Paddle::HALF_SIZE { 10, Game::SIZE.y / 10 };
inline constexpr float Paddle::SPAWN_Y { Game::SIZE.y / 2 };
inline constexpr float Ball::RADIUS { Paddle::HALF_SIZE.y / 4 };
inline constexpr float Ball::INIT_SPEED { Game::SIZE.x / 2 };
inline constexpr float Paddle::SPEED { Ball::INIT_SPEED };

#endif