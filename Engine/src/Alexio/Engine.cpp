#include "aio_pch.h"
#include "Engine.h"

namespace Alexio
{
	Engine* Engine::sInstance = nullptr;
	Ref<Camera> Engine::sMainCamera = nullptr;

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
		imgui = new ImGUI();

		mWindow = Window::Create("Alexio Engine", 1280, 720);
		mWindow->SetEventCallback(AIO_BIND_EVENT_FN(Engine::OnEvent));
		Input::SetKeyCodes();

		sMainCamera = CreateRef<Camera>(static_cast<float>(mWindow->GetWidth()) / static_cast<float>(mWindow->GetHeight()));
		Renderer::Init();

		Random::Init();

		PushOverlay(imgui);
		for (Layer* layer : mLayerStack)
			layer->OnStart();

		Timer::StartApp();
		while (mRunning)
		{
			Timer::Update();

			mWindow->PollEvents();
			Input::Scan();

			Renderer::Stats = {0};
			
			imgui->Begin();
			// Manual check for closing on alt + F4 for Win32 API, since the system keys are not being checked
			if ((Renderer::GetGraphicsAPI() == DirectX11 && (Input::KeyHeld(L_ALT) && Input::KeyPressed(F4))))
				Close();

			sMainCamera->OnUpdate(Timer::DeltaTime());

			for (Layer* layer : mLayerStack)
				layer->OnUpdate(Timer::DeltaTime());
			
			for (Layer* layer : mLayerStack)
				layer->OnImGuiRender();

			imgui->End();

			Renderer::GetBackend()->SwapBuffer();
		}

		Renderer::End();
	}

	void Engine::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(AIO_BIND_EVENT_FN(Engine::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(AIO_BIND_EVENT_FN(Engine::OnWindowResize));

		if (sMainCamera != nullptr)
			sMainCamera->OnEvent(e);

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



