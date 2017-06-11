#include "spaceship.hpp"
#include "math_helper.hpp"

Spaceship::Spaceship(SDL_Texture *texture, float weight, float force_max) {
    this->texture = texture;
    this->force_max = force_max;
    this->weight = weight;

    mPosX = 200;
    mPosY = 100;

    mVelX = 0;
    mVelY = 0;

    mAccX = 0;
    mAccY = 0;

    SDL_QueryTexture(texture, NULL, NULL, &width, &height);

    angle = 0;
    force = 0;
    center = {width/2, height/2}; // the center where the texture will be rotated.
}

Spaceship::~Spaceship(void) {
    SDL_DestroyTexture(texture);
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

void Spaceship::updatePhysics(void) {
    mAccX = force * sinf(deg_to_rad(angle)) / weight;
    mAccY = -force * cosf(deg_to_rad(angle)) / weight;
}

void Spaceship::move(float timeStep) {
    mVelX += mAccX * timeStep;
    mVelY += mAccY * timeStep;

    mPosX += mVelX * timeStep;  
    mPosY += mVelY * timeStep;
}

void Spaceship::render(SDL_Renderer * renderer) {
    SDL_Rect rect = {(int) mPosX, (int) mPosY, width, height};

    SDL_RenderCopyEx(renderer, texture, NULL , &rect, angle, &center, SDL_FLIP_NONE);
}

void Spaceship::printDiagnostics(DebugOverlay *overlay) {
    overlay->print("x: %.2f", mPosX);
    overlay->print("y: %.2f", mPosY);
}