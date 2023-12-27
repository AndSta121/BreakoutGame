#include"Game.h"


Game::~Game()
{
	SDL_DestroyTexture(heartTexture);
	FC_FreeFont(textFont);
	FC_FreeFont(textViewFont);
}

bool Game::Init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	Mix_Init(0);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

	this->window = SDL_CreateWindow("Breakout", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (!window) {
		cout << "Error creating window!";
		return false;
	}

	this->renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		cout << "Error creating renderer!";
		return false;
	}

	textFont = FC_CreateFont();
	FC_LoadFont(textFont, renderer, "Fonts/OpenSans-Semibold.ttf", 18, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
	if (!textFont) {
		cout << "Error loading font: " << TTF_GetError() << endl;
		return false;
	}

	textViewFont = FC_CreateFont();
	FC_LoadFont(textViewFont, renderer, "Fonts/OpenSans-Semibold.ttf", 60, FC_MakeColor(255, 165, 0, 255), TTF_STYLE_NORMAL);
	if (!textViewFont) {
		cout << "Error loading font: " << TTF_GetError() << endl;
		return false;
	}

	SDL_Surface* surface = IMG_Load("Textures/Heart/Heart.png");
	if (!surface) {
		cout << "Failed to load image at Textures/Heart/Heart.png " << SDL_GetError();
		return false;
	}
	this->heartTexture = SDL_CreateTextureFromSurface(renderer, surface);
	
	SDL_FreeSurface(surface);

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
	
	this->lastTick = SDL_GetTicks();
	this->currentLevel = 1;
	this->remainingLives = 3;
	this->totalScore = 0;

	return true;
}

void Game::Run()
{
	while (currentLevel <= 3) {
		level = new Level(renderer, currentLevel);
		paddle = new Paddle(renderer);
		ball = new Ball(renderer, WINDOW_WIDTH, WINDOW_HEIGHT - PADDLE_HEIGHT);
		remainingBricks = level->CountBricks();
				
		//Render level number and total score - Renderining before starting level
		RenderLevelNumberViewBeforePlay();
		

		SDL_Event event;
		this->lastTick = SDL_GetTicks();
		bool buttons[2] = {};
		
		//Play time
		while (1) {
			//Keyboard input
			SDL_Event event;
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT)
					return;
				else if (event.type == SDL_KEYDOWN) {
					if (event.key.keysym.sym == SDLK_ESCAPE)
						return;
					else if (event.key.keysym.sym == SDLK_LEFT)
						buttons[Buttons::PaddleLeft] = true;
					else if (event.key.keysym.sym == SDLK_RIGHT)
						buttons[Buttons::PaddleRight] = true;
				}
				else if (event.type == SDL_KEYUP)
				{
					if (event.key.keysym.sym == SDLK_LEFT)
						buttons[Buttons::PaddleLeft] = false;
					else if (event.key.keysym.sym == SDLK_RIGHT)
						buttons[Buttons::PaddleRight] = false;
				}
			}
			//Set paddle velocity
			if (buttons[Buttons::PaddleLeft])
				paddle->SetVelocity(-PADDLE_SPEED);
			else if (buttons[Buttons::PaddleRight])
				paddle->SetVelocity(PADDLE_SPEED);
			else
				paddle->SetVelocity(0.0f);


			//Timing
			newTick = SDL_GetTicks();
			delta = (newTick - lastTick) / 1000.0f;
			lastTick = newTick;

			//CheckCollision
			CheckWallCollision();
			CheckPaddleCollision();
			CheckBrickCollision();

			//Update
			paddle->UpdatePaddle(delta);
			ball->UpdateBall(delta);


			//CheckLives - Game over
			if (this->remainingLives == 0)
			{
				RenderGameOverView();
				return;
			}

			//Check reamining bricks
			if (this->remainingBricks == 0)
			{
				currentLevel++;
				paddle->PaddleStartingPosition();
				ball->BallStartingPosition(WINDOW_WIDTH, WINDOW_HEIGHT - PADDLE_HEIGHT);
				break;
			}

			//Render
			SDL_RenderClear(renderer);
			RenderScoreLevelRemainingLives(delta);
			level->RenderLevel(delta);
			paddle->RenderPaddle(delta);
			ball->RenderBall(delta);
			SDL_RenderPresent(renderer);
		}

		delete level;
		delete paddle;
		delete ball;

	}
	
	//Render game finished view and total score - Renderining when game finished
	RenderGameFinishedView();

	// Destroy renderer and window
	SDL_DestroyTexture(heartTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
}

