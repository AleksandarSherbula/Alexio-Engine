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
		Vector3 Position = { 0.0f, 0.0f, 0.0f };
		Vector3 Rotation = { 0.0f, 0.0f, 0.0f };
		Vector3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const Vector3& position)
			: Position(position) {}

		Mat4x4 GetTransform() const
		{
			glm::mat4x4 rotation = glm::rotate(glm::mat4x4(1.0f), Rotation.x, {1.0f, 0.0f, 0.0f})
				* glm::rotate(glm::mat4x4(1.0f), Rotation.y, { 0.0f, 1.0f, 0.0f })
				* glm::rotate(glm::mat4x4(1.0f), Rotation.z, { 0.0f, 0.0f, 1.0f });

			return glm::translate(glm::mat4x4(1.0f), Position)
				* rotation
				* glm::scale(glm::mat4x4(1.0f), Scale);
		}
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