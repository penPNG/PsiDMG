#include "context.h"
#include <stdexcept>

Context::Context(int _width, int _height) : width(_width), height(_height) {

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw std::runtime_error(getSDLError("Init"));
	}

	window = SDL_CreateWindow("DMG", width, height, SDL_WINDOW_HIDDEN);
	if (window == NULL) {
		throw std::runtime_error(getSDLError("CreateWindow"));
	}

	renderer = SDL_CreateRenderer(window, NULL);
	if (renderer == NULL) {
		throw std::runtime_error(getSDLError("CreateRenderer"));
	}

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR1555, SDL_TEXTUREACCESS_STREAMING, width, height);
	if (texture == NULL) {
		throw std::runtime_error(getSDLError("CreateTexture"));
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	SDL_LockTexture(texture, NULL, &texturePixels, &texturePitch);
}