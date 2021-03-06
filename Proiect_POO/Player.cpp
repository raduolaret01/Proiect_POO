#include <cmath>
#include "Player.h"

int Player::numberOfAnimStates = 8;
AnimationState* Player::animationStates = NULL;

Player::Player() : SolidObj() {
	soundChannel = Mix_PlayChannel(-1, SoundManager::SFX[SoundManager::Step], -1);
	if (soundChannel == -1) {
		Logs::logF << "Sound system error: " << Mix_GetError() << std::endl;
	}
	else {
		Mix_Pause(soundChannel);
	}
	range = 1;
	direction = 0;
	currentAnimState = animationStates;
	speed = 0;
	placedBombs = 0;
	maxPlacedBombs = 1;
	isDead = false;
}

Player::Player(const SDL_Point coord, const int w, const int h) : SolidObj(coord, w, h) {
	soundChannel = Mix_PlayChannel(-1, SoundManager::SFX[SoundManager::Step], -1);
	if (soundChannel == -1) {
		Logs::logF << "Sound system error: " << Mix_GetError() << std::endl;
	}
	else {
		Mix_Pause(soundChannel);
	}
	range = 1;
	direction = 0;
	currentAnimState = animationStates;
	speed = 0;
	placedBombs = 0;
	maxPlacedBombs = 1;
	isDead = false;
}

Player::~Player() {

}

int Player::getAnimState() {
	return (int)((currentAnimState - animationStates) / sizeof(AnimationState));
}

void Player::setAnimState(int x) {
	currentAnimFrame = 0;
	currentAnimState = animationStates + x;
}

bool Player::isOnLastAnimFrame() {
	return currentAnimFrame == currentAnimState->numberOfFrames - 1;
}

int Player::Show(SDL_Point Offset) {
	currentFrameTime += Timer::getDTime();
	if (soundChannel != -1) {
		if (speed == 0) {
			Mix_Pause(soundChannel);
		}
		else {
			Mix_Resume(soundChannel);
		}
	}

	if (currentFrameTime / 60 >= currentAnimState->secondsPerFrame[currentAnimFrame]) {
		
		if (currentAnimState == animationStates + Defeat && isOnLastAnimFrame()) {
			Mix_HaltChannel(soundChannel);
			return -1;
		}

		if (currentAnimState->numberOfFrames > 1 && isOnLastAnimFrame()) {
			currentAnimFrame = 0;
			currentFrameTime = 0;
		}
		else {
			++currentAnimFrame;
			currentFrameTime = 0;
		}
	}
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if (direction == Left) {
		flip = SDL_FLIP_HORIZONTAL;
	}

	SDL_Point spriteOffset = { (currentAnimState->textureArea.w - 12) / 2,currentAnimState->textureArea.h - 12 };
	SDL_Rect TempSrc = { currentAnimState->textureArea.x + currentAnimFrame * currentAnimState->frameOffset,currentAnimState->textureArea.y,currentAnimState->textureArea.w,currentAnimState->textureArea.h };
	SDL_Rect TempDest = { (int)((hitbox.x - spriteOffset.x) * 2 + Offset.x),(int)((hitbox.y - spriteOffset.y) * 2 + Offset.y),currentAnimState->textureArea.w * 2,currentAnimState->textureArea.h * 2 };
	SDL_RenderCopyEx(TextureManager::Renderer, TextureManager::Texture[TextureManager::Player1], &TempSrc, &TempDest, 0, NULL, flip);

	return 0;
}

void Player::Update(int dTime, float collisionDist, bool dirChanged, bool spdChanged) {
	//Speed is more of a movement flag: 0 = no movement;1 = forced movement(eg: conveyor belts);2 = voluntary movement
	dTime = std::min(dTime, 20);
	if (speed == 2) {
		if (dirChanged||spdChanged) {
			switch (direction) {
			case Down:
				setAnimState(DownMove);
				break;
			case Right:
			case Left:
				setAnimState(SideMove);
				break;
			case Up:
				setAnimState(UpMove);
				break;
			}
		}
		if(collisionDist!=-1.0f){
			switch (direction) {
			case Down:
				hitbox.y += std::min(speed * dTime / 40.0f, collisionDist);
				break;
			case Right:
				hitbox.x += std::min(speed * dTime / 40.0f, collisionDist);
				break;
			case Left:
				hitbox.x -= std::min(speed * dTime / 40.0f, collisionDist);
				break;
			case Up:
				hitbox.y -= std::min(speed * dTime / 40.0f, collisionDist);
				break;
			}
		}
		else {
			switch (direction) {
			case Down:
				hitbox.y += speed * dTime / 40.0f;
				break;
			case Right:
				hitbox.x += speed * dTime / 40.0f;
				break;
			case Left:
				hitbox.x -= speed * dTime / 40.0f;
				break;
			case Up:
				hitbox.y -= speed * dTime / 40.0f;
				break;
			}
		}
	}
	else {
		switch (direction) {
		case Down:
			setAnimState(DownStill);
			break;
		case Right:
		case Left:
			setAnimState(SideStill);
			break;
		case Up:
			setAnimState(UpStill);
			break;
		}
	}
	
}

bool Player::setDirection(int dir) {
	dir %= 4;
	if (direction != dir) {
		direction = dir;
		return true;
	}
	return false;
}

bool Player::setSpeed(int sp) {
	if (speed != sp) {
		speed = sp;
		return true;
	}
	return false;
}

int Player::getDirection() {
	return direction;
}

bool Player::loadAnimationStates(std::ifstream* input) {
	animationStates = new AnimationState[numberOfAnimStates];
	for (int i = 0; i < numberOfAnimStates; ++i) {
		*input >> animationStates[i].textureArea.x >> animationStates[i].textureArea.y >> animationStates[i].textureArea.w >> animationStates[i].textureArea.h;
		*input >> animationStates[i].numberOfFrames >> animationStates[i].frameOffset;
		animationStates[i].secondsPerFrame = new int[animationStates[i].numberOfFrames];
		for (int j = 0; j < animationStates[i].numberOfFrames; ++j) {
			*input >> animationStates[i].secondsPerFrame[j];
		}
	}
	return true;
}

void Player::unloadAnimationStates() {
	if (animationStates) {
		delete[] animationStates;
		animationStates = NULL;
	}
}