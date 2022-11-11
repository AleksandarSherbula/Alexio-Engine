#include "aio_pch.h"
#include "Input.h"

#include "Core/Engine.h"

namespace Alexio
{
	bool Input::oldKeyState[1024] = { false };
	bool Input::newKeyState[1024] = { false };
	Input::States Input::keyStates[1024] = { false };

	bool Input::oldMouseButtonState[8] = { false };
	bool Input::newMouseButtonState[8] = { false };
	Input::States Input::mouseButtonStates[8] = { false };

	void Input::Scan()
	{
		auto scan = [](Input::States& state, bool& oldState, bool& newState)
		{
			state.pressed = false;
			state.released = false;

			if (newState != oldState)
			{
				if (newState)
				{
					state.pressed = !state.held;
					state.held = true;
				}
				else
				{
					state.released = true;
					state.held = false;
				}
			}

			oldState = newState;
		};

		for (int i = 0; i < 1024; i++)
			scan(keyStates[i], oldKeyState[i], newKeyState[i]);

		for (int i = 0; i < 8; i++)
			scan(mouseButtonStates[i], oldMouseButtonState[i], newMouseButtonState[i]);
	}

	void Input::UpdateKeyState(int keycode, bool state)
	{
		newKeyState[keycode] = state;
	}

	void Input::GetKeyCodes()
	{

	}
}