#include "aio_pch.h"
#include "Alexio/ImGuiLayer.h"
#include "Alexio/Engine.h"
#include "Alexio/Renderer.h"

#include <imgui.h>

namespace Alexio
{
    void ImGUI::OnAttach()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        
        Renderer::GetAPI()->ImGuiBackendInit();

        showWindow = true;
    }

    void ImGUI::Begin()
    {
        Renderer::GetAPI()->ImGuiBackendBegin();
        ImGui::NewFrame();
    }

    void ImGUI::OnUpdate()
    {
        if (showWindow)
            ImGui::ShowDemoWindow(&showWindow);

        ImGui::Render();
        Renderer::GetAPI()->ImGuiBackendDrawData();
    }

    void ImGUI::OnDetach()
    {
        Renderer::GetAPI()->ImGuiBackendShutDown();
        ImGui::DestroyContext();
    }
}