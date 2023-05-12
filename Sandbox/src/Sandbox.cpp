#include "Sandbox.h"

#include <iostream>

using namespace Alexio;

Sandbox::Sandbox()
{
	SetGraphicsAPI(DirectX11);

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
}

void MainLayer::OnUpdate()
{
    Renderer::Clear(0.0f, 0.8f, 1.0f, 1.0f);

	Alexio::Renderer::DrawRotatedQuad({ -0.9f, -0.9f }, { 0.5f , 0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f }, Alexio::Timer::Get());
    
	Alexio::Renderer::DrawCircle({ -0.5f, 0.5f }, { 1.0f, 0.5f, 0.0f, 1.0f }, 0.5f, 1.0f, 0.5f);
    
	for (int i = 0; i < 20; i++)
		Alexio::Renderer::DrawLine({ -1.7f, -0.9f + (i * 0.1f), 0.5f }, { -1.2f, -0.9f + (i * 0.1f), 0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f });
    
	Alexio::Renderer::DrawPartialSprite(tileMap, { 0.5f, 0.0f }, { 1.501f, 1.0f }, { 1.0f, 0.0f }, { 16.0f, 16.0f });
    Alexio::Renderer::DrawSprite(texture, { 0.5f,-1.0f }, { 1.0f, 1.0f });
	Alexio::Renderer::DrawSprite(texture2, { 0.5f, 0.0f }, { 1.0f, 1.0f });
	Alexio::Renderer::DrawRotatedSprite(texture, { -0.5f, -0.5f, 1.0f }, { 0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f }, Alexio::Timer::Get());
    
    Renderer::Flush();
}

void MainLayer::OnImGuiRender()
{
}

void MainLayer::OnEvent(Alexio::Event& event)
{
}