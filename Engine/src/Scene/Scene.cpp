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
		mCameraObject = CreateObject("Camera");
		mCameraObject.AddComponent<CameraComponent>(Engine::Get()->ScreenWidth() / Engine::Get()->ScreenHeight());
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
		CameraComponent& camera = mCameraObject.GetComponent<CameraComponent>();
		camera.Controller.OnUpdate(Timer::DeltaTime());


		auto group = mRegistry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
	
			Renderer::DrawQuad(transform, sprite.Color);
		}
	}

	void Scene::OnResize(float width, float height)
	{
		CameraComponent& camera = mCameraObject.GetComponent<CameraComponent>();
		camera.Controller.UpdateProjection(width, height);
	}

	void Scene::OnEvent(Event& e)
	{
		CameraComponent& camera = mCameraObject.GetComponent<CameraComponent>();
		camera.Controller.OnEvent(e);
	}
}


