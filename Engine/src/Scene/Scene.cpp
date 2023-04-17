#include "aio_pch.h"
#include "Object.h"
#include "Scene.h"

#include "Alexio/Timer.h"
#include "Alexio/Engine.h"
#include "Renderer/Renderer.h"

namespace Alexio
{
	Scene::Scene()
	{
		mViewportWidth = 0.0f;
		mViewportHeight = 0.0f;

		mCameraObject = CreateObject("Camera");
		CameraComponent& camera = mCameraObject.AddComponent<CameraComponent>();
		camera.Primary = true;
	}
	
	Scene::~Scene()
	{
	
	}

	Object Scene::CreateObject(const std::string& name)
	{
		Object object = {mRegistry.create(), this};
		object.AddComponent<TransformComponent>();
		TagComponent& tag = object.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Undefined" : name;
		return object;
	}

	void Scene::OnUpdate()
	{
		Mat4x4* mainCameraProjection = nullptr;
		Mat4x4* mainCameraTransform = nullptr;
		{
			auto view = mRegistry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCameraProjection = &camera.Camera.GetProjection();
					mainCameraTransform = &transform.Transform;
					break;
				}
			}
		}

		if (mainCameraProjection)
		{
			Mat4x4 viewProj = *mainCameraProjection * glm::inverse(*mainCameraTransform);

			Renderer::GetCameraBuffer()->SetData(&viewProj, sizeof(Mat4x4));
			Renderer::GetCameraBuffer()->Bind(0);

			auto group = mRegistry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer::DrawQuad(transform, sprite.Color);
			}

			Renderer::Flush();
		}
	}

	void Scene::OnViewportResize(float width, float height)
	{
		mViewportWidth = width;
		mViewportHeight = height;

		auto view = mRegistry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			cameraComponent.Camera.SetViewportSize(width, height);
		}
	}
}


