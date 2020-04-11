#include "Drawing.hpp"

#include <stdexcept>

#include <SDL2/SDL.h>

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

static void setColor(SDL_Color const& color) noexcept {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void InitDrawing() {
    if (!window) {
        int error = SDL_Init(SDL_INIT_VIDEO);
        if (error)
            throw std::runtime_error("Could not initialize SDL2");
        window = SDL_CreateWindow("PONG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 2*Game::SIZE.x, 2*Game::SIZE.y, SDL_WINDOW_RESIZABLE);
        if (!window) {
            SDL_Quit();
            throw std::runtime_error("Could not create SDL_Window");
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!renderer) {
            SDL_DestroyWindow(window);
            SDL_Quit();
            window = nullptr;
            throw std::runtime_error("Could not create SDL_Renderer");
        }
    }
    SDL_RenderSetLogicalSize(renderer, Game::SIZE.x, Game::SIZE.y);
    setColor(COLOR_BLACK);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    RenderFrame();
}

void QuitDrawing() noexcept {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void FillScreen(SDL_Color color) noexcept {
    setColor(color);
    SDL_Rect r { 0, 0, Game::SIZE.x, Game::SIZE.y };
    SDL_RenderFillRect(renderer, &r);
}
void DrawBall(Ball const& b, SDL_Color color) noexcept {
    SDL_Rect br { b.pos.x - Ball::RADIUS, b.pos.y - Ball::RADIUS, 2*Ball::RADIUS, 2*Ball::RADIUS };
    setColor(color);
    SDL_RenderFillRect(renderer, &br);
}
void DrawPaddle(Paddle const& p, int x, SDL_Color color) noexcept {
    SDL_Rect pr { x - Paddle::HALF_SIZE.x, p.posY - Paddle::HALF_SIZE.y, 2*Paddle::HALF_SIZE.x, 2*Paddle::HALF_SIZE.y };
    setColor(color);
    SDL_RenderFillRect(renderer, &pr);
    pr.y = 0; pr.h = Game::SIZE.y;
    color.a *= 0.3;
    setColor(color);
    SDL_RenderFillRect(renderer, &pr);
}
void DrawGame(Game const& g) noexcept {
    DrawBall(g.ball, COLOR_BALL);
    DrawPaddle(g.left, 0, COLOR_P1);
    DrawPaddle(g.right, Game::SIZE.x, COLOR_P2);

    float opacity = g.flashAnim.timeLeft / FlashAnim::DURATION;
    if (opacity != 0) {
        opacity *= opacity; // to get a quadratic evolution
        SDL_Color color = g.flashAnim.color;
        color.a *= opacity;
        FillScreen(color);
    }

}
void RenderFrame() noexcept {
    SDL_RenderPresent(renderer);
}
