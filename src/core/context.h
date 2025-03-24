#pragma once
#include "../core/ds.h"
#include "SDL3/SDL.h"
#include <string>

class Context {
public:
	Context(int _width, int _height);
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;

	const int width;
	const int height;

	int texturePitch;
	void* texturePixels;

	static const std::string getSDLError(const std::string& errorFunction) {
		return "SDL_" + errorFunction + " Error: " + SDL_GetError();
	}
};