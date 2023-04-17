#include "aio_pch.h"
#include "Engine.h"

#include "Random.h"
#include "Timer.h"
#include "Renderer/Renderer.h"
#include "Input/Input.h"

namespace Alexio
{
	Engine* Engine::sInstance = nullptr;

	Engine::Engine()
	{
		Alexio::Log::Initialize();

		AIO_ASSERT(!sInstance, "An instance of Engine has already been made");
		sInstance = this;

		SetGraphicsAPI(OpenGL);
		mRunning = true;
	}

	void Engine::Run()
	{
		mImGuiLayer = new ImGuiLayer();

		mWindow = Window::Create("Alexio Engine", 1280, 720, AIO_BIND_EVENT_FN(Engine::OnEvent));
		Input::SetKeyCodes();

		Renderer::Init();

		Random::Init();

		PushOverlay(mImGuiLayer);
		for (Layer* layer : mLayerStack)
			layer->OnStart();

		Timer::StartApp();
		while (mRunning)
		{
			Timer::Update();

			mWindow->PollEvents();
			Input::Scan();

			Renderer::Stats = {0};
			
			mImGuiLayer->Begin();
			// Manual check for closing on alt + F4 for Win32 API, since the system keys are not being checked
			if ((Renderer::GetGraphicsAPI() == DirectX11 && (Input::KeyHeld(L_ALT) && Input::KeyPressed(F4))))
				Close();

			for (Layer* layer : mLayerStack)
				layer->OnUpdate(Timer::DeltaTime());
			
			for (Layer* layer : mLayerStack)
				layer->OnImGuiRender();

			mImGuiLayer->End();

			Renderer::GetBackend()->SwapBuffer();
		}

		Renderer::End();
	}

	void Engine::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(AIO_BIND_EVENT_FN(Engine::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(AIO_BIND_EVENT_FN(Engine::OnWindowResize));
				
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

	void Engine::SetGraphicsAPI(GraphicsAPI api)
	{
		Renderer::SetGraphicsAPI(api);
	}

	bool Engine::OnWindowClose(WindowCloseEvent& e)
	{
		mRunning = false;
		return true;
	}

	bool Engine::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() != 0 && e.GetHeight() != 0)
			Renderer::SetViewport(0, 0, e.GetWidth(), e.GetHeight());
		return true;
	}
}



