#include "Menu.h"

bool Menu::quitFlag = false;
int Menu::loadLevelFlag = 0;
bool Menu::reloadFlag = false;
bool Menu::isInMainMenuFlag = false;
bool Menu::isInPauseMenuFlag = false;

Menu::Menu() {
	ButtonArray = NULL;
	Background = NULL;
}

Menu::~Menu() {
	if (Background) {
		SDL_DestroyTexture(Background);
		Background = NULL;
	}
}

#pragma region FlagFunctionsDef
void Menu::clearFlags() {
	loadLevelFlag = 0;
	reloadFlag = false;
}

bool Menu::getQuitFlag() {
	return quitFlag;
}

void Menu::setQuitFlag() {
	quitFlag = true;
}

int Menu::getLoadLevelFlag() {
	return loadLevelFlag;
}

void Menu::loadLevel1() {
	loadLevelFlag = 1;
	isInMainMenuFlag = false;
}

void Menu::loadLevel2() {
	loadLevelFlag = 2;
	isInMainMenuFlag = false;
}

void Menu::loadLevel3() {
	loadLevelFlag = 3;
	isInMainMenuFlag = false;
}

void Menu::loadLevel4() {
	loadLevelFlag = 4;
	isInMainMenuFlag = false;
}

void Menu::loadLevel5() {
	loadLevelFlag = 5;
	isInMainMenuFlag = false;
}

bool Menu::getReloadFlag() {
	return reloadFlag;
}

void Menu::setReloadFlag() {
	reloadFlag = true;
	isInPauseMenuFlag = false;
}

bool Menu::getMainMenuFlag() {
	return isInMainMenuFlag;
}

void Menu::setMainMenuFlag() {
	isInMainMenuFlag = true;
}

bool Menu::getPauseMenuFlag() {
	return isInPauseMenuFlag;
}

void Menu::setPauseMenuFlag() {
	isInPauseMenuFlag = true;
}

void Menu::clearPauseMenuFlag() {
	isInPauseMenuFlag = false;
}
#pragma endregion

MainMenu::MainMenu() :Menu() {}

MainMenu::~MainMenu() {
	if (ButtonArray) {
		for (int i = 0; i < 6; ++i) {
			if (ButtonArray[i]) {
				delete ButtonArray[i];
				ButtonArray[i] = NULL;
			}
		}
		delete[] ButtonArray;
		ButtonArray = NULL;
	}
}

void MainMenu::Init() {
	ButtonArray = new Button * [6];
	ButtonArray[Quit] = new Button({ 200,310,200,60 }, &Menu::setQuitFlag, "Quit");
	ButtonArray[Load_1p_1] = new Button({ 120,110,120,60 }, &Menu::loadLevel1, "Level 1");
	ButtonArray[Load_1p_2] = new Button({ 240,110,120,60 }, &Menu::loadLevel2, "Level 2");
	ButtonArray[Load_1p_3] = new Button({ 360,110,120,60 }, &Menu::loadLevel3, "Level 3");
	ButtonArray[Load_2p_coop] = new Button({ 60,170,240,60 }, &Menu::loadLevel4, "2 Player CO-OP");
	ButtonArray[Load_2p_vs] = new Button({ 300,170,240,60 }, &Menu::loadLevel5, "2 Player Versus");
}

