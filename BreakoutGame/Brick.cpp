#include "Brick.h"

Brick Brick::operator= (const Brick& brick) {
	this->id = brick.id;
	this->hitPointsRemained = brick.hitPointsRemained;

	return *this;
}

float Brick::GetBallReflectionFromBrick(float hitPointOnBrick, float brickWidthOrHeight)
{
	if (hitPointOnBrick < 0) {
		hitPointOnBrick = 0;
	}
	else if (hitPointOnBrick > brickWidthOrHeight) {
		hitPointOnBrick = brickWidthOrHeight;
	}

	// Everything left the center of the paddle is reflected left or everything above the center is reflected above
	// while everything right the center is reflected right or everything below the center is reflected below
	hitPointOnBrick -= brickWidthOrHeight / 2.0f;

	// Scale the reflection, making it fall in the range -2.0f to 2.0f
	return 2.0f * (hitPointOnBrick / (brickWidthOrHeight / 2.0f));
}


