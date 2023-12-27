#include"Paddle.h"

using namespace std;

Paddle::Paddle(SDL_Renderer* renderer)
{
	this->renderer = renderer;
	//this->height = 30;
	//this->width = 130;
	this->paddleTexture = 0;
	this->PaddleStartingPosition();
	const char* image_path = "Textures/Paddle/Paddle.png";
	SDL_Surface* surface = IMG_Load(image_path);
	if (!surface) {
		cout << "Failed to load image at " << image_path << ": " << SDL_GetError();
		return;
	}

	this->paddleTexture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
}

Paddle::~Paddle()
{
	SDL_DestroyTexture(paddleTexture);
}

float Paddle::GetX()
{
	return this->x;
}

float Paddle::GetY()
{
	return this->y;
}

void Paddle::SetVelocity(float velocity)
{
	this->velocity = velocity;
}

float Paddle::GetVelocity()
{
	return this->velocity;
}

void Paddle::RenderPaddle(float delta)
{
	SDL_Rect paddleRect;

	paddleRect.w =(int) PADDLE_WIDTH;
	paddleRect.h =(int) PADDLE_HEIGHT;
	paddleRect.x =(int) this->x;
	paddleRect.y =(int) this->y;

	SDL_RenderCopy(renderer, this->paddleTexture, 0, &paddleRect);
}

void Paddle::UpdatePaddle(float delta)
{
	this->x += this->velocity * delta;

	if (x < 0)
		x = 0;
	else if (x > (WINDOW_WIDTH - PADDLE_WIDTH))
		x = WINDOW_WIDTH - PADDLE_WIDTH;
}

void Paddle::PaddleStartingPosition()
{
	this->x = (WINDOW_WIDTH - PADDLE_WIDTH) / 2;
	this->y = WINDOW_HEIGHT - PADDLE_HEIGHT;
	this->velocity = 0;
}

float Paddle::GetBallReflectionFromPaddle(float hitPointOnPaddle)
{
	if (hitPointOnPaddle < 0) {
		hitPointOnPaddle = 0;
	}
	else if (hitPointOnPaddle > PADDLE_WIDTH) {
		hitPointOnPaddle = PADDLE_WIDTH;
	}

	// Everything left the center of the paddle is reflected left
	// while everything right the center is reflected right
	hitPointOnPaddle -= PADDLE_WIDTH/ 2.0f;

	// Scale the reflection, making it fall in the range -2.0f to 2.0f
	return 2.0f * (hitPointOnPaddle / (PADDLE_WIDTH / 2.0f));
}
