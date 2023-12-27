#include"Level.h"

Level::Level(SDL_Renderer* renderer, int currentLevel)
{
	this->renderer = renderer;
	if (!this->GetLevelFromXml(currentLevel)) {
		cout << "Error loading level from XML file!";
		return;
	}
	SDL_SetRenderDrawColor(renderer, 128, 128, 128, 0);
	brickWidth = (float)(WINDOW_WIDTH - (this->numColmns + 1) * this->columnSpacing) / this->numColmns;
}

Level::~Level()
{
	SDL_DestroyTexture(levelTexture);
}

bool Level::GetLevelFromXml(int currentLevel)
{
	XMLDocument levelDoc;
	string level = "Level" + to_string(currentLevel) + ".xml";
	XMLError reader = levelDoc.LoadFile(level.c_str());
	SDL_Surface* surface;
	const char* imageSound_path;

	XMLElement* levelElement = levelDoc.FirstChildElement("Level");
	if (levelElement == nullptr) {
		cout << "Level node not found!";
		return XML_ERROR_FILE_READ_ERROR;
	}
	else {
		//Get Level atributes
		this->numRows = stoi(levelElement->Attribute("RowCount"));
		this->numColmns = stoi(levelElement->Attribute("ColumnCount"));
		this->rowSpacing = stoi(levelElement->Attribute("RowSpacing"));
		this->columnSpacing = stoi(levelElement->Attribute("ColumnSpacing"));
		
		//Get Level background texture and load it
		imageSound_path = levelElement->Attribute("BackgroundTexture");
		surface = IMG_Load(imageSound_path);
		if (!surface) {
			cout << "Failed to load image at " << imageSound_path << ": " << SDL_GetError();
			return false;
		}
		this->levelTexture = SDL_CreateTextureFromSurface(renderer, surface);

		
		//Get brickTypes element
		XMLElement* brickTypesElement = levelElement->FirstChildElement();
		int brickTypeCnt = brickTypesElement->ChildElementCount();
		
		//Create array of BrickTypes
		this->brickTypes = new BrickType[brickTypeCnt];

		if (this->brickTypes == nullptr) {
			cout << "BrickType array not created!";
			return false;
		}
		else {
			//Get First BrickType
			XMLElement* brickTypeChild = brickTypesElement->FirstChildElement();
			if (brickTypeChild == nullptr) {
				cout << "No brick type found in XML";
				return false;
			}
			else {
				int i = 0;
				//Get BrickType ID
				while (brickTypeChild != nullptr && i < brickTypeCnt) {
					if (brickTypeChild->Attribute("Id")[0]) {
						this->brickTypes[i].SetID(brickTypeChild->Attribute("Id")[0]);
					}

					//Get BrickType texture and load it
					if (brickTypeChild->Attribute("Texture")) {
						imageSound_path = brickTypeChild->Attribute("Texture");
						surface = IMG_Load(imageSound_path);
						if (!surface) {
							cout << "Failed to load image at " << imageSound_path << ": " << SDL_GetError();
							return false;
						}
						this->brickTypes[i].SetTexture(SDL_CreateTextureFromSurface(renderer, surface));
					}

					//Get BrickType HitPOints
					string hitPoints = brickTypeChild->Attribute("HitPoints");
					
					if ((string)brickTypeChild->Attribute("HitPoints") == "Infinite") {
						this->brickTypes[i].SetHitPoints(INT_MAX);
					}
					else {
						this->brickTypes[i].SetHitPoints(stoi(brickTypeChild->Attribute("HitPoints")));
					}

					//Get brickType HitSound and load it
					if (brickTypeChild->Attribute("HitSound")) {
						imageSound_path = brickTypeChild->Attribute("HitSound");
						Mix_Chunk* sound = Mix_LoadWAV(imageSound_path);
						if (!sound) {
							cout << "Sound error: " << Mix_GetError();
							return false;
						}
						this->brickTypes[i].SetHitSound(sound);
					}

					//Get BrickType BreakSound and load it
					if (brickTypeChild->Attribute("BreakSound")) {
						if (this->brickTypes[i].GetID() != 'I') {
							imageSound_path = brickTypeChild->Attribute("BreakSound");
							Mix_Chunk* sound = Mix_LoadWAV(imageSound_path);
							if (!sound) {
								cout << "Sound error: " << Mix_GetError();
								return false;
							}
							this->brickTypes[i].SetBreakSound(sound);
						}
					}

					//Get brickType break score
					if (brickTypeChild->Attribute("BreakScore")) {
						this->brickTypes[i].SetBreakScore(stoi(brickTypeChild->Attribute("BreakScore")));
					}

					//Go to next sibling of BrickType
					brickTypeChild = brickTypeChild->NextSiblingElement();
					i++;
				}
				SDL_FreeSurface(surface);
			}
		}

		this->bricks = new Brick* [this->numRows];
		for (int i = 0; i < this->numRows; i++) {
			this->bricks[i] = new Brick[this->numColmns];
		}

		if (this->bricks)
		{
			XMLElement* bricks = brickTypesElement->NextSiblingElement();

			if (bricks) {
				string brickStr = bricks->GetText();
				brickStr.erase(remove(brickStr.begin(), brickStr.end(), '\n'), brickStr.cend());
				brickStr.erase(remove(brickStr.begin(), brickStr.end(), '\t'), brickStr.cend());
				brickStr.erase(remove(brickStr.begin(), brickStr.end(), ' '), brickStr.cend());
				Brick* tempBrick = new Brick();
				int stringNum;
				for (int i = 0; i < this->numRows; i++)
					for (int j = 0; j < this->numColmns; j++) {
						stringNum = i * this->numColmns + j;
						tempBrick->id = brickStr[stringNum];
						tempBrick->hitPointsRemained = this->ReturnHitPointsFromBrickType(brickStr[stringNum]);
						tempBrick->collisionInPreviousFrame = false;
						if (tempBrick->hitPointsRemained == -1) {
							cout << "Error: Brick is not in BrickTypes array!";
								return false;
						}
						this->bricks[i][j] = *tempBrick;
					};
				delete(tempBrick);

			}
		}
	}
	return true;
}

