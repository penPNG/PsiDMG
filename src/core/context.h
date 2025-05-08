#pragma once
#include "ds.h"
#include "dmg.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_sdl3.h"
#include "../imgui/imgui_impl_sdlrenderer3.h"
#include "SDL3/SDL.h"
#include <string>

class Context {
public:
	Context(int _width, int _height, DMG*);
	bool update();
	bool input();

	int shutdown();

//private:
	void showDMGDebugger();

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;

	ImGuiIO& io;
	DMG& dmg;

	const int width;
	const int height;

	bool showdemo = true;
	bool running = true;

	int texturePitch;
	void* texturePixels;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	static const std::string getSDLError(const std::string& errorFunction) {
		return "SDL_" + errorFunction + " Error: " + SDL_GetError();
	}
};