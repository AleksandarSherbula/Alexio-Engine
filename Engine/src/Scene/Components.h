#pragma once

#include "Math/Math.h"
#include "SceneCamera.h"
#include "Renderer/CameraController.h"

namespace Alexio
{
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		Mat4x4 Transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const Mat4x4& transform)
			: Transform(transform) {}

		operator Mat4x4& () { return Transform; }
		operator const Mat4x4& () const { return Transform; }
	};

	struct SpriteRendererComponent
	{
		Vector4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const Vector4& color)
			: Color(color) {}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};
}