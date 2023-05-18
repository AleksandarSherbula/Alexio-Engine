#pragma once

#include "Alexio/Utilities.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"

namespace Alexio
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> mContext;
		Entity mSelectionContext;
	};
}