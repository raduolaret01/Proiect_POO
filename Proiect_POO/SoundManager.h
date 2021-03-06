#pragma once

#include <stdio.h>
#include <SDL_mixer.h>
#include "logFile.h"




class SoundManager{
public:
	enum MusicEnum {
		Menu,
		Game
	};

	enum SFXEnum {
		Splash,
		MouseOver,
		Click,
		Bomb_Place,
		Bomb_Fuze,
		Bomb_Boom,
		Step,
		NPC_Death,
		Player_Death
	};

	static Mix_Music* BGM[2];
	static Mix_Chunk* SFX[10];

	SoundManager();
	~SoundManager();

	static Mix_Chunk* loadSoundEffect(const char*);
	static bool loadSFX();
	static Mix_Music* loadMusic(const char*);
	static bool loadBGM();
	static void freeSounds();
};

