#pragma once

#include "Math/Math.h"
#include "SceneCamera.h"

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
		float OrthographicSize = 10.0f;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	class EntityScript;

	struct NativeScriptComponent
	{
		EntityScript* Instance = nullptr;

		EntityScript*(*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<EntityScript*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};
}