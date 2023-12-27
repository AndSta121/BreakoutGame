#include "BrickType.h"

BrickType::~BrickType()
{
	SDL_DestroyTexture(this->texture);
	Mix_FreeChunk(hitSound);
	Mix_Chunk(breakSound);

}

char BrickType::GetID()
{
	return this->id;
}

void BrickType::SetID(char id)
{
	this->id = id;
}

SDL_Texture* BrickType::GetTexture()
{
	return this->texture;
}

void BrickType::SetTexture(SDL_Texture* texture)
{
	this->texture = texture;
}

int BrickType::GetHitPoints()
{
	return this->hitPoints;
}

void BrickType::SetHitPoints(int hitPoints)
{
	this->hitPoints = hitPoints;
}

Mix_Chunk* BrickType::GetHitSound()
{
	return this->hitSound;
}

void BrickType::SetHitSound(Mix_Chunk* hitSound)
{
	this->hitSound = hitSound;
}

Mix_Chunk* BrickType::GetBreakSound()
{
	return this->breakSound;
}

void BrickType::SetBreakSound(Mix_Chunk* breakSound)
{
	this->breakSound = breakSound;
}

int BrickType::GetBreakScore()
{
	return this->breakScore;
}

void BrickType::SetBreakScore(int breakScore)
{ 
	this->breakScore = breakScore;
}

BrickType BrickType::operator= (const BrickType& bricktype) {
	id = bricktype.id;
	texture = bricktype.texture;
	hitPoints = bricktype.hitPoints;
	hitSound = bricktype.hitSound;
	breakSound = bricktype.breakSound;
	breakScore = bricktype.breakScore;

	return *this;
}

