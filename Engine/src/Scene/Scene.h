#pragma once

#include "../../dependecies/entt/include/entity/entt.hpp"
#include "Components.h"

namespace Alexio
{
	class Scene
	{
	public:
		Scene();
		~Scene();
	
		entt::entity CreateEntity();
		inline entt::registry& Reg() { return mRegistry; }
	
		void OnUpdate();	
	private:
		entt::registry mRegistry;
	};
}