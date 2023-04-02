#include "Alexio/Engine.h"

#include <iostream>

class ExampleLayer : public Alexio::Layer
{
public:
	std::shared_ptr<Alexio::Texture> texture;
	std::shared_ptr<Alexio::Texture> texture2;
	std::shared_ptr<Alexio::Texture> tileMap;

	ExampleLayer()
		: Layer("Example")
	{
		texture  = Alexio::Texture::Create("assets/img/AlexioLogo(Black).png");
		texture2 = Alexio::Texture::Create("assets/img/awesomeface.png");
		tileMap = Alexio::Texture::Create("assets/img/tilemap.png");
	}

	void OnUpdate(float deltaTime) override
	{
		Alexio::Renderer::Clear(0.0f, 0.8f, 1.0f, 1.0f);
		
		Alexio::Renderer::DrawRotatedQuad({ -0.9f, -0.9f}, { 0.5f , 0.5f }, {1.0f, 1.0f, 0.0f, 1.0f}, Alexio::Timer::Get());
		
		Alexio::Renderer::DrawCircle({-0.5f, 0.5f, 0.5f }, {1.0f, 0.5f, 0.0f, 1.0f}, 0.5f);

		for (int i = 0; i < 1100; i++)
			Alexio::Renderer::DrawLine({-1.7f, -0.9f + (i * 0.1f), 0.5f}, {-1.2f, -0.9f + (i * 0.1f), 0.5f}, {1.0f, 1.0f, 0.0f, 1.0f});

		Alexio::Renderer::DrawPartialSprite(tileMap, { 0.5f, 0.0f }, {1.501f, 1.0f}, { 0.0f, 0.0f }, { 16.0f, 32.0f });
		Alexio::Renderer::DrawSprite(texture,  { 0.5f,-1.0f }, { 1.0f, 1.0f });
		Alexio::Renderer::DrawRotatedSprite(texture, {-0.5f, -0.5f, 1.0f}, { 0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f }, Alexio::Timer::Get());		
	}

	void OnImGuiRender() override
	{
	}

	void OnEvent(Alexio::Event& event) override
	{
	}
};

class Game : public Alexio::Engine
{
public:
	Game::Game()
	{
		// Set Graphics API here
		// Example: Alexio::Renderer::SetGraphicsAPI(OpenGL);
		Alexio::Renderer::SetGraphicsAPI(DirectX11);
	}

	bool OnStart() override
	{
		PushLayer(new ExampleLayer());
		return true;
	}

	bool OnUpdate() override
	{
		return !Alexio::Input::KeyPressed(ESCAPE);
	}
};

int main(int argc, char** agrv)
{
	Game* game = new Game();
	game->Run();
	delete game;
	return 0;
}