#pragma once

#include <SDL.h>
#include "DebugOverlay.hpp"

class SpaceObject {
protected:
	float mPosX, mPosY;
	float mVelX, mVelY;
	float mAccX, mAccY;
	float angle;
	
	float force;

	float weight;

	int width, height;

	SDL_Texture *texture;

public:
	SpaceObject(float x, float y, SDL_Texture *texture, float weight);

	~SpaceObject(void);

	void move(float timeStep);

	void updatePhysics(void);

	//Shows the dot on the screen
	void render(SDL_Renderer * renderer);

	void printDiagnostics(DebugOverlay * overlay);
};

class Spaceship : public SpaceObject {
	float force_max;

public:
	Spaceship(float x, float y, SDL_Texture *texture, float weight, float force_max);

	//Takes key presses and adjusts the ship's velocity
	void handleEvent(SDL_Event& e);
};