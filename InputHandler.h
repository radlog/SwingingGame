#pragma once
#include "Command.h"
#include "Input.h"

class InputHandler
{
public:
	InputHandler();
	~InputHandler();

private:
	Command* w_button;
	Command* a_button;
	Command* s_button;
	Command* d_button;
	Command* c_button;
	Command* jump_button;
	Command* shoot_button;
	Command* hook_button;

	Input input;
};

