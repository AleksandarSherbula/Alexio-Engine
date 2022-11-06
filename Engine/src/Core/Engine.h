#ifndef ENGINE_H
#define ENGINE_H

#include "Window/Window.h"
#include "Events/AppEvent.h"

namespace Alexio
{
	class Engine
	{
	public:
		inline static Engine* GetInstance() { return sInstance; }
	public:
		Engine();
		virtual void OnStart() = 0;
		virtual void OnUpdate() = 0;

		void Run();

		void OnEvent(Event& e);
	private:
		bool OnWindowClose(WindowCloseEvent& e);
	protected:
		GraphicsAPI m_gAPI;
	private:
		std::unique_ptr<Window> mWindow;
		bool mRunning;
	private:
		static Engine* sInstance;
	};
}

Alexio::Engine* Create();



#endif // ENGINE_H