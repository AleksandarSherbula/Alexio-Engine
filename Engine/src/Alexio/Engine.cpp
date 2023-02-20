#include "aio_pch.h"
#include "Engine.h"

namespace Alexio
{
	Engine* Engine::sInstance = nullptr;

	Engine::Engine()
	{
		AIO_ASSERT(!sInstance, "An instance of Engine has already been made");
		sInstance = this;
		
		mRunning = true;
	}

	void Engine::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Engine::OnWindowClose, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&Engine::OnWindowResize, this, std::placeholders::_1));

		for (auto it = mLayerStack.end(); it != mLayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Engine::PushLayer(Layer* layer)
	{
		mLayerStack.PushLayer(layer);
	}

	void Engine::PushOverlay(Layer* layer)
	{
		mLayerStack.PushOverlay(layer);
	}

	void Engine::Run()
	{
		Renderer::SetGraphicsAPI(GraphicsAPI::DirectX11);

		std::string apiName = (Renderer::GetGraphicsAPI() == GraphicsAPI::OpenGL) ? "OpenGL" : "DirectX11";

		mWindow = Window::Create("Alexio Engine (" + apiName + ")", 1280, 720);
		mWindow->SetEventCallback(std::bind(&Engine::OnEvent, this, std::placeholders::_1));
		Input::SetKeyCodes();

		Renderer::Begin(mWindow.get());

		AIO_ASSERT(OnStart(),"Initialization failed");

		while (mRunning)
		{
			mWindow->PollEvents();

			Input::Scan();

			for (Layer* layer : mLayerStack)
				layer->OnUpdate();
			
			if (!OnUpdate() ||
				// Manual code for closing on alt + F4 for Win32 API, since the system keys are not being checked
				(Window::GetAPI() == WindowAPI::Win32 && Input::KeyHeld(Alexio::L_ALT) && Input::KeyPressed(Alexio::F4)))
				mRunning = false;
			
			Renderer::ClearColor(0.0f, 0.0f, 0.0f, 1.0f);

			Renderer::Draw();

			Renderer::imgui->Begin();
			Renderer::imgui->OnUpdate();
			Renderer::imgui->End();

			Renderer::SwapBuffer();
		}

		Renderer::End();
	}	

	bool Engine::OnWindowClose(WindowCloseEvent& e)
	{
		mRunning = false;
		return true;
	}

	bool Engine::OnWindowResize(WindowResizeEvent& e)
	{
		if (Renderer::GetAPI() != nullptr)
			Renderer::GetAPI()->SetViewport(0, 0, e.GetWidth(), e.GetHeight());
		return true;
	}
}


int main(int argc, char** agrv)
{
	Alexio::Log::Initialize();

	Alexio::Engine* engine = Create();
	engine->Run();
	delete engine;
	return 0;
}


