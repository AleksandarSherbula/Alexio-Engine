#include "Editor.h"

#include <iostream>

namespace Alexio
{
    Ref<Camera> EditorLayer::sCamera = nullptr;

    Editor::Editor()
    {
        SetGraphicsAPI(OpenGL);

        PushLayer(new EditorLayer());
    }

    Editor::~Editor()
    {
    }

    EditorLayer::EditorLayer()
        : Layer("Editor")
    {
    }

    void EditorLayer::OnStart()
    {
        sCamera = CreateRef<Camera>(static_cast<float>(Engine::GetInstance()->GetWindow()->GetWidth()) 
            / static_cast<float>(Engine::GetInstance()->GetWindow()->GetHeight()));

        texture = Texture::Create("assets/images/AlexioLogo(Black).png");
        texture2 = Texture::Create("assets/images/awesomeface.png");
        tileMap = Texture::Create("assets/images/tilemap.png");

        fbSpec.width = Engine::GetInstance()->GetWindow()->GetWidth();
        fbSpec.height = Engine::GetInstance()->GetWindow()->GetHeight();
        fbSpec.samples = 1;

        framebuffer = Framebuffer::Create(fbSpec);
    }

    void EditorLayer::OnUpdate(float deltaTime)
    {
        if (mViewportFocused)
            sCamera->OnUpdate(deltaTime);

        framebuffer->Bind();
        framebuffer->Clear(0.0f, 0.8f, 1.0f, 1.0f);

        Renderer::DrawRotatedQuad({ -0.9f, -0.9f }, { 0.5f , 0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f }, Timer::Get());

        Renderer::DrawCircle({ -0.5f, 0.5f }, { 1.0f, 0.5f, 0.0f, 1.0f }, 0.5f, 1.0f, 0.5f);

        for (int i = 0; i < 20; i++)
            Renderer::DrawLine({ -1.7f, -0.9f + (i * 0.1f), 0.5f }, { -1.2f, -0.9f + (i * 0.1f), 0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f });

        Renderer::DrawPartialSprite(tileMap, { 0.5f, 0.0f }, { 1.501f, 1.0f }, { 1.0f, 0.0f }, { 16.0f, 16.0f });
        Renderer::DrawSprite(texture, { 0.5f,-1.0f }, { 1.0f, 1.0f });
        Renderer::DrawSprite(texture2, { 0.5f, 0.0f }, { 1.0f, 1.0f });
        Renderer::DrawRotatedSprite(texture, { -0.5f, -0.5f, 1.0f }, { 0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f }, Timer::Get());

        Renderer::Flush();
        framebuffer->Unbind();
    }

    void EditorLayer::OnImGuiRender()
    {
        static bool dockSpaceOpen = true;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace", &dockSpaceOpen, window_flags);
        ImGui::PopStyleVar();

        // ImGuiStyleVar_WindowRounding and ImGuiStyleVar_WindowBorderSize
        ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("DockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        else
        {
            AIO_LOG_ERROR("Docking isn't enabled");
        }

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Exit", NULL, false, dockSpaceOpen != NULL))
                    Engine::GetInstance()->Close();
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

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
        ImGui::Text("");
        ImGui::Text("Application Time: %.2f", Timer::Get());
        ImGui::Text("");
        ImGui::Text("Application framerate:");
        ImGui::Indent();
        ImGui::Text("%.3f ms / frame", 1000.0f / ImGui::GetIO().Framerate);
        ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
        ImGui::Unindent();
        ImGui::Text("");
        ImGui::Text("Rendering Stats:");
        ImGui::Indent();
        ImGui::Text("Lines: %d", Renderer::Stats.Lines);
        ImGui::Text("DrawLine: %d", Renderer::Stats.DrawLine);
        ImGui::Text("");
        ImGui::Text("Quads: %d", Renderer::Stats.Quads);
        ImGui::Text("DrawQuad: %d", Renderer::Stats.DrawQuad);
        ImGui::Text("");
        ImGui::Text("Circles: %d", Renderer::Stats.Circles);
        ImGui::Text("DrawCircle: %d", Renderer::Stats.DrawCircle);
        ImGui::Unindent();
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Viewport");

        mViewportFocused = ImGui::IsWindowFocused();
        mViewportHovered = ImGui::IsWindowHovered();
        Engine::GetInstance()->GetImGuiLayer()->BlockEvents(!mViewportFocused || !mViewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        if (mViewportSize != *((glm::vec2*)&viewportPanelSize))
        {
            framebuffer->OnResize(viewportPanelSize.x, viewportPanelSize.y);
            mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
            sCamera->UpdateProjection(viewportPanelSize.x, viewportPanelSize.y); // TO DO: Set a Camera to run only for Viewport
        }
        ImVec2 uv0 = Renderer::GetGraphicsAPI() == OpenGL ? ImVec2(0, 1) : ImVec2(0, 0); // Top-left UV coordinate
        ImVec2 uv1 = Renderer::GetGraphicsAPI() == OpenGL ? ImVec2(1, 0) : ImVec2(1, 1); // Bottom-right UV coordinate
        ImGui::Image(framebuffer->GetColorAttachment(), ImVec2(mViewportSize.x, mViewportSize.y), uv0, uv1);
        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();
        
    }

    void EditorLayer::OnEvent(Event& event)
    {
        if (sCamera != nullptr && mViewportFocused)
            sCamera->OnEvent(event);
    }
}