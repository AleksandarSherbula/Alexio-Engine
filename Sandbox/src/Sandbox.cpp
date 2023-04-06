#include "Sandbox.h"

#include <iostream>

using namespace Alexio;

Sandbox::Sandbox()
{
	SetGraphicsAPI(OpenGL);

	PushLayer(new MainLayer());
}

Sandbox::~Sandbox()
{
}

MainLayer::MainLayer()
	: Layer("Main")
{
}

void MainLayer::OnStart()
{
	texture = Alexio::Texture::Create("assets/images/AlexioLogo(Black).png");
	texture2 = Alexio::Texture::Create("assets/images/awesomeface.png");
	tileMap = Alexio::Texture::Create("assets/images/tilemap.png");

    fbSpec.width = 1280;
    fbSpec.height = 720;
    fbSpec.samples = 1;

    framebuffer = Alexio::Framebuffer::Create(fbSpec);
}

void MainLayer::OnUpdate(float deltaTime)
{
    framebuffer->Bind();

    Engine::GetInstance()->GetCamera()->OnUpdate(deltaTime);

	Renderer::Clear(0.0f, 0.8f, 1.0f, 1.0f);
    
	//Alexio::Renderer::DrawRotatedQuad({ -0.0f, -0.0f }, { 0.5f , 0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f }, Alexio::Timer::Get());
    
	Alexio::Renderer::DrawCircle({ -0.5f, 0.5f, 0.5f }, { 1.0f, 0.5f, 0.0f, 1.0f }, 0.5f);
    
	for (int i = 0; i < 100; i++)
		Alexio::Renderer::DrawLine({ -1.7f, -0.9f + (i * 0.1f), 0.5f }, { -1.2f, -0.9f + (i * 0.1f), 0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f });
    
	Alexio::Renderer::DrawPartialSprite(tileMap, { 0.5f, 0.0f }, { 1.501f, 1.0f }, { 1.0f, 0.0f }, { 16.0f, 16.0f });
	Alexio::Renderer::DrawSprite(texture, { 0.5f,-1.0f }, { 1.0f, 1.0f });
	Alexio::Renderer::DrawSprite(texture2, { 0.5f, 0.0f }, { 1.0f, 1.0f });
	Alexio::Renderer::DrawRotatedSprite(texture, { -0.5f, -0.5f, 1.0f }, { 0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f }, Alexio::Timer::Get());
    
    Renderer::Flush();
    framebuffer->Unbind();
}

void MainLayer::OnImGuiRender()
{
    static bool dockingEnabled = true;

    if (dockingEnabled)
    {
        static bool dockSpaceOpen = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
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

        ImGui::Begin("Framebuffer");
        ImGui::Image(framebuffer->GetColorAttachment(), ImVec2(1280, 720));
        ImGui::End();

        ImGui::End();
    }
}

void MainLayer::OnEvent(Alexio::Event& event)
{
}