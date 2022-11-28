#ifndef APP_EVENT_H
#define APP_EVENT_H

#include "Event.h"

namespace Alexio
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		int GetCategoryFlags() const override { return EventCategoryApplication; }

		static EventType GetStaticType() { return EventType::WindowResize; }
		EventType GetEventType() const override { return GetStaticType(); }

		const char* GetName() const override { return "WindowResize"; }
	private:
		unsigned int m_Width, m_Height;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		static EventType GetStaticType() { return EventType::WindowClose; }
		EventType GetEventType() const override { return GetStaticType(); }

		const char* GetName() const override { return "WindowClose"; }

		int GetCategoryFlags() const override { return EventCategoryApplication; }
	};

	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() = default;

		static EventType GetStaticType() { return EventType::AppTick; }
		EventType GetEventType() const override { return GetStaticType(); }

		const char* GetName() const override { return "AppTick"; }

		int GetCategoryFlags() const override { return EventCategoryApplication; }
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;

		static EventType GetStaticType() { return EventType::AppUpdate; }
		EventType GetEventType() const override { return GetStaticType(); }

		const char* GetName() const override { return "AppUpdate"; }

		int GetCategoryFlags() const override { return EventCategoryApplication; }
	};

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;

		static EventType GetStaticType() { return EventType::AppRender; }
		EventType GetEventType() const override { return GetStaticType(); }

		const char* GetName() const override { return "AppRender"; }

		int GetCategoryFlags() const override { return EventCategoryApplication; }
	};
}

#endif // !APP_EVENT_H