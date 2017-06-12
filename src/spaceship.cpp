#include "spaceship.hpp"
#include "math_helper.hpp"

static const float M_G = 0.00000000667408;

SpaceObject::SpaceObject(float x, float y, SDL_Texture *texture, float weight) : mPosX(x), mPosY(y) {
    this->texture = texture;
    this->weight = weight;

    mVelX = 0;
    mVelY = 0;

    mAccX = 0;
    mAccY = 0;

    angle = 0;
    angle_speed = 0;
    angle_acc = 0;

    force = 0;
    force_rot = 0;

    if (!texture)
        printf("can't load texture!\n");

    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
}

SpaceObject::~SpaceObject(void) {
    SDL_DestroyTexture(texture);
}

float SpaceObject::getMass(void) {
    return weight;
}

SDL_Point SpaceObject::getPos(void) {
    SDL_Point ret = {(int) mPosX, (int) mPosY};
    return ret;
}

void SpaceObject::updatePhysics(std::vector<SpaceObject*> &v) {
    mAccX = force * sinf(deg_to_rad(angle)) / weight;
    mAccY = -force * cosf(deg_to_rad(angle)) / weight;

    angle_acc = force_rot / weight;

    for (auto &o : v) {
        if (o == this)
            continue;

        SDL_Point p = o->getPos();
        float distance_2 = powf(p.x - mPosX, 2) + powf(p.y - mPosY, 2);
        float F_g = M_G * weight * o->getMass() / distance_2;

        mAccX += F_g * (p.x - mPosX) / (weight * sqrtf(distance_2));
        mAccY += F_g * (p.y - mPosY) / (weight * sqrtf(distance_2));
    }
}

void SpaceObject::move(float timeStep) {
    mVelX += mAccX * timeStep;
    mVelY += mAccY * timeStep;

    mPosX += mVelX * timeStep;
    mPosY += mVelY * timeStep;

    angle_speed += angle_acc * timeStep;
    angle += angle_speed * timeStep;
}

void SpaceObject::render(SDL_Renderer * renderer, SDL_Rect &camera) {
    SDL_Rect rect = {(int) mPosX - width / 2 - camera.x, (int) mPosY - height / 2 - camera.y, width, height};
    SDL_Point center = {width/2, height/2}; // the center where the texture will be rotated.

    SDL_RenderCopyEx(renderer, texture, NULL , &rect, angle, &center, SDL_FLIP_NONE);
}

void SpaceObject::printDiagnostics(DebugOverlay *overlay) {
    overlay->print("Vx: %.2f m/s + %.2f m/s^2", mVelX, mAccX);
    overlay->print("Vy: %.2f m/s + %.2f m/s^2", mVelY, mAccY);
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
                force_rot = 10 * force_max * e.jaxis.value / (float) 0x10000 / 2;
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