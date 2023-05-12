#pragma once

#include "..\..\Engine\dependencies\entt\include\entity\entt.hpp"
#include "Components.h"
#include "Scene.h"

namespace Alexio
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			AIO_ASSERT(!HasComponent<T>(), "Entity already has component");
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
			AIO_ASSERT(HasComponent<T>(), "Entity does not have component");
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

	class EntityScript
	{
	public:
		virtual ~EntityScript() {}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			return mEntity.AddComponent<T>(args);
		}

		template<typename T>
		T& GetComponent()
		{
			return mEntity.GetComponent<T>();
		}

		template<typename T>
		bool HasComponent()
		{
			return mEntity.HasComponent<T>();
		}

		template<typename T>
		void RemoveComponent()
		{
			mEntity.RemoveComponent<T>();
		}

		virtual void OnCreate() {}
		virtual void OnUpdate() {}
		virtual void OnDestroy() {}
	private:
		Entity mEntity;
		friend class Scene;
	};
}