#include "core/dmg.h"
#include "core/context.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl3.h"
#include "imgui/imgui_impl_sdlrenderer3.h"
#include <stdlib.h>
#include <stdexcept>
#include <stdio.h>
#include <SDL3/SDL.h>
#include <iostream>

int main(char* argv) {
	DMG* dmg = new DMG("../../../Tetris.gb");
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw std::runtime_error(SDL_GetError());
	}
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	Context* context = new Context(1280, 720, dmg);

	bool show_demo_window = true;
	bool show_another_window = false;

	bool done = false;
	while (!done) {
		done = context->update();
	}

	delete(dmg);

	return context->shutdown();
}