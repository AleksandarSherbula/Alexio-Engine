#pragma once

#include "Alexio/Window.h"

#include "Alexio/ImGuiLayer.h"

#include "Renderer/RendererAPI.h"
#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"

namespace Alexio
{
	enum class GraphicsAPI
	{
		None,
		OpenGL,
		DirectX11
	};

	class Renderer
	{
	public:
		static void Begin(Window* window);
		static void DrawFrame();
		static void End();

		static void SetAPI(GraphicsAPI api);
		static GraphicsAPI GetGraphicsAPI() { return s_API; }
		inline static RendererAPI* GetAPI() { return sRendererAPI.get(); };
	private:
		static std::shared_ptr<RendererAPI> sRendererAPI;	
		static ImGUI* imgui;
		static GraphicsAPI s_API;
	};
}