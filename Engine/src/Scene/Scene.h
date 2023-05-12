#pragma once

#include "..\..\Engine\dependencies\entt\include\entity\entt.hpp"

namespace Alexio
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();
	
		Entity CreateEntity(const std::string& name = std::string());
		
		entt::registry& Reg() { return mRegistry; }

		void OnUpdate();
		void OnDestroy();

		void OnViewportResize(float width, float height);
	private:
		entt::registry mRegistry;
		float mViewportWidth, mViewportHeight;

		friend class SceneHierarchyPanel;
	};
}