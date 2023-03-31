#include "Alexio/Engine.h"

#include <iostream>

class ExampleLayer : public Alexio::Layer
{
public:
	std::shared_ptr<Alexio::Texture> texture;
	std::shared_ptr<Alexio::Texture> texture2;

	ExampleLayer()
		: Layer("Example")
	{
		texture  = Alexio::Texture::Create("assets/img/AlexioLogo(Black).png");
		texture2 = Alexio::Texture::Create("assets/img/awesomeface.png");
	}

	glm::vec2 position = { -1.0f, -1.0f };
	void OnUpdate(float deltaTime) override
	{
		Alexio::Renderer::Clear(0.0f, 0.8f, 1.0f, 1.0f);

		if (Alexio::Input::KeyHeld(LEFT))
			position.x -= 0.05f * deltaTime;
		if (Alexio::Input::KeyHeld(RIGHT))
			position.x += 0.05f * deltaTime;
		if (Alexio::Input::KeyHeld(UP))
			position.y -= 0.05f * deltaTime;
		if (Alexio::Input::KeyHeld(DOWN))
			position.y += 0.05f * deltaTime;

		Alexio::Renderer::DrawQuad({ position.x, position.y, 1.0f }, { 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f });
		Alexio::Renderer::DrawRotatedQuad({ 0.0f, 0.0f, 0.0f }, { 0.5f, 0.5f }, { 0.5f, 0.5f, 1.0f, 1.0f }, Alexio::Timer::Get());
		
		//Alexio::Renderer::DrawRotatedSprite(texture, {-1.0f, -1.0f}, { 1.0f, 1.0f }, {1.0f, 1.0f, 1.0f, 1.0f});
		//Alexio::Renderer::DrawCircle({-0.5f, 0.5f }, {1.0f, 0.5f, 0.0f, 1.0f}, 0.5f, thickness, fade);
		//Alexio::Renderer::DrawRect({ 0.0f, 0.0f}, { 1.0f, 1.0f }, {1.0f, 1.0f, 0.0f, 1.0f});

		Alexio::Renderer::DrawRotatedSprite(texture, { 0.0f, 0.0f, 1.0f }, { 0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f }, Alexio::Timer::Get());
		Alexio::Renderer::DrawSprite(texture2, { 0.5f, 0.0f }, { 1.0f, 1.0f });
		Alexio::Renderer::DrawSprite(texture,  { 0.5f,-1.0f }, { 1.0f, 1.0f });
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
		Alexio::Renderer::SetGraphicsAPI(OpenGL);
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