void Game::RenderScoreLevelRemainingLives(float delta)
{
	//Render current level number
	string temp_str = "Level: " + to_string(this->currentLevel);
	FC_Draw(textFont, renderer, 100, 0, temp_str.c_str());

	//Render score
	temp_str = "Total score: " + to_string(this->totalScore);
	FC_Draw(textFont, renderer, 350, 0, temp_str.c_str());

	//Render remainingLives
	temp_str = "Lives: ";
	FC_Draw(textFont, renderer, 600, 0, temp_str.c_str());

	//Render hearts
	for (int i = 0; i < (int)this->remainingLives; i++) {
		SDL_Rect dest = { 650 + i* 30, 2, 25, 25 };
		SDL_RenderCopy(renderer, this->heartTexture, 0, &dest);
	}	
}

void Game::CheckWallCollision()
{
	if (ball->position->x < 0) {
		ball->velocity->x *= -1;
		ball->position->x = 0;
	}	
	else if (ball->position->x > (WINDOW_WIDTH - BALL_WIDTH)) {
		ball->velocity->x *= -1;
		ball->position->x = WINDOW_WIDTH - BALL_WIDTH;
	}
		
	if (ball->position->y < TEXTURE_START) {
		ball->velocity->y *= -1;
		ball->position->y = TEXTURE_START;
	}
		
	else if (ball->position->y > WINDOW_HEIGHT - BALL_HEIGHT) 
	{
		paddle->PaddleStartingPosition();
		ball->BallStartingPosition(WINDOW_WIDTH, WINDOW_HEIGHT - PADDLE_HEIGHT);
		remainingLives--;
	}
}

void Game::CheckPaddleCollision()
{
	float ballButtom = ball->position->y+ BALL_HEIGHT;
	float ballLeft = ball->position->x;
	float ballRight = ball->position->x +BALL_WIDTH;

	float paddleTop = paddle->GetY();
	float paddleLeft = paddle->GetX();
	float paddleRight = paddle->GetX() + PADDLE_WIDTH;

	//Check if there is no collision
	if (ballButtom < paddleTop)
		return;
	if (ballLeft > paddleRight)
		return;
	if (ballRight < paddleLeft)
		return;
	
	//There is collision
	float ballCenterX = ball->position->x + BALL_WIDTH / 2;
	float ballCenterY = ball->position->y + BALL_HEIGHT / 2;
	
	//Check if the ball center is above the center of the paddle 
	if (ballCenterY <= (paddle->GetY() + PADDLE_WIDTH / 2)) 
	{
		//if ball hit the upper half, reflect upwards
		ball->velocity->x = paddle->GetBallReflectionFromPaddle(ballCenterX - paddle->GetX());
		ball->velocity->y = -1;
		ball->SetDirection();
	}
	else
	{
		//if ball hit the down half(from aside), reflect down
		ball->velocity->x = -ball->velocity->x;
	}
	return;
}

