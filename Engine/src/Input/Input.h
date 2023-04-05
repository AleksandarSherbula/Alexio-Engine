#pragma once

#include "Input/KeyCodes.h"

#include <glm/glm.hpp>

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

		inline static bool MouseButtonPressed(int32_t mb) { return mouseButtonStates[mb].pressed; }
		inline static bool MouseButtonHeld(int32_t mb) { return mouseButtonStates[mb].held; }
		inline static bool MouseButtonReleased(int32_t mb) { return mouseButtonStates[mb].released; }
		static glm::vec2 GetMousePosition();
	};
}