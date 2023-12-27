#pragma once
#include"Level.h"
#include"Paddle.h"
#include"Ball.h"

enum Buttons
{
    PaddleLeft = 0,
    PaddleRight
};

class Game {
private:
    SDL_Window* window{};
    SDL_Renderer* renderer{};

    Level* level{};
    Paddle* paddle{};
    Ball* ball{};

    unsigned int lastTick{}, newTick{};

    float delta{};

    unsigned int currentLevel{};
    unsigned int totalScore{};
    unsigned int remainingLives{};
    unsigned int remainingBricks{};

    FC_Font* textFont{};
    FC_Font* textViewFont{};
    SDL_Texture* heartTexture{};

public:
    ~Game();
    bool Init();
    void Run();
    void RenderScoreLevelRemainingLives(float delta);
    void CheckWallCollision();
    void CheckPaddleCollision();
    void CheckBrickCollision();
    void RenderGameOverView();
    void RenderLevelNumberViewBeforePlay();
    void RenderGameFinishedView();
};