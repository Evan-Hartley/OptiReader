#pragma once

#include "keepch.h"
#include "Keela/Core.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Keela {

	//As it stands Keela currently deals with events by immediately dispatching them
	// and therefore events must be delt with right at that moment. In the future, I
	// will implement a buffer for events by implementing an event bus and processing
	// them during the "event" part of the update stage. But this works for now.

	// Define Event Types that will be defined in respective headerfiles
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	// Categorize the event so we can tell what type of event it is in case we want to filter
	// BIT feild is defined in Core.h
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication    = BIT(0),
		EventCategoryInput          = BIT(1),
		EventCategoryKeyboard       = BIT(2),
		EventCategoryMouse          = BIT(3),
		EventCategoryMouseButton    = BIT(4)
	};


// define macros needed for the classes, to be referrenced in other event header files
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
                                virtual EventType GetEventType() const override { return GetStaticType(); }\
                                virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	//-------- Base class for events--------------------------------------------------------------
	// Pure Virtual Functions (must be implemented): GetEventType, GetName, GetCategory Flags
	// Name and possibly GetCategoryFlag will be debug only
	// ToString returns the name of the event, is implemented by default
	// IsInCategory checks to see if the event is in the given category
	// The handled boolean allows us to determine whether or not the event will continue to propogate
	class KEELA_API Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
		bool Handled = false;
	};


	//-------- Event Dispatcher --------------------------------------------------------------
	// Correlates the event function with the event type
	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};


	//-------- Output String Operator --------------------------------------------------------------
	// Makes Logging Events Easy
	inline std::string format_as(const Event& e)
	{
		return e.ToString();
	}
}