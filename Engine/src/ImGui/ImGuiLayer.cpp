#include "aio_pch.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer/Renderer.h"
#include "Alexio/Random.h"

#include "Alexio/Timer.h"

namespace Alexio
{
    void ImGuiLayer::OnStart()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGuiStyle& style = ImGui::GetStyle();
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        Renderer::GetBackend()->ImGuiBackendInit();
    }

    void ImGuiLayer::Begin()
    {
        Renderer::GetBackend()->ImGuiBackendBegin();
    }

    void ImGuiLayer::End()
    {
        Renderer::GetBackend()->ImGuiBackendUpdate();
    }

    void ImGuiLayer::OnDetach()
    {
        Renderer::GetBackend()->ImGuiBackendShutDown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::OnImGuiRender()
    {
    }

    void ImGuiLayer::OnEvent(Event& e)
    {
        // TO DO: Go back to this after you setup the SceneCamera or EditorCamera.
        // Currently, this SHOULD block anything you want to do on OnEvent function
        // if it's key or mouse event. If these are events being dispatched, just set
        // them as being handled and skip. However, this does not block the Input Polling
        // because it is not part of the OnEvent functionality.

        // Possible consideration: take it to the origin of where events are being spawn
        // (GLFW_Window events or Win32_Window events).

        if (mBlockEvents)
        {
            ImGuiIO& io = ImGui::GetIO();
            e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
            e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
        }
    }
}