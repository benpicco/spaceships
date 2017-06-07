#include "spaceship.hpp"

Spaceship::Spaceship(SDL_Texture *texture) {
    this->texture = texture;

    mPosX = 200;
    mPosY = 100;

    mVelX = 0;
    mVelY = 0;

    SDL_QueryTexture(texture, NULL, NULL, &width, &height);

    angle = 180.0f;
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

            printf("%d: %d\n", e.jaxis.axis, e.jaxis.value);

            if ( e.jaxis.axis == 0) {

                /* Left-right movement code goes here */
                if (e.jaxis.value < 0)
                    mVelX -= 0.1f;
                if (e.jaxis.value > 0)
                    mVelX += 0.1f;
            }

            if ( e.jaxis.axis == 1) {

                /* Up-Down movement code goes here */
                if (e.jaxis.value < 0)
                    mVelY -= 0.1f;
                if (e.jaxis.value > 0)
                    mVelY += 0.1f;
            }

            // left trigger
            if ( e.jaxis.axis == 2) {
                angle = -180.f * (e.jaxis.value + 0x10000/2) / (float) 0x10000;
            }

            // right trigger
            if ( e.jaxis.axis == 5) {
                angle = 180.f * (e.jaxis.value + 0x10000/2) / (float) 0x10000;
            }

            break;
    }
}

void Spaceship::move(float timeStep) {
    //Move the dot left or right
    mPosX += mVelX * timeStep;

    //If the dot went too far to the left or right
    if ( mPosX < 0 )
    {
        mPosX = 0;
    }
//    else if( mPosX > SCREEN_WIDTH - DOT_WIDTH )
//    {
//        mPosX = SCREEN_WIDTH - DOT_WIDTH;
//    }
    
    //Move the dot up or down
    mPosY += mVelY * timeStep;

    //If the dot went too far up or down
    if ( mPosY < 0 )
    {
        mPosY = 0;
    }
//    else if( mPosY > SCREEN_HEIGHT - DOT_HEIGHT )
//    {
//        mPosY = SCREEN_HEIGHT - DOT_HEIGHT;
//    }
}

void Spaceship::render(SDL_Renderer * renderer) {
    SDL_Rect rect = {(int) mPosX, (int) mPosY, width, height};

    SDL_RenderCopyEx(renderer, texture, NULL , &rect, angle, &center, SDL_FLIP_NONE);
}