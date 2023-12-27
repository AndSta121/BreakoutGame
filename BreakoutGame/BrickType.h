#pragma once
#include"SDL.h"
#include"SDL_mixer.h"

using namespace std;

class BrickType
{
private:
	char id{};
	SDL_Texture* texture{};
	int hitPoints{};
	Mix_Chunk* hitSound{};
	Mix_Chunk* breakSound{};
	int breakScore{};

public:
	~BrickType();
	char GetID();
	void SetID(char id);
	SDL_Texture* GetTexture();
	void SetTexture(SDL_Texture* texture);
	int GetHitPoints();
	void SetHitPoints(int hitPoints);
	Mix_Chunk* GetHitSound();
	void SetHitSound(Mix_Chunk* hitSound);
	Mix_Chunk* GetBreakSound();
	void SetBreakSound(Mix_Chunk* breakSound);
	int GetBreakScore();
	void SetBreakScore(int breakScore);
	BrickType operator= (const BrickType& brickType);
};