void MainMenu::Show() {
	//Send background to renderer
	SDL_SetRenderDrawColor(TextureManager::Renderer, 0x11, 0x05, 0x80, 0xFF);
	SDL_RenderFillRect(TextureManager::Renderer, NULL);

	//Transform title text into a texture and send it to renderer
	SDL_Surface* textSurface = TTF_RenderText_Solid(TextureManager::Font[0], "Bomberman", { 0xFF,0xFF,0xFF });
	if (textSurface == NULL) {
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else {
		int tWidth, tHeight;
		//Create texture from surface pixels
		SDL_Texture* tTexture = SDL_CreateTextureFromSurface(TextureManager::Renderer, textSurface);
		if (tTexture == NULL) {
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else {
			//Get text image dimensions
			tWidth = textSurface->w;
			tHeight = textSurface->h;

			//Set text to render centered on x axis
			SDL_Rect temp;
			temp.x = (600 - tWidth) / 2;
			temp.y = 30;
			temp.w = tWidth;
			temp.h = tHeight;
			SDL_RenderCopy(TextureManager::Renderer, tTexture, NULL, &temp);
			//Get rid of old surface
			SDL_FreeSurface(textSurface);
			SDL_DestroyTexture(tTexture);
		}
	}

	for (int i = 0; i < 6; ++i) {
		ButtonArray[i]->Show();
	}

	SDL_RenderPresent(TextureManager::Renderer);
}

void MainMenu::Hide() {
	SDL_SetRenderDrawColor(TextureManager::Renderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(TextureManager::Renderer);
	isInMainMenuFlag = false;
}

void MainMenu::Destroy() {
	if (ButtonArray) {
		for (int i = 0; i < 6; ++i) {
			if (ButtonArray[i]) {
				delete ButtonArray[i];
				ButtonArray[i] = NULL;
			}
		}
		delete[] ButtonArray;
		ButtonArray = NULL;
	}
}

PauseMenu::PauseMenu() :Menu() {}

PauseMenu::~PauseMenu() {
	if (ButtonArray) {
		for (int i = 0; i < 3; ++i) {
			if (ButtonArray[i]) {
				delete ButtonArray[i];
				ButtonArray[i] = NULL;
			}
		}
		delete[] ButtonArray;
		ButtonArray = NULL;
	}
}

void PauseMenu::Init() {
	SDL_Texture* TempTexture = SDL_CreateTexture(TextureManager::Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 600, 400);
	if (TempTexture == NULL) {
		printf("Unable to render blank background texture! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else {
		SDL_SetRenderTarget(TextureManager::Renderer, TempTexture);
		SDL_RenderPresent(TextureManager::Renderer);
		SDL_SetRenderTarget(TextureManager::Renderer, NULL);
	}
	if (Background) {
		SDL_DestroyTexture(Background);
	}
	Background = TempTexture;

	if (ButtonArray == NULL) {
		ButtonArray = new Button * [3];
		ButtonArray[Quit] = new Button({ 200,310,200,70 }, &Menu::setQuitFlag, "Quit Game");
		ButtonArray[QuitToMenu] = new Button({ 200,210,200,70 }, &Menu::setReloadFlag, "Main Menu");
		ButtonArray[Resume] = new Button({ 200,110,200,70 }, &Menu::clearPauseMenuFlag, "Resume");
	}
}

void PauseMenu::Show() {
	//Send background to renderer

	SDL_SetTextureColorMod(Background, 0x7F, 0x7F, 0x7F);
	SDL_RenderCopy(TextureManager::Renderer, Background, NULL, NULL);
	SDL_SetTextureColorMod(Background, 0xFF, 0xFF, 0xFF);

	//Transform title text into a texture and send it to renderer
	SDL_Surface* textSurface = TTF_RenderText_Solid(TextureManager::Font[0], "Paused", { 0xFF,0xFF,0xFF });
	if (textSurface == NULL) {
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else {
		int tWidth, tHeight;
		//Create texture from surface pixels
		SDL_Texture* tTexture = SDL_CreateTextureFromSurface(TextureManager::Renderer, textSurface);
		if (tTexture == NULL) {
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else {
			//Get text image dimensions
			tWidth = textSurface->w;
			tHeight = textSurface->h;

			//Set text to render centered on x axis
			SDL_Rect temp;
			temp.x = (600 - tWidth * 2) / 2;
			temp.y = 30;
			temp.w = tWidth * 2;
			temp.h = tHeight * 2;
			SDL_RenderCopy(TextureManager::Renderer, tTexture, NULL, &temp);
			//Get rid of old surface
			SDL_FreeSurface(textSurface);
			SDL_DestroyTexture(tTexture);
		}
	}

	for (int i = 0; i < 3; ++i) {
		ButtonArray[i]->Show();
	}

	SDL_RenderPresent(TextureManager::Renderer);
}

void PauseMenu::Hide() {
	SDL_SetRenderDrawColor(TextureManager::Renderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(TextureManager::Renderer);
	isInPauseMenuFlag = false;
}

void PauseMenu::Destroy() {
	if (ButtonArray) {
		for (int i = 0; i < 3; ++i) {
			if (ButtonArray[i]) {
				delete ButtonArray[i];
				ButtonArray[i] = NULL;
			}
		}
		delete[] ButtonArray;
		ButtonArray = NULL;
	}
}