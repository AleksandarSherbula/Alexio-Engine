#pragma once

#include "..\..\Engine\dependencies\entt\include\entity\entt.hpp"
#include "Components.h"

namespace Alexio
{
	class Object;

	class Scene
	{
	public:
		Scene();
		~Scene();
	
		Object CreateEntity(const std::string& name = std::string());
		
		entt::registry& Reg() { return mRegistry; }

		void OnUpdate();
	private:
		entt::registry mRegistry;
	};
}