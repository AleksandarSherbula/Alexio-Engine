#pragma once

#include "Alexio/Utilities.h"
#include "Scene/Scene.h"

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
		Ref<Scene> mContext;
	};
}