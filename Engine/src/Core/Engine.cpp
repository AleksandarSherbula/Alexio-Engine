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
		mRunning = true;
	}

	void Engine::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		if (e.GetEventType() == EventType::KeyPressed)
		{
			
		}

		//AIO_LOG_TRACE("{0}", e);
	}

	void Engine::Run()
	{
		OnStart();

		while (mRunning)
		{
			Input::Scan();

			OnUpdate();

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


