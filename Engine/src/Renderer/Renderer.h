#pragma once

#include "RendererBackend.h"

#include "Alexio/Utilities.h"

namespace Alexio
{
	class Renderer
	{
	public:
		static void Begin();
		static void Draw(const Ref<Shader>& shader, const Ref<VertexResources>& vertexResources = nullptr);
		static void End();

		static void Clear(float r, float g, float b, float a);
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		inline static void SetVSync(bool vSync) { sRendererBackend->SetVSync(vSync); }
		inline static bool IsVSync() { return sRendererBackend->IsVSync(); }
		
		inline static RendererBackend* GetBackend() { return sRendererBackend.get(); };
		inline static ConstantBuffer* GetCameraBuffer() { return sCameraBuffer.get(); }

		inline static void SetGraphicsAPI(GraphicsAPI api) { sRendererBackend->SetGraphicsAPI(api); }
		inline static GraphicsAPI GetGraphicsAPI() { return sRendererBackend->GetGraphicsAPI(); }
	private:
		static Ref<RendererBackend> sRendererBackend;
		static Ref<ConstantBuffer> sCameraBuffer;
	};
}