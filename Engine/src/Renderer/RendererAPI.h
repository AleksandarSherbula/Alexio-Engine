#pragma once

#include "Alexio/Window.h"

#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"

namespace Alexio
{
	class RendererAPI
	{
	public:
		virtual ~RendererAPI() = default;

		static std::shared_ptr<RendererAPI> Create();

		virtual void Initialize() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void Draw(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexResources>& vertexResources = nullptr) = 0;
		virtual void ClearColor(float r, float g, float b, float a) = 0;
		virtual void SwapBuffer() = 0;

		virtual void ImGuiBackendInit() = 0;
		virtual void ImGuiBackendBegin() = 0;
		virtual void ImGuiBackendUpdate() = 0;
		virtual void ImGuiBackendShutDown() = 0;

		virtual std::string GetName() const = 0;

		inline void SetWindow(Window* window) { mWindow = window; }
		inline void SetVSync(bool vSync) { mVSync = vSync; }
		inline bool IsVSync() const { return mVSync; }

	protected:
		bool mVSync;
		Window* mWindow;
	};
}