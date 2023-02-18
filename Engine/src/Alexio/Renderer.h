#pragma once

#include "Alexio/Window.h"

#include "Alexio/ImGuiLayer.h"

#include "Renderer/RendererAPI.h"
#include "Renderer/Buffer.h"

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
		inline static RendererAPI* GetAPI() { return sRendererAPI.get(); };
	private:
		static GraphicsAPIflag sAPI_flag;
		static std::shared_ptr<RendererAPI> sRendererAPI;	
		static ImGUI* imgui;
	};
}