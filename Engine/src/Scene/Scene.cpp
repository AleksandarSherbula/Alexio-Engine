#include "aio_pch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"

#include "Alexio/Timer.h"
#include "Alexio/Engine.h"
#include "Renderer/Renderer.h"

namespace Alexio
{
	Scene::Scene()
	{
		mViewportWidth = 0.0f;
		mViewportHeight = 0.0f;
	}
	
	Scene::~Scene()
	{
		OnDestroy();
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = {mRegistry.create(), this};
		entity.AddComponent<TransformComponent>();
		TagComponent& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Undefined" : name;
		return entity;
	}

	void Scene::OnUpdate()
	{
		// Update scripts
		{
			mRegistry.view<NativeScriptComponent>().each([=](entt::entity entity, NativeScriptComponent& nsc)
				{
					if (!nsc.Instance)
					{
						nsc.Instance = nsc.InstantiateScript();
						nsc.Instance->mEntity = Entity{entity, this};
						nsc.Instance->OnCreate();
					}

					nsc.Instance->OnUpdate();
				});
		}

		Mat4x4* mainCameraProjection = nullptr;
		Mat4x4* mainCameraTransform = nullptr;
		{
			auto view = mRegistry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCameraProjection = &camera.Camera.GetProjection();
					mainCameraTransform = &transform.GetTransform();
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
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer::DrawQuad(transform.GetTransform(), sprite.Color);
			}

			Renderer::Flush();
		}
	}

	void Scene::OnDestroy()
	{
		mRegistry.view<NativeScriptComponent>().each([=](entt::entity entity, NativeScriptComponent& nsc)
			{
				if (nsc.Instance)
				{
					nsc.Instance->OnDestroy();
					nsc.DestroyScript(&nsc);
				}				
			});
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


