#pragma once
#include"SDL.h"
#include"iostream"
#include"Vector2.h"
#include"SDL_image.h"

#define BALL_SPEED 500.0f
#define BALL_WIDTH 15
#define BALL_HEIGHT 15

using namespace std;

class Ball {
private:
	SDL_Renderer* renderer{};
	SDL_Texture* ballTexture{};
public:
	Vector2 *position{};
	Vector2 *velocity{};

	Ball(Vector2* position);
	Ball(SDL_Renderer* renderer, float x, float y);
	~Ball();
	void BallStartingPosition(float x, float y);
	void SetDirection();
	void UpdateBall(float delta);
	void RenderBall(float delta);
	void CheckBallVelocity();
};