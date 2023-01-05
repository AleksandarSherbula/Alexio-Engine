#pragma once

#include "Alexio/Window.h"
#include "Alexio/ImGuiLayer.h"
#include "Renderer/RendererAPI.h"

namespace Alexio
{
	class Renderer
	{
	public:
		static void Begin(Window* window);
		static void DrawFrame();
		static void End();

		inline static void SetAPIflag(GraphicsAPI api) { sAPI_flag = api; }
		inline static GraphicsAPI GetAPIflag() { return sAPI_flag; }
		inline static RendererAPI* GetAPI() { return sRendererAPI; };
	private:
		static GraphicsAPI sAPI_flag;
		static RendererAPI* sRendererAPI;	
		static ImGUI* imgui;
	};
}