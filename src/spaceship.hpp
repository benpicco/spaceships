#pragma once

#include <SDL.h>
#include "DebugOverlay.hpp"

class Spaceship {
	float mPosX, mPosY;
	float mVelX, mVelY;
	float angle;

	int width, height;
	SDL_Point center;

	SDL_Texture *texture;

public:
	Spaceship(SDL_Texture *texture);

	~Spaceship(void);

	//Takes key presses and adjusts the ship's velocity
	void handleEvent(SDL_Event& e);

	//Moves the ship
	void move(float timeStep);

	//Shows the dot on the screen
	void render(SDL_Renderer * renderer);

	void printDiagnostics(DebugOverlay * overlay);
};