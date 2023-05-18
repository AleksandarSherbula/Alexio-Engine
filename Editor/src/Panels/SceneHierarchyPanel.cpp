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

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				mSelectionContext = {};

			ImGui::End();
		}

		if (ImGui::Begin("Inspector"))
		{
			if (mSelectionContext)
			{
				DrawComponents(mSelectionContext);
			}

			ImGui::End();
		}
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		TagComponent& tagComponent = entity.GetComponent<TagComponent>();
		ImGuiTreeNodeFlags flags = ((mSelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool isOpened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tagComponent.Tag.c_str());

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

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			TagComponent& tagComponent = entity.GetComponent<TagComponent>();

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tagComponent.Tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tagComponent.Tag = std::string(buffer);
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform Component"))
			{
				TransformComponent& transformComponent = entity.GetComponent<TransformComponent>();

				ImGui::Text("Transform:");
				ImGui::DragFloat2("Position",
					glm::value_ptr(transformComponent.Transform[3]), 0.05f);

				ImGui::TreePop();
			}
			
		}

		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera Component"))
			{
				auto& camera = entity.GetComponent<CameraComponent>();
				ImGui::Text("Orthographic size:");
				if (ImGui::DragFloat(" ", &camera.OrthographicSize))
					camera.Camera.SetOrthographicSize(camera.OrthographicSize);

				ImGui::TreePop();
			}
			
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "SpriteRenderer Component"))
			{
				SpriteRendererComponent& sprRenderer = entity.GetComponent<SpriteRendererComponent>();

				ImGui::ColorEdit4("Color", glm::value_ptr(sprRenderer.Color));

				ImGui::TreePop();
			}
		}
	}
}