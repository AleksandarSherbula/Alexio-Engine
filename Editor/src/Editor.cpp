#include "Editor.h"

#include <iostream>

namespace Alexio
{
    Editor::Editor()
    {
        SetGraphicsAPI(DirectX11);

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
        texture  = Texture::Create("assets/images/AlexioLogo(Black).png");
        texture2 = Texture::Create("assets/images/awesomeface.png");
        tileMap  = Texture::Create("assets/images/tilemap.png");

        mCurrentScene = CreateRef<Scene>();

        mSquare = mCurrentScene->CreateObject("Square");
        mSquare.AddComponent<SpriteRendererComponent>(Vector4(0.0f, 0.0f, 1.0f, 1.0f));

        fbSpec.width = Engine::Get()->ScreenWidth();
        fbSpec.height = Engine::Get()->ScreenHeight();
        fbSpec.samples = 1;

        framebuffer = Framebuffer::Create(fbSpec);
    }

    void EditorLayer::OnUpdate()
    {    
        if (mViewportSize.x > 0.0f && mViewportSize.y > 0.0f && // zero sized framebuffer is invalid
            (fbSpec.width != mViewportSize.x || fbSpec.height != mViewportSize.y))
        {
            framebuffer->OnResize((uint32_t)mViewportSize.x, (uint32_t)mViewportSize.y);
            mCurrentScene->OnViewportResize(mViewportSize.x, mViewportSize.y);
        }

        framebuffer->Bind();
        framebuffer->Clear(0.0f, 0.8f, 1.0f, 1.0f);

        mCurrentScene->OnUpdate();
       
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
                    Engine::Get()->Close();
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        char* apiName = "";
        #if defined(AIO_API_OPENGL) && defined(AIO_API_DX11)
            apiName = (Renderer::GetGraphicsAPI() == OpenGL) ? "OpenGL 4.5" : "DirectX 11";
        #elif defined(AIO_API_OPENGL)
            apiName = "OpenGL 4.5";
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
        ImGui::Text("");
        ImGui::Unindent();

        if (mSquare)
        {
            TagComponent& tag = mSquare.GetComponent<TagComponent>();
            SpriteRendererComponent sprRenderer = mSquare.GetComponent<SpriteRendererComponent>();

            ImGui::Text("Object: %s", tag.Tag.c_str());
            ImGui::ColorEdit4("Square Color", glm::value_ptr(sprRenderer.Color));
        }
        
        ImGui::Text("Tag: %s", mCurrentScene->GetCamera().GetComponent<TagComponent>().Tag.c_str());
        ImGui::Text("Camera Transform: ");
        ImGui::DragFloat3("",
            glm::value_ptr(mCurrentScene->GetCamera().GetComponent<TransformComponent>().Transform[3]), 0.05f);

        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Viewport");

        mViewportFocused = ImGui::IsWindowFocused();
        mViewportHovered = ImGui::IsWindowHovered();
        Engine::Get()->GetImGuiLayer()->BlockEvents(!mViewportFocused || !mViewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        ImVec2 uv0 = Renderer::GetGraphicsAPI() == OpenGL ? ImVec2(0, 1) : ImVec2(0, 0); // Top-left UV coordinate
        ImVec2 uv1 = Renderer::GetGraphicsAPI() == OpenGL ? ImVec2(1, 0) : ImVec2(1, 1); // Bottom-right UV coordinate
        ImGui::Image(framebuffer->GetColorAttachment(), ImVec2(mViewportSize.x, mViewportSize.y), uv0, uv1);
        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();
        
    }

    void EditorLayer::OnEvent(Event& e)
    {
    }
}