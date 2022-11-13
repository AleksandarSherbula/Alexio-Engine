#ifndef WINDOW_H
#define WINDOW_H

#include "../Core/Utilities.h"
#include "../Events/Event.h"

namespace Alexio
{
	using EventCallbackFn = std::function<void(Event&)>;

	class Window
	{
	public:

		virtual ~Window() = 0;
		virtual void Initialize() = 0;
		virtual void Update() = 0;

		virtual void PollEvents() = 0;

		inline uint32_t GetWidth() { return mWidth; }
		inline uint32_t GetHeight() { return mHeight; }		

		inline static WindowAPI GetAPI() { return sAPI; }

		virtual inline void* GetHandle() = 0;
		virtual inline void SetEventCallback(const EventCallbackFn& callback) = 0;
	public:
		static std::unique_ptr<Window> Create(const std::string& title, uint32_t width, uint32_t height, GraphicsAPI api);
	private:
		static WindowAPI sAPI;
	protected:
		uint32_t mWidth;
		uint32_t mHeight;
		std::string mTitle;	
		
	};

}

#endif // !WINDOW_H