#include "context.h"
#include <stdexcept>

Context::Context(int _width, int _height, DMG* _dmg) : width(_width), height(_height), io(ImGui::GetIO()), dmg(*_dmg) {

	window = SDL_CreateWindow("DMG", width, height, SDL_WINDOW_INPUT_FOCUS);
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

	io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

	ImGui::StyleColorsDark();

	ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer3_Init(renderer);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	SDL_LockTexture(texture, NULL, &texturePixels, &texturePitch);
}

bool Context::input() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		ImGui_ImplSDL3_ProcessEvent(&event);
		if (event.type == SDL_EVENT_QUIT) {
			return true;
		}
		if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window)) {
			return true;
		}
	}
	return false;
}

bool Context::update() {
	bool done = input();

	const auto start_time = std::chrono::steady_clock::now();

	int tCycles = (cycles + oCycles);
	oCycles = dmg.hardwareCycle(tCycles);

	auto frame_time = duration_cast<std::chrono::microseconds > (std::chrono::steady_clock::now() - start_time);
	max_frame_time = std::max(max_frame_time, frame_time);
	avg_frame_time += frame_time;
	if (++frames == 60) {
		max_frame_time = std::chrono::microseconds(0);
		avg_frame_time = std::chrono::microseconds(0);
		frames = 0;
	}

	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	ImGui::ShowDemoWindow(&showdemo);
	showDMGDebugger();

	ImGui::Render();
	SDL_SetRenderDrawColorFloat(renderer, clear_color.x, clear_color.y, clear_color.z, clear_color.w);

	//SDL_UnlockTexture(texture);
	SDL_RenderClear(renderer);
	//SDL_RenderTexture(renderer, texture, NULL, NULL);
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
	SDL_RenderPresent(renderer);
	//SDL_LockTexture(texture, NULL, &texturePixels, &texturePitch);
	return done;
}

int Context::shutdown() {
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}


