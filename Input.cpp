#include "Input.h"



Input::Input()
{
}


Input::~Input()
{
}

Input::Input(Player * actor)
{
	this->actor = actor;
}

void Input::handle_input(WPARAM key)
{
	State state = actor->get_State();
	switch (key)
	{
		// represets move forward
	case 0x57:

		break;
		// represets move left
	case 0x41:
		break;
		// represets move back
	case 0x53:
		break;
		// represets move right
	case 0x44:
		break;
		// represents jump
	case VK_SPACE:
		break;
		// represents crouch
	case VK_LCONTROL:
		break;
	}
}
