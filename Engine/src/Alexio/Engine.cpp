#include "aio_pch.h"
#include "Engine.h"
#include "Log.h"
#include "Alexio/Input.h"
#include "Alexio/Math.h"

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
		
		//imgui.OnAttach();
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
		m_gAPI = GraphicsAPI::DirectX11;

		AIO_ASSERT(OnStart(),"Initialization failed");

		mWindow = Window::Create("Alexio Engine", 1280, 720, m_gAPI);
		mWindow->SetEventCallback(BIND_EVENT_FN(Engine::OnEvent));
		Input::SetKeyCodes();

		while (mRunning)
		{
			mWindow->PollEvents();

			Input::Scan();

			//imgui.Begin();

			for (Layer* layer : mLayerStack)
				layer->OnUpdate();
			
			if (!OnUpdate() || 
			// Because Win32 API needs to make my life hard
				(Window::GetAPI() == WindowAPI::Win32 && Input::KeyHeld(Alexio::L_ALT) && Input::KeyPressed(Alexio::F4)))
				mRunning = false;

			//imgui.OnUpdate();
			mWindow->Update();
		}

		//imgui.OnDetach();
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

