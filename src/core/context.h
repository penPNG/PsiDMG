#pragma once
#include "../core/ds.h"
#include "../core/dmg.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_sdl3.h"
#include "../imgui/imgui_impl_sdlrenderer3.h"
#include "SDL3/SDL.h"
#include <string>

class Context {
public:
	Context(int _width, int _height);
	void update();

//private:
	static void showDMGDebugger(DMG*);

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;

	ImGuiIO& io;

	const int width;
	const int height;

	bool showdemo = true;
	bool running = true;

	int texturePitch;
	void* texturePixels;

	static const std::string getSDLError(const std::string& errorFunction) {
		return "SDL_" + errorFunction + " Error: " + SDL_GetError();
	}
};