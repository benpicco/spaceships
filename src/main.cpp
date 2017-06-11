#include <SDL.h>
#include <SDL_image.h>

#include "DebugOverlay.hpp"
#include "spaceship.hpp"
#include "LTimer.hpp"

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
 
    SDL_Window * window = SDL_CreateWindow("SDL2 Sprite Sheets", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_JoystickEventState(SDL_ENABLE);
    SDL_Joystick * joystick = SDL_JoystickOpen(0);

    if (joystick) {
        printf("opened joystick\n");
    }

    // for this to work, i will assume you already have a SDL_Renderer and a SDL_Window.
    SDL_Texture * texture = IMG_LoadTexture(renderer, "assets/1.png");

    DebugOverlay * overlay = new DebugOverlay();

    Spaceship * ship = new Spaceship(texture, 100, 10000);

    LTimer stepTimer;

    while (!quit)
    {
        overlay->print("FPS: %.1f", getFrameMs());

        while (SDL_PollEvent(&event)) {
            switch (event.type)
            {
                case SDL_QUIT:
                    quit = true;
                    break;
            }

            ship->handleEvent(event);
        }
 
        //Calculate time step
        float timeStep = stepTimer.getTicks() / 1000.f;

        ship->updatePhysics();

        //Move for time step
        ship->move(timeStep);

        //Restart step timer
        stepTimer.start();

        // Select the color for drawing. It is set to red here.
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        // Clear the entire screen to our selected color.
        SDL_RenderClear(renderer);

        ship->printDiagnostics(overlay);
        ship->render(renderer);
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