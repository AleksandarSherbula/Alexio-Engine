#include "Alexio.h"

class ExampleLayer : public Alexio::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		//AIO_LOG_INFO("Example Layer: Updated");
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
		
	}

	bool Game::OnStart() override
	{
		Alexio::Renderer::SetAPI(GraphicsAPI::DirectX11);

		Alexio::Vector4f a(1.4f, 2.5f, 3.2f, 4.7f);
		Alexio::Vector4f b(2.7f, 3.2f, 3.2f, 4.7f);

		AIO_LOG_INFO(a + b);

		PushLayer(new ExampleLayer());

		return true;
	}

	bool Game::OnUpdate() override
	{
		if (Alexio::Input::MouseButtonPressed(Alexio::L_BUTTON))
			AIO_LOG_TRACE("L_BUTTON was pressed");
		if (Alexio::Input::MouseButtonReleased(Alexio::L_BUTTON))
			AIO_LOG_TRACE("L_BUTTON is being released");
		
		if (Alexio::Input::MouseButtonPressed(Alexio::R_BUTTON))
			AIO_LOG_TRACE("R_BUTTON was pressed");
		if (Alexio::Input::MouseButtonReleased(Alexio::R_BUTTON))
			AIO_LOG_TRACE("R_BUTTON is being released");

		if (Alexio::Input::MouseButtonPressed(Alexio::M_BUTTON))
			AIO_LOG_TRACE("M_BUTTON was pressed");
		if (Alexio::Input::MouseButtonReleased(Alexio::M_BUTTON))
			AIO_LOG_TRACE("M_BUTTON is being released");

		return !Alexio::Input::KeyPressed(Alexio::ESCAPE);
	}
};

Alexio::Engine* Create()
{
	return new Game();
}