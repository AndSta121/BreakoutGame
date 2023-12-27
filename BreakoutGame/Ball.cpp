#include"Ball.h"

Ball::Ball(Vector2 *position)
{
    this->position = position;
}

Ball::Ball(SDL_Renderer* renderer, float x, float y)
{
    this->renderer = renderer;
    position = new Vector2();
    velocity = new Vector2();

    SDL_Surface* surface = IMG_Load("Textures/Ball/Ball.png");
    if (!surface) {
        cout << "Failed to load image at Textures/Ball/Ball.png :" << SDL_GetError();
        return;
    }
    ballTexture = SDL_CreateTextureFromSurface(renderer, surface);
    //this->width = BALL_WIDTH;
    //this->height = BALL_HEIGHT;
    SDL_FreeSurface(surface);

    this->BallStartingPosition(x,y);
}

Ball::~Ball()
{
    SDL_DestroyTexture(ballTexture);
}

void Ball::BallStartingPosition(float x, float y)
{
    this->position->SetVector((x - BALL_WIDTH) / 2, y - BALL_HEIGHT);
    this->velocity->SetVector(1, -1);
    SetDirection();
}

void Ball::SetDirection()
{
    float length = sqrt(this->velocity->x * this->velocity->x + this->velocity->y * this->velocity->y);
    //jedinicni vector * ball_speed
    *velocity = *velocity / length * BALL_SPEED;
}

void Ball::UpdateBall(float delta) {
    *position += (*velocity * delta);
}

void Ball::RenderBall(float delta)
{
    SDL_Rect rect;
    rect.x = (int)position->x;
    rect.y = (int)position->y;
    rect.w = BALL_WIDTH;
    rect.h = BALL_HEIGHT;
    SDL_RenderCopy(renderer, this->ballTexture, 0, &rect);
}

void Ball::CheckBallVelocity()
{
    float length = sqrt(this->velocity->x * this->velocity->x + this->velocity->y * this->velocity->y);
    Vector2 tempVelocity = *velocity / length;
    if (tempVelocity.y == 0)
        tempVelocity.y = 1;
    if (tempVelocity.x == 0)
        tempVelocity.x = 1;
    *velocity = tempVelocity * BALL_SPEED;
}
