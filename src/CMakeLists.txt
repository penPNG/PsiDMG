add_executable(${PROJECT_NAME})
include_directories($ENV{VULKAN_SDK})
target_link_libraries(${PROJECT_NAME} PRIVATE vendor)
target_sources(${PROJECT_NAME} PRIVATE main.cpp "cpu.cpp" "cpu.h" "memory.h" "memory.cpp" "dmg.h" "dmg.cpp")
target_sources(${PROJECT_NAME} PRIVATE "imgui/imgui.cpp" "imgui/imgui.h" "imgui/imgui_demo.cpp" "imgui/imgui_draw.cpp" "imgui/imgui_impl_sdl3.cpp" "imgui/imgui_impl_sdl3.h"
"imgui/imgui_internal.h" "imgui/imgui_tables.cpp" "imgui/imgui_widgets.cpp" "imgui/imstb_rectpack.h" "imgui/imstb_textedit.h" "imgui/imstb_truetype.h" "imgui/imgui_impl_sdlrenderer3.cpp")