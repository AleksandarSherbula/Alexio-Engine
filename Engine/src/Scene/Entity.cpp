#include "aio_pch.h"
#include "Entity.h"

namespace Alexio
{
	Entity::Entity(entt::entity handle, Scene* scene)
	{
  		mEntityHandle = handle;
		mScene = scene;
	}
}


