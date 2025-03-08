add_executable(${PROJECT_NAME})
include_directories($ENV{VULKAN_SDK})
target_link_libraries(${PROJECT_NAME} PRIVATE vendor)
target_sources(${PROJECT_NAME} PRIVATE main.cpp "cpu.cpp" "cpu.h" "memory.h" "memory.cpp" "dmg.h" "dmg.cpp")
target_sources(${PROJECT_NAME} PRIVATE imgui.cpp imgui.h imgui_demo.cpp imgui_draw.cpp imgui_impl_sdl3.cpp imgui_impl_sdl3.h
imgui_internal.h imgui_tables.cpp imgui_widgets.cpp imstb_rectpack.h imstb_textedit.h imstb_truetype.h imgui_impl_sdlrenderer3.cpp)