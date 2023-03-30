#include "Alexio/Engine.h"

class ExampleLayer : public Alexio::Layer
{
public:
	std::shared_ptr<Alexio::Texture> texture;
	std::shared_ptr<Alexio::Texture> texture2;

	ExampleLayer()
		: Layer("Example")
	{
		texture  = Alexio::Texture::Create("assets/img/AlexioLogo(Black).png");
		texture2  = Alexio::Texture::Create("assets/img/awesomeface.png");
	}

	void OnUpdate(float deltaTime) override
	{
		Alexio::Renderer::Clear(0.0f, 0.8f, 1.0f, 1.0f);

		for (int i = 0; i < 2; i++)
			Alexio::Renderer::DrawQuad({-1.0f + 2 * i, -1.0f}, {0.5f, 0.5f}, {0.0f, 0.0f, 1.0f, 1.0f});
		
		//Alexio::Renderer::DrawRotatedQuad({ 0.0f, -1.0f}, { 1.0f, 1.0f }, {0.5f, 0.0f, 1.0f, 1.0f}, Alexio::Timer::Get());
		//Alexio::Renderer::DrawRotatedSprite(texture, {-1.0f, -1.0f}, { 1.0f, 1.0f }, {1.0f, 1.0f, 1.0f, 1.0f});
		//Alexio::Renderer::DrawCircle({-0.5f, 0.5f }, {1.0f, 0.5f, 0.0f, 1.0f}, 0.5f, thickness, fade);
		//Alexio::Renderer::DrawRect({ 0.0f, 0.0f}, { 1.0f, 1.0f }, {1.0f, 1.0f, 0.0f, 1.0f});

		Alexio::Renderer::DrawSprite(texture, { -0.5f, 0.0f }, { 1.0f, 1.0f });
		Alexio::Renderer::DrawSprite(texture2, {  0.5f, 0.0f }, { 1.0f, 1.0f });
		Alexio::Renderer::DrawSprite(texture, {  0.5f, 0.5f }, { 1.0f, 1.0f });
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