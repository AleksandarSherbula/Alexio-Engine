#pragma once

#include "Alexio/Window.h"

#include "Alexio/ImGuiLayer.h"

#include "Renderer/RendererAPI.h"
#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"

namespace Alexio
{
	class Renderer
	{
	public:
		static void Begin(Window* window);
		static void Draw(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexResources>& vertexResources = nullptr);
		static void End();

		static void ClearColor(float r, float g, float b, float a);
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void SwapBuffer();

		static void SetGraphicsAPI(GraphicsAPI api);
		inline static GraphicsAPI GetGraphicsAPI() { return s_API; }
		inline static RendererAPI* GetAPI() { return sRendererAPI.get(); };
	public:
		static ImGUI* imgui;
	private:
		static std::shared_ptr<RendererAPI> sRendererAPI;
		static GraphicsAPI s_API;
	};
}