#pragma once

#include "Window/Window.h"
#include "Input/KeyCodes.h"

namespace Alexio
{
	class Input
	{
	public:
		struct States
		{
			bool pressed = false;
			bool held = false;
			bool released = false;
		};
	private:
		static std::array<States, 256> keyStates;
		static std::array<bool, 256> newKeyState;
		static std::array<bool, 256> oldKeyState;

		static std::array<States, 8> mouseButtonStates;
		static std::array<bool, 8> newMouseButtonState;
		static std::array<bool, 8> oldMouseButtonState;
	public:
		static std::unordered_map<size_t, int8_t> mapKeys;
	public:
		static void SetKeyCodes();
		static void Scan();
		static void UpdateKeyState(int32_t keycode, bool state);
		static void UpdateMouseState(int32_t mousebutton, bool state);

		inline static States GetKeyState(int keycode) { return keyStates[keycode]; }

		static bool KeyPressed(int32_t keycode) { return keyStates[keycode].pressed; }
		static bool KeyHeld(int32_t keycode) { return keyStates[keycode].held; }
		static bool KeyReleased(int32_t keycode) { return keyStates[keycode].released; }

		static inline bool MouseButtonPressed(int32_t mb) { return mouseButtonStates[mb].pressed; }
		static inline bool MouseButtonHeld(int32_t mb) { return mouseButtonStates[mb].held; }
		static inline bool MouseButtonReleased(int32_t mb) { return mouseButtonStates[mb].released; }
	};
}