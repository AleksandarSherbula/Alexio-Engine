#ifndef ENGINE_H
#define ENGINE_H

namespace Alexio
{
	class Engine
	{
	public:
		virtual void OnStart() = 0;
		virtual void OnUpdate() = 0;

		void Run();
	};
}

#endif // ENGINE_H