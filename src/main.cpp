#include <vector>

#include <SDL.h>
#include <SDL_image.h>

#include "DebugOverlay.hpp"
#include "spaceship.hpp"
#include "LTimer.hpp"

using namespace std;

#define JOYSTICK_DEAD_ZONE

static float getFrameMs(void) {
    static int ticks_old = 0;

    int ret = SDL_GetTicks() - ticks_old;
    ticks_old = SDL_GetTicks();

    return ret;
}

int main(int argc, char ** argv)
{
    bool quit = false;
    SDL_Event event;
 
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
    IMG_Init(IMG_INIT_PNG);
 
    SDL_Window * window = SDL_CreateWindow("Spaceships", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_JoystickEventState(SDL_ENABLE);
    SDL_Joystick * joystick = SDL_JoystickOpen(0);

    if (joystick) {
        printf("opened joystick\n");
    }

    DebugOverlay * overlay = new DebugOverlay();

    vector<SpaceObject*> spaceObjects;

    spaceObjects.push_back(new SpaceObject(600, 600, IMG_LoadTexture(renderer, "assets/planet.png"), 1000000000000000));

    Spaceship * ship = new Spaceship(100, 100, IMG_LoadTexture(renderer, "assets/1.png"), 100, 10000);

    spaceObjects.push_back(ship);

    LTimer stepTimer;

    while (!quit)
    {
        overlay->print("%.1f ms", getFrameMs());

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
            }

            ship->handleEvent(event);
        }
 
        //Calculate time step
        float timeStep = stepTimer.getTicks() / 1000.f;

        for (auto &o : spaceObjects) {
            o->updatePhysics(spaceObjects);
            o->move(timeStep);
        }

        stepTimer.start();

        // Select the color for drawing. It is set to red here.
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        // Clear the entire screen to our selected color.
        SDL_RenderClear(renderer);

        ship->printDiagnostics(overlay);

        for (auto &o : spaceObjects)
            o->render(renderer);

        overlay->render(renderer);

        SDL_RenderPresent(renderer);

        SDL_Delay(20);
    }
 
    if (joystick)
        SDL_JoystickClose(joystick);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
 
    return 0;
}