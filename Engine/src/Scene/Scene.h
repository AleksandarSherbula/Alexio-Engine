#pragma once

#include "..\..\Engine\dependencies\entt\include\entity\entt.hpp"
#include "Object.h"

namespace Alexio
{
	class Scene
	{
	public:
		Scene();
		~Scene();
	
		Object CreateObject(const std::string& name = std::string());
		
		entt::registry& Reg() { return mRegistry; }

		void OnUpdate();

		void OnResize(float width, float height);
		void OnEvent(Event& e);
	private:
		entt::registry mRegistry;
		Object mCameraObject;
	};
}