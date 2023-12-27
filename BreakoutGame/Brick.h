#pragma once

class Brick {
public:
	char id;
	int hitPointsRemained;
	bool collisionInPreviousFrame;

	Brick operator= (const Brick& brick);
	float GetBallReflectionFromBrick(float hitPointOnBrick, float brickWidthOrHeight);
};