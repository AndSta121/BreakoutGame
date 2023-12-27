#pragma once
#include"SDL.h"
#include"SDL_image.h"
#include<iostream>
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define PADDLE_SPEED 500.0f
#define PADDLE_WIDTH 130
#define PADDLE_HEIGHT 30

class Paddle {
private:
	SDL_Renderer* renderer{};
	SDL_Texture* paddleTexture{};
	float x{}, y{};
	float velocity{};

public:
	Paddle(SDL_Renderer* renderer);
	~Paddle();
	float GetX();
	float GetY();
	void SetVelocity(float velocity);
	float GetVelocity();
	void RenderPaddle(float delta);
	void UpdatePaddle(float delta);
	void PaddleStartingPosition();
	float GetBallReflectionFromPaddle(float hitPointOnPaddle);
};