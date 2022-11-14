#include "aio_pch.h"
#include "Engine.h"
#include "Log.h"
#include "Input/Input.h"

#include "Events/AppEvent.h"
#include "Window/GLFW_Window.h"

namespace Alexio
{
#define BIND_EVENT_FN(x) std::bind(&Engine::x, this, std::placeholders::_1)

	Engine* Engine::sInstance = nullptr;

	Engine::Engine()
	{
		AIO_ASSERT(!sInstance, "An instance of Engine has already been made");
		sInstance = this;

		m_gAPI = GraphicsAPI::OpenGL;

		mWindow = Window::Create("Alexio Engine", 1280, 720, m_gAPI);
		mWindow->SetEventCallback(BIND_EVENT_FN(OnEvent));
		Input::SetKeyCodes();
		mRunning = true;
	}

	void Engine::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		//AIO_LOG_TRACE("{0}", e);

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
		AIO_ASSERT(OnStart(),"Initialization failed");

		while (mRunning)
		{
			mWindow->PollEvents();

			Input::Scan();

			for (Layer* layer : mLayerStack)
				layer->OnUpdate();
			
			if (!OnUpdate() || 
			// Because Win32 needs to make my life hard
				(Window::GetAPI() == WindowAPI::Win32 && Input::KeyHeld(Alexio::L_ALT) && Input::KeyPressed(Alexio::F4)))
				mRunning = false;

			mWindow->Update();
		}
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


