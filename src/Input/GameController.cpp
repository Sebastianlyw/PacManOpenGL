#include "GameController.h"
#include <GLFW/glfw3.h>

GameController::GameController() :mMouseMovedAction(nullptr)
{

}

InputAction GameController::GetActionForKey(InputKey key)
{
	for (const auto& buttonAction : mButtonActions)
	{
		if (key == buttonAction.key)
		{
			return buttonAction.action;
		}
	}

	return [](uint32_t, InputState) {};
}

void GameController::AddInputActionForKey(const ButtonAction& buttonAction)
{
	mButtonActions.push_back(buttonAction);
}

void GameController::ClearAll()
{
	mButtonActions.clear();
}

bool GameController::IsPressed(InputState state)
{
	return state == GLFW_PRESS;
}

bool GameController::IsReleased(InputState state)
{
	return state == GLFW_RELEASE;
}

InputKey GameController::ActionKey()
{
	return static_cast<InputKey>(GLFW_KEY_A);
}

InputKey GameController::CancelKey()
{
	return static_cast<InputKey>(GLFW_KEY_S);
}

InputKey GameController::LeftKey()
{
	return static_cast<InputKey>(GLFW_KEY_LEFT);
}

InputKey GameController::RightKey()
{
	return static_cast<InputKey>(GLFW_KEY_RIGHT);
}

InputKey GameController::DownKey()
{
	return static_cast<InputKey>(GLFW_KEY_DOWN);
}

InputKey GameController::UpKey()
{
	return static_cast<InputKey>(GLFW_KEY_UP);
}


MouseInputAction GameController::GetMouseButtonActionForMouseButton(MouseButton button)
{
	for (const auto& buttonAction : mMouseButtonActions)
	{
		if (button == buttonAction.mouseButton)
		{
			return buttonAction.mouseInputAction;
		}
	}

	return [](InputState state, const MousePosition& pos) {};
}

void GameController::AddMouseButtonAction(const MouseButtonAction& mouseButtonAction)
{
	mMouseButtonActions.push_back(mouseButtonAction);
}

MouseButton GameController::LeftMouseButton()
{
	return static_cast<MouseButton>(GLFW_MOUSE_BUTTON_LEFT);
}

MouseButton GameController::RightMouseButton()
{
	return static_cast<MouseButton>(GLFW_MOUSE_BUTTON_RIGHT);
}
