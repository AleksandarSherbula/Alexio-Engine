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

		static void StartBatches();

		static void Clear(float r, float g, float b, float a);

		inline static void SetVSync(bool vSync) { sBackend->SetVSync(vSync); }
		inline static bool IsVSync() { return sBackend->IsVSync(); }

		inline static Ref<RendererBackend>& GetBackend() { return sBackend; }
		inline static Ref<ConstantBuffer>& GetCameraBuffer() { return sCameraBuffer; }

		inline static void SetGraphicsAPI(GraphicsAPI api) { sBackend->SetGraphicsAPI(api); }
		inline static GraphicsAPI GetGraphicsAPI() { return sBackend->GetGraphicsAPI(); }

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		static void DrawLine(const Vector2& p0, const Vector2& p1, const Vector4& color = Vector4(1.0f));
		static void DrawLine(const Vector3& p0, const Vector3& p1, const Vector4& color = Vector4(1.0f));

		static void DrawRect(const Vector2& position, const Vector2& size, const Vector4& color = Vector4(1.0f));
		static void DrawRect(const Vector3& position, const Vector2& size, const Vector4& color = Vector4(1.0f));

		static void DrawQuad(const Vector2& position, const Vector2& size, const Vector4& color = Vector4(1.0f));
		static void DrawQuad(const Vector3& position, const Vector2& size, const Vector4& color = Vector4(1.0f));

		static void DrawQuad(const Mat4x4& transform, const Vector4& color);

		static void DrawRotatedQuad(const Vector2& position, const Vector2& size, const Vector4& color = Vector4(1.0f), float angle = 0.0f);
		static void DrawRotatedQuad(const Vector3& position, const Vector2& size, const Vector4& color = Vector4(1.0f), float angle = 0.0f);

		static void DrawSprite(const Ref<Texture>& texture, const Vector2& position, const Vector2& size, const Vector4& color = Vector4(1.0f));
		static void DrawSprite(const Ref<Texture>& texture, const Vector3& position, const Vector2& size, const Vector4& color = Vector4(1.0f));

		static void DrawPartialSprite(const Ref<Texture>& texture, const Vector2& position, const Vector2& size, const Vector2& tileOffset, const Vector2& tileSize, const Vector4& colorTint = Vector4(1.0f));
		static void DrawPartialSprite(const Ref<Texture>& texture, const Vector3& position, const Vector2& size, const Vector2& tileOffset, const Vector2& tileSize, const Vector4& colorTint = Vector4(1.0f));

		static void DrawRotatedSprite(const Ref<Texture>& texture, const Vector2& position, const Vector2& size, const Vector4& color = Vector4(1.0f), float angle = 0.0f);
		static void DrawRotatedSprite(const Ref<Texture>& texture, const Vector3& position, const Vector2& size, const Vector4& color = Vector4(1.0f), float angle = 0.0f);

		static void DrawCircle(const Vector2& position, const Vector4& color = Vector4(1.0f), float radius = 0.5f, float thickness = 1.0f, float fade = 0.005f);
		static void DrawCircle(const Vector3& position, const Vector4& color = Vector4(1.0f), float radius = 0.5f, float thickness = 1.0f, float fade = 0.005f);		

		static void Flush();

		static void Draw(uint32_t vertexCount);
		static void DrawIndexed(uint32_t indexCount);

		static void End();
	public:
		struct Statistics
		{
			uint32_t Lines;
			uint32_t DrawLine;

			uint32_t Quads;
			uint32_t DrawQuad;

			uint32_t Circles;
			uint32_t DrawCircle;

			inline void Reset()
			{
				Lines = 0;
				Quads = 0;
				Circles = 0;
			}
		};

		static Statistics Stats;
	private:	
		static Ref<RendererBackend> sBackend;
		static Ref<ConstantBuffer> sCameraBuffer;

		static Ref<LineRenderer> sLineRenderer;
	};
}