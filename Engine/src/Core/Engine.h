#ifndef ENGINE_H
#define ENGINE_H

#include "Window/Window.h"

namespace Alexio
{
	class Engine
	{
	public:
		Engine();
		virtual void OnStart() = 0;
		virtual void OnUpdate() = 0;

		void Run();
		inline static Engine* GetInstance() { return sInstance; }
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