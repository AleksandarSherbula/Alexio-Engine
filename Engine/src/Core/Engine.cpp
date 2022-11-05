#include "aio_pch.h"
#include "Engine.h"
#include "Log.h"

#include "Events/AppEvent.h"
#include "Window/GLFW_Window.h"

#include "GLFW/glfw3.h"

namespace Alexio
{
	Engine* Engine::sInstance = nullptr;

	Engine::Engine()
	{
		AIO_ASSERT(!sInstance, "An instance of Engine has already been made");
		sInstance = this;

		m_gAPI = GraphicsAPI::OpenGL;
		mWindow = Window::Create("Alexio Engine", 1280, 720, m_gAPI);
		mRunning = true;
	}

	void Engine::Run()
	{
		OnStart();

		WindowResizeEvent e(1280, 720);
		AIO_LOG_TRACE(e);

		while (mRunning)
		{
			glClear(GL_COLOR_BUFFER_BIT);
			glClearColor(0.0f, 0.8f, 1.0f, 1.0f);

			OnUpdate();

			mWindow->Update();
		}
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


