#pragma once

#include "RendererBackend.h"
#include "RenderData.h"

#include "Alexio/Utilities.h"
#include "Math/Math.h"

namespace Alexio
{
	class Renderer
	{
	public:
		static void Init();

		static void Clear(float r, float g, float b, float a);

		inline static void SetVSync(bool vSync) { sRendererBackend->SetVSync(vSync); }
		inline static bool IsVSync() { return sRendererBackend->IsVSync(); }

		inline static RendererBackend* GetBackend() { return sRendererBackend.get(); }
		inline static ConstantBuffer* GetCameraBuffer() { return sCameraBuffer.get(); }

		inline static void SetGraphicsAPI(GraphicsAPI api) { sRendererBackend->SetGraphicsAPI(api); }
		inline static GraphicsAPI GetGraphicsAPI() { return sRendererBackend->GetGraphicsAPI(); }

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		static void Draw(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f), float angle = 0.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f), float angle = 0.0f);

		static void DrawSprite(const Ref<Texture>& texture, const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f), float angle = 0.0f);
		static void DrawSprite(const Ref<Texture>& texture, const glm::vec3& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f), float angle = 0.0f);

		static void DrawCircle(const glm::vec2& position, const glm::vec4& color = glm::vec4(1.0f), float radius = 1.0f, float thickness = 1.0f, float fade = 0.05f);
		static void DrawCircle(const glm::vec3& position, const glm::vec4& color = glm::vec4(1.0f), float radius = 1.0f, float thickness = 1.0f, float fade = 0.05f);
	private:
		static Ref<RendererBackend> sRendererBackend;
		static Ref<ConstantBuffer> sCameraBuffer;

		static Ref<QuadRenderer> sQuadRenderer;
		static Ref<CircleRenderer> sCircleRenderer;
	};
}