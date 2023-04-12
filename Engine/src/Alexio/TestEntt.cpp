#include "aio_pch.h"
#include "TestEntt.h"
#include "Math/Math.h"

#include "entity/entt.hpp"

Alexio::TestEntt::TestEntt()
{
	entt::registry registry;

	struct Transform
	{
		Vector2 position;
		Vector2 scale;
		float rotation;
	};

	struct Physics
	{
		Vector2 velocity;
		bool useGravity;
	};

	struct Audio
	{
		std::string audioFile;
		bool IsLooped;
	};

	entt::entity ent = registry.create();
	registry.emplace<Transform>(ent, Vector2(0.5f, 0.5f), Vector2(1.0f, 1.0f), 3.14f);


	auto view = registry.view<const Transform>();
}
