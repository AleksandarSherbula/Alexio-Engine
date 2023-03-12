#include "aio_pch.h"
#include "ImGui/ImGuiLayer.h"
#include "Alexio/Engine.h"
#include "Renderer/Renderer.h"

#include <imgui.h>
#include <backends/imgui_impl_dx11.h>

namespace Alexio
{
    void ImGUI::OnAttach()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGuiStyle& style = ImGui::GetStyle();
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        Renderer::GetAPI()->ImGuiBackendInit();
    }

    void ImGUI::Begin()
    {
        Renderer::GetAPI()->ImGuiBackendBegin();
        ImGui::NewFrame();
    }

    void ImGUI::End()
    {
        ImGui::Render();
        Renderer::GetAPI()->ImGuiBackendUpdate();
    }

    void ImGUI::OnDetach()
    {
        Renderer::GetAPI()->ImGuiBackendShutDown();
        ImGui::DestroyContext();
    }

    void ImGUI::OnImGuiRender()
    {
        bool showWindow = true;
        if (showWindow)
            ImGui::ShowDemoWindow(&showWindow);
    }
}