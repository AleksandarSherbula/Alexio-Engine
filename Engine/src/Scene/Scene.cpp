#include "aio_pch.h"
#include "Scene.h"
#include "Object.h"

#include "Renderer/Renderer.h"

namespace Alexio
{
	Scene::Scene()
	{
#if ENTT_EXAMPLE_CODE
		entt::entity entity = m_Registry.create();
		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

		m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();


		if (m_Registry.has<TransformComponent>(entity))
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);


		auto view = m_Registry.view<TransformComponent>();
		for (auto entity : view)
		{
			TransformComponent& transform = view.get<TransformComponent>(entity);
		}

		auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
		for (auto entity : group)
		{
			auto& [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
		}
#endif
	}
	
	Scene::~Scene()
	{
	
	}

	Object Scene::CreateEntity(const std::string& name)
	{
		Object object = {mRegistry.create(), this};
		object.AddComponent<TransformComponent>();
		TagComponent& tag = object.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Undefined" : name;
		return object;
	}

	void Scene::OnUpdate()
	{
		auto group = mRegistry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
	
			Renderer::DrawQuad(transform, sprite.Color);
		}
	}
	
	
}


