#include <cstdarg>
#include <vector>

#include <SDL2/SDL_ttf.h>
#include "DebugOverlay.hpp"

using namespace std;

TTF_Font* Sans;
SDL_Color White = {255, 255, 255};

vector<SDL_Surface*> surfaces;

DebugOverlay::DebugOverlay(void) {
	TTF_Init();
	Sans = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeMono.ttf", 24);

	if (!Sans)
		printf("TTF_OpenFont: %s\n", TTF_GetError());
}

DebugOverlay::~DebugOverlay(void) {
	TTF_CloseFont(Sans);
}

void DebugOverlay::print(const char* fmt, ...) {
	char buffer[512];

	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);

	surfaces.push_back(TTF_RenderText_Solid(Sans, buffer, White));
}

void DebugOverlay::render(SDL_Renderer * renderer) {
	SDL_Rect Message_rect = {0, 0, 0, 0};

	for (vector<SDL_Surface*>::iterator s = surfaces.begin();
                              s != surfaces.end();
                              ++s) {

		SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, *s); //now you can convert it into a texture

		SDL_QueryTexture(Message, NULL, NULL, &Message_rect.w, &Message_rect.h);

		SDL_RenderCopy(renderer, Message, NULL, &Message_rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture

		Message_rect.y += 24;

		SDL_FreeSurface(*s);
		SDL_DestroyTexture(Message);
	}

	surfaces.clear();
}