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
		void DrawEntityNode(Entity entity);
	private:
		Ref<Scene> mContext;
		Entity mSelectionContext;
	};
}