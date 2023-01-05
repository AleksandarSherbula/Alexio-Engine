#include "aio_pch.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <backends/imgui_impl_glfw.cpp>
#include <backends/imgui_impl_opengl3.cpp>

#ifdef AIO_PLATFORM_WINDOWS
#include <backends/imgui_impl_win32.cpp>
#include <backends/imgui_impl_dx11.cpp>
#endif // AIO_PLATFORM_WINDOWS
