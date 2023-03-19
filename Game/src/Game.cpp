#include "Alexio/Engine.h"

class ExampleLayer : public Alexio::Layer
{
public:
	std::shared_ptr<Alexio::VertexArray> va;
	std::shared_ptr<Alexio::VertexBuffer> vb;
	std::shared_ptr<Alexio::IndexBuffer> ib;
	std::shared_ptr<Alexio::Shader> shader;
	std::shared_ptr<Alexio::Texture> texture;

	glm::vec4 clearColor;

	ExampleLayer()
		: Layer("Example")
	{
		clearColor = { 0.0f, 0.8f, 1.0f, 1.0f };

		texture = Alexio::Texture::Create("assets/img/awesomeface.png");
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Clear Color", &clearColor.r);
		ImGui::End();
	}

	void OnUpdate(float deltaTime) override
	{
		Alexio::Renderer::Clear(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

		Alexio::Renderer::DrawQuad({-0.2f, 0.0f}, { 1.0f, 1.0f }, {0.5f, 0.0f, 1.0f, 1.0f});
		Alexio::Renderer::DrawSprite(texture, { 0.0f, 0.0f}, { 1.0f, 1.0f }, {1.0f, 1.0f, 1.0f, 1.0f});
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