#pragma once

#include <SDL.h>

class DebugOverlay {
	public:
		DebugOverlay(void);
		~DebugOverlay(void);

		void render(SDL_Renderer * renderer);
		void print(const char* fmt, ...);
};