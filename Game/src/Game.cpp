#include "Alexio/Engine.h"

class ExampleLayer : public Alexio::Layer
{
public:
	std::shared_ptr<Alexio::VertexArray>  va;
	std::shared_ptr<Alexio::VertexBuffer> vb;
	std::shared_ptr<Alexio::IndexBuffer>  ib;
	std::shared_ptr<Alexio::Shader>       shader;
	std::shared_ptr<Alexio::Texture>      texture;

	glm::vec4 clearColor;
	float thickness = 1.0f;
	float fade = 0.005f;

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
		ImGui::SliderFloat("Circle Thickness", &thickness, 0.0f, 1.0f);
		ImGui::SliderFloat("Circle Fade", &fade, 0.0f, 1.0f);
		ImGui::End();
	}

	void OnUpdate(float deltaTime) override
	{
		Alexio::Renderer::Clear(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

		Alexio::Renderer::DrawRotatedQuad({ 0.0f, -1.0f}, { 1.0f, 1.0f }, {0.5f, 0.0f, 1.0f, 1.0f}, Alexio::Timer::Get());
		Alexio::Renderer::DrawRotatedSprite(texture, {-1.0f, -1.0f}, { 1.0f, 1.0f }, {1.0f, 1.0f, 1.0f, 1.0f});
		Alexio::Renderer::DrawCircle({-0.5f, 0.5f }, {1.0f, 0.5f, 0.0f, 1.0f}, 0.5f, thickness, fade);
		Alexio::Renderer::DrawRect({ 0.0f, 0.0f}, { 1.0f, 1.0f }, {1.0f, 1.0f, 0.0f, 1.0f});
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