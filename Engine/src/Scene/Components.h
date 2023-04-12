#pragma once

#include "Math/Math.h"

namespace Alexio
{
	struct TransformComponent
	{
		Mat4x4 transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const Mat4x4& transform)
			: transform(transform) {}

		operator Mat4x4& () { return transform; }
		operator const Mat4x4& () const { return transform; }
	};

	struct SpriteRendererComponent
	{
		Vector4 color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const Vector4& color)
			: color(color) {}
	};
}