#pragma once

#include <vector>
#include <SDL.h>
#include "DebugOverlay.hpp"

class SpaceObject {
protected:
	float mPosX, mPosY;
	float mVelX, mVelY;
	float mAccX, mAccY;
	float angle, angle_speed, angle_acc;
	
	float force;
	float force_rot;

	float weight;

	int width, height;

	SDL_Texture *texture;

public:
	SpaceObject(float x, float y, SDL_Texture *texture, float weight);

	~SpaceObject(void);

	void move(float timeStep);

	void updatePhysics(std::vector<SpaceObject*> &v);

	//Shows the dot on the screen
	void render(SDL_Renderer * renderer);

	void printDiagnostics(DebugOverlay * overlay);

	float getMass(void);

	SDL_Point getPos(void);
};

class Spaceship : public SpaceObject {
	float force_max;

public:
	Spaceship(float x, float y, SDL_Texture *texture, float weight, float force_max);

	//Takes key presses and adjusts the ship's velocity
	void handleEvent(SDL_Event& e);
};