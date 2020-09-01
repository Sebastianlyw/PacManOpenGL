#include <GLFW/glfw3.h>
#include "InputController.h"
#include "GameController.h"

InputController::InputController() :mQuit(nullptr), mnoptrCurrentController(nullptr)
{

}

void InputController::Init(InputAction quitAction)
{
	mQuit = quitAction;
}

void InputController::Update(uint32_t dt)
{
}

void InputController::SetGameController(GameController* controller)
{
	mnoptrCurrentController = controller;
}
