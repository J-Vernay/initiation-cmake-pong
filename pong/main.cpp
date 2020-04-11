#include <pong/Drawing.hpp>
#include <pong/Logic.hpp>

#include <SDL2/SDL.h>

#include <chrono>
using Seconds = std::chrono::duration<float>;
auto now() noexcept { return std::chrono::steady_clock::now(); }

int main(int argc, char** argv) {
    InitDrawing();
    Game game;
    game.flashAnim.color = COLOR_BLACK;
    game.flashAnim.timeLeft = FlashAnim::DURATION;
    game.ball.speed = 0;
    
    auto lastTime = now();
    while(!Game::shouldQuit()) {
        auto elapsed = now() - lastTime;
        lastTime = now();
        
        game.update(Seconds(elapsed).count());
        
        FillScreen(COLOR_BG);
        DrawGame(game);
        RenderFrame();
    }
    
    QuitDrawing();
    return 0;
}

