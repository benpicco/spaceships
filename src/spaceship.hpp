#pragma once

#include <SDL.h>
#include "DebugOverlay.hpp"

class Spaceship {
	float mPosX, mPosY;
	float mVelX, mVelY;
	float mAccX, mAccY;
	float angle;
	
	float force;
	float force_max;

	float weight;

	int width, height;
	SDL_Point center;

	SDL_Texture *texture;

public:
	Spaceship(SDL_Texture *texture, float weight, float force_max);

	~Spaceship(void);

	//Takes key presses and adjusts the ship's velocity
	void handleEvent(SDL_Event& e);

	//Moves the ship
	void move(float timeStep);

	void updatePhysics(void);

	//Shows the dot on the screen
	void render(SDL_Renderer * renderer);

	void printDiagnostics(DebugOverlay * overlay);
};