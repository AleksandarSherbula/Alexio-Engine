#pragma once

#include "Alexio/Window.h"

#include "Renderer/RendererAPI.h"

namespace Alexio
{
	class Renderer
	{
	public:
		static void Begin(Window* window);
		static void DrawFrame();
		static void End();

		static void SetAPI(GraphicsAPI api) { sAPI_flag = api; }
		static GraphicsAPI GetAPI() { return sAPI_flag; }
	private:
		static GraphicsAPI sAPI_flag;
		static RendererAPI* sRendererAPI;
	};
}