#pragma once

#include "..\..\Engine\dependencies\entt\include\entity\entt.hpp"
#include "Components.h"

namespace Alexio
{
	class Scene;

	class Object
	{
	public:
		Object() = default;
		Object(entt::entity handle, Scene* scene);

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			AIO_ASSERT(!HasComponent<T>(), "Object already has component");
			return mScene->Reg().emplace<T>(mEntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		bool HasComponent()
		{
			return mScene->Reg().all_of<T>(mEntityHandle);
		}

		template<typename T>
		T& GetComponent()
		{
			AIO_ASSERT(HasComponent<T>(), "Object does not have component");
			return mScene->Reg().get<T>(mEntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			mScene->Reg().remove<T>(mEntityHandle);
		}

		operator bool() const { return mEntityHandle != entt::null; }
	private:
		entt::entity mEntityHandle = entt::null;
		Scene* mScene = nullptr;
	};
}