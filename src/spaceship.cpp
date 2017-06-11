#include "spaceship.hpp"
#include "math_helper.hpp"

SpaceObject::SpaceObject(float x, float y, SDL_Texture *texture, float weight) : mPosX(x), mPosY(y) {
    this->texture = texture;
    this->weight = weight;

    mVelX = 0;
    mVelY = 0;

    mAccX = 0;
    mAccY = 0;

    angle = 0;
    force = 0;

    if (!texture)
        printf("can't load texture!\n");

    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
}

SpaceObject::~SpaceObject(void) {
    SDL_DestroyTexture(texture);
}

void SpaceObject::updatePhysics(void) {
    mAccX = force * sinf(deg_to_rad(angle)) / weight;
    mAccY = -force * cosf(deg_to_rad(angle)) / weight;
}

void SpaceObject::move(float timeStep) {
    mVelX += mAccX * timeStep;
    mVelY += mAccY * timeStep;

    mPosX += mVelX * timeStep;  
    mPosY += mVelY * timeStep;
}

void SpaceObject::render(SDL_Renderer * renderer) {
    SDL_Rect rect = {(int) mPosX - width / 2, (int) mPosY - height / 2, width, height};
    SDL_Point center = {width/2, height/2}; // the center where the texture will be rotated.

    SDL_RenderCopyEx(renderer, texture, NULL , &rect, angle, &center, SDL_FLIP_NONE);
}

void SpaceObject::printDiagnostics(DebugOverlay *overlay) {
    overlay->print("x: %.2f", mPosX);
    overlay->print("y: %.2f", mPosY);
}

Spaceship::Spaceship(float x, float y, SDL_Texture *texture, float weight, float force_max) : SpaceObject(x, y, texture, weight) {
    this->force_max = force_max;
}

void Spaceship::handleEvent(SDL_Event& e) {
    switch (e.type)
    {
        case SDL_JOYAXISMOTION:

            // dead zone
            if (abs(e.jaxis.value) < 3200)
                break;

            if ( e.jaxis.axis == 0) {

                /* Left-right movement code goes here */
                if (e.jaxis.value < 0)
                    angle -= 1.f;
                if (e.jaxis.value > 0)
                    angle += 1.f;
            }

            if ( e.jaxis.axis == 1) {

                /* Up-Down movement code goes here */
            }

            // left trigger
            if ( e.jaxis.axis == 2) { // reverse
                force = -force_max * (e.jaxis.value + 0x10000/2) / (float) 0x10000;
            }

            // right trigger
            if ( e.jaxis.axis == 5) {
                force = force_max * (e.jaxis.value + 0x10000/2) / (float) 0x10000;
            }

            break;
    }
}