// Windows
void Context::showDMGDebugger() {
	using namespace ImGui;
	SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
	ImGuiInputTextFlags iflags = ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_CharsUppercase;
	ImGuiTableFlags tflags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_HighlightHoveredColumn;

	byte v = 0x34;
	static bool lockMem = true;

	Begin("DMG Debugger");
	{
		SeparatorText("Registers");
		Text("A F: %02X %02X", dmg.cpu->get8(A), dmg.cpu->get8(F)); SameLine(0, 20);
		Text("B C: %02X %02X", dmg.cpu->get8(B), dmg.cpu->get8(C)); Dummy(ImVec2(0, 10));
		Text("D E: %02X %02X", dmg.cpu->get8(D), dmg.cpu->get8(E)); SameLine(0, 20);
		Text("H L: %02X %02X", dmg.cpu->get8(H), dmg.cpu->get8(L)); Dummy(ImVec2(0, 10));
		Text("SP: %04X  ", dmg.cpu->get16(SP)); SameLine(0, 20);
		PushItemWidth(40);
		Text("PC:"); SameLine(); InputScalar("##PCManip", ImGuiDataType_U16, &dmg.cpu->PC, NULL, NULL, "%04X", ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase);
		PopItemWidth();
	}
	Dummy(ImVec2(200, 18)); SameLine();
	{
		Button("Pause", ImVec2(70,0));
		Text("Current Instruction"); SameLine();
		PushItemWidth(30);
		InputScalar("##Instruction", ImGuiDataType_U8, &dmg.ram.ram[dmg.cpu->PC], NULL, NULL, "%02X", iflags);
		SameLine(0, 37); Button("Resume", ImVec2(70,0));
		SameLine(0, 93); Checkbox("Lock Memory", &lockMem);
		Text("Argument(s)"); SameLine();
		InputScalar("##Arguments", ImGuiDataType_U8, &v, NULL, NULL, "%02X", iflags);
		PopItemWidth(); SameLine(0, 26);
		Button("Execute"); SetItemTooltip("Execute the current instruction without\n"
			"advancing the program counter");
		SameLine(0, 10);
		if (Button("Step", ImVec2(70, 0))) {
			dmg.cpu->exec(dmg.ram.readMem(dmg.cpu->PC++));
		}
	}
	SameLine(0, 10);
	{
		static int jumpAddr = 0x100;
		static bool lockScroll = false;
		static bool toScroll = false;
		PushItemWidth(75);
		if (lockScroll) {
			InputScalar("##PCJump", ImGuiDataType_U16, &dmg.cpu->PC, NULL, NULL, "%04X", ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase);
			jumpAddr = dmg.cpu->PC;
		}
		else {
			InputScalar("##JumpAddr", ImGuiDataType_U16, &jumpAddr, NULL, NULL, "%04X", ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase);
			if (IsItemDeactivatedAfterEdit()) {
				toScroll = true;
			}
		}
		SetItemTooltip("Jump to Address"); SameLine();
		Checkbox("Lock to PC", &lockScroll);
		if (IsItemDeactivatedAfterEdit()) {
			toScroll = true;
		}

		SeparatorText("Memory");
		if (BeginTable("##Ram", 17, tflags)) {
			TableSetupScrollFreeze(0, 1);
			TableSetupColumn("Addr", ImGuiTableColumnFlags_WidthFixed, 0);
			TableSetupColumn("X0", ImGuiTableColumnFlags_None, 0);
			TableSetupColumn("X1", ImGuiTableColumnFlags_None, 0);
			TableSetupColumn("X2", ImGuiTableColumnFlags_None, 0);
			TableSetupColumn("X3", ImGuiTableColumnFlags_None, 0);
			TableSetupColumn("X4", ImGuiTableColumnFlags_None, 0);
			TableSetupColumn("X5", ImGuiTableColumnFlags_None, 0);
			TableSetupColumn("X6", ImGuiTableColumnFlags_None, 0);
			TableSetupColumn("X7", ImGuiTableColumnFlags_None, 0);
			TableSetupColumn("X8", ImGuiTableColumnFlags_None, 0);
			TableSetupColumn("X9", ImGuiTableColumnFlags_None, 0);
			TableSetupColumn("XA", ImGuiTableColumnFlags_None, 0);
			TableSetupColumn("XB", ImGuiTableColumnFlags_None, 0);
			TableSetupColumn("XC", ImGuiTableColumnFlags_None, 0);
			TableSetupColumn("XD", ImGuiTableColumnFlags_None, 0);
			TableSetupColumn("XE", ImGuiTableColumnFlags_None, 0);
			TableSetupColumn("XF", ImGuiTableColumnFlags_None, 0);
			TableHeadersRow();

			int addr = 0;
			ImGuiListClipper clipper;
			clipper.Begin(0x1000);
			while (clipper.Step()) {
				for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++) {
					TableNextRow();
					if (lockScroll) {
						SetScrollFromPosY(GetCursorStartPos().y + dmg.cpu->PC, 0.25f);
					}
					else {
						if (toScroll) {
							SetScrollFromPosY(GetCursorStartPos().y + jumpAddr, 0.25f);
							toScroll = false;
						}
					}
					for (int column = 0; column < 17; column++) {
						TableSetColumnIndex(column);
						if (lockScroll) {
							if (row == (dmg.cpu->PC / 16)) {
								if (column == (dmg.cpu->PC % 16) + 1) {
									TableSetBgColor(ImGuiTableBgTarget_CellBg, GetColorU32(ImVec4(0.26f, 0.59f, 0.98f, 1.0f)));
								}
								else {
									TableSetBgColor(ImGuiTableBgTarget_CellBg, GetColorU32(ImVec4(0.15f, 0.29f, 0.45f, 1.0f)));
								}
							}
						}
						else {
							if (row == (jumpAddr / 16)) {
								if (column == (jumpAddr % 16) + 1) {
									TableSetBgColor(ImGuiTableBgTarget_CellBg, GetColorU32(ImVec4(0.26f, 0.59f, 0.98f, 1.0f)));
								}
								else {
									TableSetBgColor(ImGuiTableBgTarget_CellBg, GetColorU32(ImVec4(0.15f, 0.29f, 0.45f, 1.0f)));
								}
							}
						}
						if (column == 0) {
							Text("%03XX", row + column);
						}
						else {
							addr = row * 16 + column - 1;
							if (lockMem) {
								Text("%02X", dmg.ram.readMem(addr));
								//SDL_Log("0x%04X", addr);
							}
							else {
								PushID(addr);
								if (addr < 0x8000) {
									// TODO: ROM Area
									InputScalar("##", ImGuiDataType_U8, &dmg.ram.rom[addr], NULL, NULL, "%02X");
								}
								else if (addr < 0xA000) {
									// Video Ram
									InputScalar("##", ImGuiDataType_U8, &dmg.ram.vram[addr - 0x8000], NULL, NULL, "%02X");
								}
								else if (addr < 0xFF00) {
									if (addr < 0xC000) {
										// External Ram
										InputScalar("##", ImGuiDataType_U8, &dmg.ram.ext_ram[addr - 0xA000], NULL, NULL, "%02X");
									}
									else if (addr < 0xE000) {
										// Work Ram
										InputScalar("##", ImGuiDataType_U8, &dmg.ram.wram[addr - 0xC000], NULL, NULL, "%02X");
									}
									else if (addr < 0xFE00) {
										// Echo of Work Ram
										InputScalar("##", ImGuiDataType_U8, &dmg.ram.wram[addr - 0xE000], NULL, NULL, "%02X");
									}
									else if (addr < 0xFEA0) {
										// OAM
										InputScalar("##", ImGuiDataType_U8, &dmg.ram.oam[addr-0xFE00], NULL, NULL, "%02X");
									}
								}
								else if (addr < 0xFF80) {
									// IO Registers
									InputScalar("##", ImGuiDataType_U8, &dmg.ram.io[addr&0xFF], NULL, NULL, "%02X");
								}
								else if (addr < 0xFFFF) {
									// High Ram
									InputScalar("##", ImGuiDataType_U8, &dmg.ram.hram[addr - 0xFF80], NULL, NULL, "%02X");
								}
								else {
									InputScalar("##", ImGuiDataType_U8, &dmg.ram.ram[addr], NULL, NULL, "%02X");
								}
								//InputScalar("##", ImGuiDataType_U8, &dmg.ram.ram[addr], NULL, NULL, "%02X");
								PopID();
							}
						}
					}
				}
			}
			EndTable();
		}

	}
	End();
}