void Game::CheckBrickCollision()
{
	float ballCenterX = ball->position->y + BALL_HEIGHT / 2;
	float ballCenterY = ball->position->x + BALL_WIDTH / 2;

	for (int i = 0; i < level->numRows; i++) {
		for (int j = 0; j < level->numColmns; j++) {
			Brick brick = level->bricks[i][j];

			if (brick.hitPointsRemained != 0) {
				float brickLeft = (float)level->columnSpacing * (j + 1) + level->brickWidth * j;
				float brickTop =(float) level->rowSpacing * (i + 1) + BRICK_HEIGHT * i + TEXTURE_START;

				//There is no collision with this brick;
				if (ball->position->y + BALL_HEIGHT < brickTop || //Entire ball is above the brick
					ball->position->x + BALL_WIDTH < brickLeft || //Entire ball is on the left of the brick
					ball->position->y > brickTop + BRICK_HEIGHT || //Entire ball is below the brick
					ball->position->x > brickLeft + level->brickWidth) //Entire ball is on the right of the brick
				{
					level->bricks[i][j].collisionInPreviousFrame = false;
					continue;
				}
				
				//There is collision
				if (!brick.collisionInPreviousFrame) {
					Mix_PlayChannel(-1, level->ReturnHitSoundFromBrickType(brick.id), 0);
					float brickCenterX = brickLeft + level->brickWidth / 2;
					float brickCenterY = brickTop + BRICK_HEIGHT / 2;
					float intersectionX, intersectionY;

					//Calculate intersection on Y axis
					if (ballCenterY > brickCenterY)
						intersectionY = BRICK_HEIGHT - (ball->position->y - brickTop);
					else
						intersectionY = BALL_HEIGHT - (brickTop - ball->position->y);

					if (intersectionY > BALL_HEIGHT) //If the entire ball is inside brick, set intersectionY to ball height
						intersectionY = BALL_HEIGHT;

					//Calculate intersection on X axis
					if (ballCenterX < brickCenterX)
						intersectionX = BALL_WIDTH - (brickLeft - ball->position->x);
					else
						intersectionX = BRICK_HEIGHT - (ball->position->x - brickLeft);

					if (intersectionX > BALL_WIDTH) //If the entire ball is inside brick, set intersectionX to ball width
						intersectionX = BALL_WIDTH;


					//Reflection
					if (intersectionX < intersectionY) //Hit spot is top or buttom
					{
						if (ball->velocity->y < 0)
							ball->velocity->y = 1; //Brick was hit from below;						
						else
							ball->velocity->y = -1; //Brick was hit from above
						ball->velocity->x = brick.GetBallReflectionFromBrick(ballCenterX - brickLeft, level->brickWidth);
						ball->SetDirection();
					}
					else
					{
						if (ball->velocity->x < 0)
							ball->velocity->x = 1; //Brick was hit from right
						else
							ball->velocity->x = -1; //Brick was hit from left
						ball->velocity->y = brick.GetBallReflectionFromBrick(ballCenterY - brickTop, BRICK_HEIGHT);
						ball->SetDirection();
					}

					if (brick.id != 'I')
						level->bricks[i][j].hitPointsRemained--;
					level->bricks[i][j].collisionInPreviousFrame = true;
					if (level->bricks[i][j].hitPointsRemained == 0)
					{
						Mix_PlayChannel(-1, level->ReturnBreakSoundFromBrickType(brick.id), 0);
						totalScore += level->ReturnBreakScoreFromBrick(i, j);
						remainingBricks--;
						return;
					}
				}
				
			}
		};
	}
	return;
}

void Game::RenderGameOverView()
{
	SDL_Event event;
	bool showingGameOver = true;
	while (showingGameOver) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				return;
			else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE)
					showingGameOver = false;
			}
		}
		//Render GameOver view
		SDL_RenderClear(renderer);
		FC_Draw(textViewFont, renderer, 220, 200, "Game Over!");
		string temp_str = "Total score: " + to_string(totalScore);
		FC_Draw(textFont, renderer, 340, 290, temp_str.c_str());
		FC_Draw(textFont, renderer, 280, 315, "Press Escape to close the game!");
		SDL_RenderPresent(renderer);
	}
}

void Game::RenderLevelNumberViewBeforePlay()
{
	SDL_Event event;

	bool showingLevelNumber = true;
	while (showingLevelNumber) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_SPACE)
					showingLevelNumber = false;
			}
		}
		//Render LevelNumber View
		SDL_RenderClear(renderer);
		string temp_str = "Level: " + to_string(this->currentLevel);
		FC_Draw(textViewFont, renderer, 300, 200, temp_str.c_str());
		temp_str = "Current score: " + to_string(this->totalScore);
		FC_Draw(textFont, renderer, 340, 285, temp_str.c_str());
		FC_Draw(textFont, renderer, 280, 315, "Press SPACE to start the game!");
		SDL_RenderPresent(renderer);
	}
	return;
}

void Game::RenderGameFinishedView()
{
	SDL_Event event;

	bool showingLevelNumber = true;
	while (showingLevelNumber) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				return;
			else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE)
					showingLevelNumber = false;
			}
		}
		//Render LevelNumber View
		SDL_RenderClear(renderer);
		FC_Draw(textViewFont, renderer, 220, 200, "Game Finished!");
		string temp_str = "Total score: " + to_string(totalScore);
		FC_Draw(textFont, renderer, 340, 290, temp_str.c_str());
		FC_Draw(textFont, renderer, 280, 315, "Press Escape to close the game!");
		SDL_RenderPresent(renderer);
	}
	return;
}


