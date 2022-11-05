#ifndef WINDOW_H
#define WINDOW_H

#include "Core/Utilities.h"

namespace Alexio
{
	class Window
	{
	public:
		virtual ~Window() = 0;
		virtual void Initialize() = 0;
		virtual void Update() = 0;

		virtual inline void* GetHandle() = 0;
		inline uint32_t GetWidth() { return mWidth; }
		inline uint32_t GetHeight() { return mHeight; }
	public:
		static std::unique_ptr<Window> Create(const std::string& title, uint32_t width, uint32_t height, GraphicsAPI api);
	protected:
		uint32_t mWidth;
		uint32_t mHeight;
		std::string mTitle;
	};

}

#endif // !WINDOW_H