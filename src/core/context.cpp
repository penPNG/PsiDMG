#include "context.h"
#include <stdexcept>

Context::Context(int _width, int _height, DMG* _dmg) : width(_width), height(_height), dmg(_dmg), io(ImGui::GetIO()) {

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

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

	ImGui::StyleColorsDark();

	ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer3_Init(renderer);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	SDL_LockTexture(texture, NULL, &texturePixels, &texturePitch);
}

void Context::update() {
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	ImGui::ShowDemoWindow(&showdemo);
	showDMGDebugger(dmg);

	SDL_UnlockTexture(texture);
	SDL_RenderClear(renderer);
	SDL_RenderTexture(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
	SDL_LockTexture(texture, NULL, &texturePixels, &texturePitch);
}


void Context::showDMGDebugger(DMG* dmg) {
	using namespace ImGui;
	SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
	ImGuiInputTextFlags iflags = ImGuiInputTextFlags_ReadOnly;
	ImGuiTableFlags tflags = ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV;

	byte v = 0x34;

	Begin("DMG Debugger");
	{
		SeparatorText("Registers");
		Text("A F: %02X %02X", dmg->cpu->get8(A), dmg->cpu->get8(F)); SameLine(0, 20);
		Text("B C: %02X %02X", dmg->cpu->get8(B), dmg->cpu->get8(C)); Dummy(ImVec2(0, 10));
		Text("D E: %02X %02X", dmg->cpu->get8(D), dmg->cpu->get8(E)); SameLine(0, 20);
		Text("H L: %02X %02X", dmg->cpu->get8(H), dmg->cpu->get8(L)); Dummy(ImVec2(0, 10));
		Text("SP: %04X  ", dmg->cpu->get16(SP)); SameLine(0, 20);
		Text("PC: %04X", dmg->cpu->PC);
	}
	Dummy(ImVec2(200, 18)); SameLine();
	{
		Button("Pause", ImVec2(70,0));
		Text("Current Instruction"); SameLine();
		PushItemWidth(30);
		InputScalar("##Instruction", ImGuiDataType_U8, &dmg->ram.ram[dmg->cpu->PC], NULL, NULL, "%02X", iflags);
		SameLine(0, 37); Button("Resume", ImVec2(70,0));
		Text("Argument(s)"); SameLine();
		InputScalar("##Arguments", ImGuiDataType_U8, &v, NULL, NULL, "%02X", iflags);
		PopItemWidth(); SameLine(0, 26);
		Button("Execute"); SetItemTooltip("Execute the current instruction without\n"
			"advancing the program counter");
		SameLine(0, 10);
		Button("Step", ImVec2(70,0));
	}
	{
		/*SeparatorText("Memory");
		if(BeginTable("##Ram", 16, ))*/
	}
	End();
}