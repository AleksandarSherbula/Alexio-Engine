#pragma once

#include "Renderer/RendererAPI.h"

namespace Alexio
{
	class Renderer_OpenGL : public RendererAPI
	{
	public:
		Renderer_OpenGL();

		void Initialize() override;
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		void Draw() override;
		void ClearColor(float r, float g, float b, float a) override;
		void SwapBuffer() override;
	private:
		static Renderer_OpenGL* sInstance;
	};
}
