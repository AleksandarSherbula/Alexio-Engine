#pragma once

#include "Alexio/Utilities.h"
#include "Events/Events.h"
#include "Input/Input.h"

#include <functional>

namespace Alexio
{
	using EventCallbackFn = std::function<void(Event&)>;

	class Window
	{
	public:
		virtual ~Window() = 0;
		virtual void Initialize() = 0;

		virtual void PollEvents() = 0;
		virtual glm::vec2 GetMousePositionFromWindowAPI() = 0;

		inline uint32_t GetWidth()  const { return mWidth; }
		inline uint32_t GetHeight() const { return mHeight; }

		inline void SetWidth(uint32_t width) { mWidth = width; }
		inline void SetHeight(uint32_t height) { mHeight = height; }
		inline bool IsFullScreen() const { return mIsFullScreen; }

		virtual void* GetHandle() const = 0;
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetFullScreen(bool fullscreen) = 0;
	
		static Ref<Window> Create(const std::string& title, uint32_t width, uint32_t height);
	protected:
		uint32_t mWidth;
		uint32_t mHeight;
		std::string mTitle;

		bool mIsFullScreen;
	};

}