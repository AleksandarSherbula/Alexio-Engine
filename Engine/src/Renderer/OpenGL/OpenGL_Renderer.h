#pragma once

#include "Renderer/RendererAPI.h"


namespace Alexio
{
	class OpenGL_Renderer : public RendererAPI
	{
	public:
		OpenGL_Renderer();
		~OpenGL_Renderer();

		void Initialize() override;
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		void Draw(const Ref<Shader>& shader, const Ref<VertexResources>& vertexResources = nullptr) override;
		void ClearColor(float r, float g, float b, float a) override;
		void SwapBuffer() override;

		inline std::string GetName() const override { return "OpenGL"; };
	private:
		void ImGuiBackendInit() override;
		void ImGuiBackendBegin() override;
		void ImGuiBackendUpdate() override;
		void ImGuiBackendShutDown() override;

	private:
		static OpenGL_Renderer* sInstance;
	private:
	};
}
