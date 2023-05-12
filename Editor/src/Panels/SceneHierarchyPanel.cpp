#include "aio_pch.h"
#include "SceneHierarchyPanel.h"
#include "Scene/Components.h"

#include <imgui/imgui.h>

namespace Alexio
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		mContext = context;
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		mContext = context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		mContext->mRegistry.each([&](entt::entity entityID)
			{
				TagComponent& tagComponent = mContext->mRegistry.get<TagComponent>(entityID);
				ImGui::Text("%s", tagComponent.Tag.c_str());
			});

		ImGui::End();
	}
}


