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
		if (ImGui::Begin("Scene Hierarchy"))
		{
			mContext->mRegistry.each([&](entt::entity entityID)
				{
					Entity entity{ entityID, mContext.get() };

					DrawEntityNode(entity);
				});

		}
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		TagComponent& tagComponent = entity.GetComponent<TagComponent>();
		ImGuiTreeNodeFlags flags = ((mSelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool isOpened = ImGui::TreeNodeEx((void*)(uint64_t)entity.GetID(), flags, tagComponent.Tag.c_str());

		if (ImGui::IsItemClicked())
		{
			mSelectionContext = entity;
		}

		if (isOpened)
		{
			// Test code
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool isOpened = ImGui::TreeNodeEx((void*)9816582, flags, tagComponent.Tag.c_str());
			if (isOpened)
			{
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
	}
}