int Level::ReturnHitPointsFromBrickType(char id)
{
	for (int i = 0; i < sizeof(this->brickTypes); i++) {
		if (id == '_')
		{
			return 0;
		}
		if (this->brickTypes[i].GetID() == id)
			return this->brickTypes[i].GetHitPoints();
	}
	return -1;
}

unsigned int Level::CountBricks()
{
	int remainedBricks = 0;
	for (int i = 0; i < this->numRows; i++)
		for (int j = 0; j < this->numColmns; j++)
			if ((bricks[i][j].id != '_') && (bricks[i][j].id != 'I'))
				remainedBricks++;

	return remainedBricks;
}

void Level::RenderLevel(float delta)
{
	SDL_Rect texturerect{ 0, TEXTURE_START, WINDOW_WIDTH, WINDOW_HEIGHT};

	SDL_RenderCopy(renderer, this->levelTexture, 0, &texturerect);
	
	for(int i=0; i<this->numRows; i++)
		for (int j = 0; j < this->numColmns; j++) {
			if (!this->bricks[i][j].hitPointsRemained)
				continue;
			SDL_Rect srcrect;
			srcrect.x = (j + 1) * this->columnSpacing + j * (int)round(this->brickWidth);
			srcrect.y = (i + 1) * this->rowSpacing + i * BRICK_HEIGHT + texturerect.y;
			srcrect.w = (int)round(this->brickWidth);
			srcrect.h = BRICK_HEIGHT;

			//If brick does not change color when hit
			//SDL_RenderCopy(renderer, this->ReturnBrickTextureFromBrickType(bricks[i][j].id), 0, &srcrect); 

			//If brick does change color when hit
			SDL_RenderCopy(renderer, this->ReturnBrickTextureFromBrickType(bricks[i][j].hitPointsRemained), 0, &srcrect);

		}
}

SDL_Texture* Level::ReturnBrickTextureFromBrickType(char id)
{
	for (int i = 0; i < sizeof(this->brickTypes); i++) {
		if (this->brickTypes[i].GetID() == id)
			return this->brickTypes[i].GetTexture();
	}
	return nullptr;
}

SDL_Texture* Level::ReturnBrickTextureFromBrickType(int hitPointsRemained)
{
	for (int i = 0; i < sizeof(this->brickTypes); i++) {
		if (this->brickTypes[i].GetHitPoints() == hitPointsRemained)
			return this->brickTypes[i].GetTexture();
	}
	return nullptr;
}

int Level::ReturnBreakScoreFromBrick(int x, int y)
{
	char brickTypeId = this->bricks[x][y].id;
	for (int i = 0; i < sizeof(this->brickTypes); i++)
		if (this->brickTypes[i].GetID() == brickTypeId)
			return this->brickTypes[i].GetBreakScore();
	return 0;
}

Mix_Chunk* Level::ReturnHitSoundFromBrickType(char id)
{
	for (int i = 0; i < sizeof(this->brickTypes); i++) {
		if (this->brickTypes[i].GetID() == id)
			return this->brickTypes[i].GetHitSound();
	}
	return nullptr;
}

Mix_Chunk* Level::ReturnBreakSoundFromBrickType(char id)
{
	for (int i = 0; i < sizeof(this->brickTypes); i++) {
		if (this->brickTypes[i].GetID() == id)
			return this->brickTypes[i].GetBreakSound();
	}
	return nullptr;
}



