#pragma once

#include "Alexio/Utilities.h"
#include "Events/Events.h"
#include "Input/Input.h"
#include "Math/Math.h"

#include <functional>

namespace Alexio
{
	using EventCallbackFn = std::function<void(Event&)>;

	struct WindowDataFromCallback
	{
		uint32_t width, height;

		EventCallbackFn eventCallback;
	};

	class Window
	{
	public:
		virtual ~Window() = 0;
		virtual void Initialize() = 0;

		virtual void PollEvents() = 0;
		virtual Vector2 GetMousePositionFromWindowAPI() = 0;

		inline uint32_t GetWidth()  const { return mCallbackData.width; }
		inline uint32_t GetHeight() const { return mCallbackData.height; }

		inline bool IsFullScreen() const { return mIsFullScreen; }

		WindowDataFromCallback GetCallbackData() { return mCallbackData; }
		
		virtual void* GetHandle() const = 0;
		virtual void SetFullScreen(bool fullscreen) = 0;
	
		static Ref<Window> Create(const std::string& title, uint32_t width, uint32_t height, const EventCallbackFn& eventCallback);
	protected:
		std::string mTitle;

		WindowDataFromCallback mCallbackData;

		bool mIsFullScreen;
	};

}