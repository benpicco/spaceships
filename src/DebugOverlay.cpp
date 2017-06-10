#include <cstdarg>
#include <SDL2/SDL_ttf.h>
#include "DebugOverlay.hpp"

TTF_Font* Sans;
SDL_Color White = {255, 255, 255};
SDL_Rect Message_rect = {0, 0, 100, 100};

char buffer[1024];
char* buffer_pos;

DebugOverlay::DebugOverlay(void) {
	TTF_Init();
	Sans = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeMono.ttf", 24);

	if (!Sans)
		printf("TTF_OpenFont: %s\n", TTF_GetError());

	buffer_pos = &buffer[0];
}

DebugOverlay::~DebugOverlay(void) {
	TTF_CloseFont(Sans);
}

void DebugOverlay::print(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	buffer_pos += vsnprintf(buffer_pos, sizeof(buffer) - (buffer_pos - &buffer[0]), fmt, args);
	va_end(args);
}

void DebugOverlay::render(SDL_Renderer * renderer) {
	buffer_pos = &buffer[0];

	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, buffer_pos, White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture

	SDL_QueryTexture(Message, NULL, NULL, &Message_rect.w, &Message_rect.h);

	SDL_RenderCopy(renderer, Message, NULL, &Message_rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture

	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(Message);
}