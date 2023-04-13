#include "aio_pch.h"
#include "Object.h"

namespace Alexio
{
	Object::Object(entt::entity handle, Scene* scene)
	{
  		mEntityHandle = handle;
		mScene = scene;
	}
}


