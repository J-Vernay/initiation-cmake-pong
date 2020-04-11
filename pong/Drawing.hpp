#ifndef PONG_DRAWING
#define PONG_DRAWING

#include <pong/Logic.hpp>

void InitDrawing();
void QuitDrawing() noexcept;

void FillScreen(SDL_Color color) noexcept;
void DrawBall(Ball const& b, SDL_Color color) noexcept;
void DrawPaddle(Paddle const& p, int x, SDL_Color color) noexcept;
void DrawGame(Game const& g) noexcept;
void RenderFrame() noexcept;


constexpr SDL_Color
    COLOR_BLACK = { 0, 0, 0, 255 },
    COLOR_BG = { 0xeb, 0xeb, 0xeb, 255 },
    COLOR_P1 = { 0xff, 0x67, 0x00, 255 },
    COLOR_P2 = { 0x26, 0x33, 0xC6, 255 },
    COLOR_BALL = { 0x50, 0x50, 0x50, 255 };

#endif