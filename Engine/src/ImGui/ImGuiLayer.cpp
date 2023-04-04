#include "aio_pch.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer/Renderer.h"
#include "Alexio/Random.h"

#include "Alexio/Timer.h"

namespace Alexio
{
    void ImGUI::OnAttach()
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

    void ImGUI::Begin()
    {
        Renderer::GetBackend()->ImGuiBackendBegin();
    }

    void ImGUI::End()
    {
        Renderer::GetBackend()->ImGuiBackendUpdate();
    }

    void ImGUI::OnDetach()
    {
        Renderer::GetBackend()->ImGuiBackendShutDown();
        ImGui::DestroyContext();
    }

    void ImGUI::OnImGuiRender()
    {
        char* apiName = "";
        #if defined(AIO_API_OPENGL) && defined(AIO_API_DX11)
                apiName = (Renderer::GetGraphicsAPI() == OpenGL) ? "OpenGL 4.5" : "DirectX 11";
        #elif defined(AIO_API_OPENGL)
                apiName = "OpenGL";
        #elif defined(AIO_API_DX11)
                apiName = "DirectX11";
        #endif

        ImGui::Begin("App Info");
        ImGui::Text("Graphics API: %s", apiName);
        ImGui::Text("Application Time: %.2f", Timer::Get());
        ImGui::Text("Application framerate: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("Random number generated (0 - 10) %d", Random::Range(0, 10));
        if (ImGui::CollapsingHeader("Rendering Stats")) {
            ImGui::Indent(); // indent the contents of the list
            ImGui::Text("Lines: %d", Renderer::Stats.Lines);
            ImGui::Text("DrawLine: %d", Renderer::Stats.DrawLine);
            ImGui::Text("");
            ImGui::Text("Quads: %d", Renderer::Stats.Quads);
            ImGui::Text("DrawQuad: %d", Renderer::Stats.DrawQuad);
            ImGui::Text("");
            ImGui::Text("Circles: %d", Renderer::Stats.Circles);
            ImGui::Text("DrawCircle: %d", Renderer::Stats.DrawCircle);
            ImGui::Unindent(); // unindent the contents of the list
        }
        ImGui::End();
    }
}