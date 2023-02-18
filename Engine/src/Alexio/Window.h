#pragma once

#include "Utilities.h"
#include "Events/Event.h"

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

		inline uint32_t GetWidth() { return mWidth; }
		inline uint32_t GetHeight() { return mHeight; }

		inline void SetWidth(uint32_t width) { mWidth = width; }
		inline void SetHeight(uint32_t height) { mHeight = height; }

		inline static WindowAPI GetAPI() { return sAPI; }

		virtual inline void* GetHandle() = 0;
		virtual inline void SetEventCallback(const EventCallbackFn& callback) = 0;
	public:
		static std::unique_ptr<Window> Create(const std::string& title, uint32_t width, uint32_t height, GraphicsAPIflag api);
	private:
		static WindowAPI sAPI;
	protected:
		uint32_t mWidth;
		uint32_t mHeight;
		std::string mTitle;	
		
	};

}