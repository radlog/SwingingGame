#pragma once
#include "Command.h"
#include "Input.h"

class InputHandler
{
public:
	InputHandler();
	~InputHandler();

private:
	Command* w_button_;
	Command* a_button_;
	Command* s_button_;
	Command* d_button_;
	Command* c_button_;
	Command* jump_button_;
	Command* shoot_button_;
	Command* hook_button_;

	Input input_;
};

