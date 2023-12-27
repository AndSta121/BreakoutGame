#pragma once
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define TEXTURE_START 28
#define BRICK_HEIGHT 20

#include<SDL.h>
#include<SDL_image.h>
#include"SDL_FontCache.h"
#include<SDL_mixer.h>
#include<SDL_ttf.h>
#include<iostream>
#include<sstream>
#include"tinyxml2.h"
#include"BrickType.h"
#include"Brick.h"


using namespace tinyxml2;
using namespace std;

class Level
{
public:
	int numRows{};
	int numColmns{};
	int rowSpacing{};
	int columnSpacing{};
	Brick** bricks;
	float brickWidth{};

	Level(SDL_Renderer* renderer, int currentLevel);
	~Level();
	bool GetLevelFromXml(int currentLevel);
	int ReturnHitPointsFromBrickType(char id);
	unsigned int CountBricks();
	void RenderLevel(float delta);
	SDL_Texture* ReturnBrickTextureFromBrickType(char id); 
	SDL_Texture* ReturnBrickTextureFromBrickType(int hitPointsRemained);
	int ReturnBreakScoreFromBrick(int x, int y);
	Mix_Chunk* ReturnHitSoundFromBrickType(char id);
	Mix_Chunk* ReturnBreakSoundFromBrickType(char id);

private:
	BrickType* brickTypes;
	SDL_Renderer* renderer;
	SDL_Texture* levelTexture;
};

