#pragma once

#include "Alexio/Window.h"
#include "Alexio/ImGuiLayer.h"
#include "Renderer/RendererAPI.h"

#include "Renderer/OpenGL_Renderer.h"
#ifdef AIO_PLATFORM_WINDOWS
	#include "Renderer/DirectX11_Renderer.h"
#endif

namespace Alexio
{
	class Renderer
	{
	public:
		static void Begin(Window* window);
		static void DrawFrame();
		static void End();

		inline static void SetAPIflag(GraphicsAPIflag api) { sAPI_flag = api; }
		inline static GraphicsAPIflag GetAPIflag() { return sAPI_flag; }
		inline static RendererAPI* GetAPI() { return sRendererAPI; };
	private:
		static GraphicsAPIflag sAPI_flag;
		static RendererAPI* sRendererAPI;	
		static ImGUI* imgui;
	};
}