#include "aio_pch.h"
#include "Engine.h"
#include "Log.h"
#include "Input.h"
#include "Math.h"
#include "Renderer.h"

#include "Events/AppEvent.h"
#include "Window/GLFW_Window.h"

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
		Renderer::SetAPIflag(GraphicsAPI::DirectX11);

		std::string apiName = (Renderer::GetAPIflag() == GraphicsAPI::DirectX11) ? "DirectX 11" : "OpenGL";

		mWindow = Window::Create("Alexio Engine (" + apiName + ")", 1280, 720, Renderer::GetAPIflag());
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
			// Because Win32 API needs to make my life hard
				(Window::GetAPI() == WindowAPI::Win32 && Input::KeyHeld(Alexio::L_ALT) && Input::KeyPressed(Alexio::F4)))
				mRunning = false;

			Renderer::DrawFrame();
		}

		Renderer::End();
	}	

	bool Engine::OnWindowClose(WindowCloseEvent& e)
	{
		mRunning = false;
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


