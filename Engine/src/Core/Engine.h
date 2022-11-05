#ifndef ENGINE_H
#define ENGINE_H

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
	private:
		static Engine* sInstance;
	};
}

Alexio::Engine* Create();



#endif // ENGINE_H