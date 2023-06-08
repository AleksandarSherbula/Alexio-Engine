#include "aio_pch.h"
#include "SceneHierarchyPanel.h"
#include "Scene/Components.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Alexio
{
	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValues = 0.0f, float columnWidth = 100.0f);

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
			//if (isOpened)
			//{
			//	ImGui::TreePop();
			//}
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
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				TransformComponent& transformComponent = entity.GetComponent<TransformComponent>();

				DrawVec3Control("Position", transformComponent.Position);
				Vector3 rotationInDegrees = glm::degrees(transformComponent.Rotation);
				DrawVec3Control("Rotation", rotationInDegrees);
				transformComponent.Rotation = glm::radians(rotationInDegrees);
				DrawVec3Control("Scale", transformComponent.Scale, 1.0f);

				ImGui::TreePop();
			}
			
		}

		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& camera = entity.GetComponent<CameraComponent>();
				ImGui::Checkbox("Primary", &camera.Primary);
				ImGui::Text("Orthographic size:");
				if (ImGui::DragFloat(" ", &camera.OrthographicSize))
					camera.Camera.SetOrthographicSize(camera.OrthographicSize);

				ImGui::TreePop();
			}
			
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "SpriteRenderer"))
			{
				SpriteRendererComponent& sprRenderer = entity.GetComponent<SpriteRendererComponent>();

				ImGui::ColorEdit4("Color", glm::value_ptr(sprRenderer.Color));

				ImGui::TreePop();
			}
		}
	}


	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValues, float columnWidth)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());

		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		if (ImGui::Button("X", buttonSize))
			values.x = resetValues;

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f);
		ImGui::PopItemWidth();
		ImGui::SameLine();


		if (ImGui::Button("Y", buttonSize))
			values.y = resetValues;

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f);
		ImGui::PopItemWidth();
		ImGui::SameLine();


		if (ImGui::Button("Z", buttonSize))
			values.z = resetValues;

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f);
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}
}