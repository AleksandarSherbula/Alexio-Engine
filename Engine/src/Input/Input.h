#ifndef INPUT_H
#define INPUT_H

#include "..\Window\Window.h"

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
		static States keyStates[1024];
		static bool newKeyState[1024];
		static bool oldKeyState[1024];

		static States mouseButtonStates[8];
		static bool newMouseButtonState[8];
		static bool oldMouseButtonState[8];
	public:
		static void GetKeyCodes();
		static void Scan();
		static void UpdateKeyState(int keycode, bool state);

		inline static States GetKeyState(int keycode) { return keyStates[keycode]; }

		static bool KeyPressed(int keycode) { return keyStates[keycode].pressed; }
		static bool KeyHeld(int keycode) { return keyStates[keycode].held; }
		static bool KeyReleased(int keycode) { return keyStates[keycode].released; }

		static bool GetMouseButtonPressed(int mb) { mouseButtonStates[mb].pressed; }
		static bool GetMouseButtonHeld(int mb) { mouseButtonStates[mb].held; }
		static bool GetMouseButtonReleased(int mb) { mouseButtonStates[mb].released; }
	};

}


#endif // !INPUT_H