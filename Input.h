#pragma once
#include "Player.h";
#include "Command.h";
class Player;
class Input
{
public:
	Input();
	~Input();

	Input(Player* actor);
	void handle_input(WPARAM key);

private:
	Player* actor;
	Command* w_button;
	Command* a_button;
	Command* s_button;
	Command* d_button;
	Command* c_button;
	Command* jump_button;
	Command* shoot_button;
	Command* hook_button;

	WPARAM* forward;
	WPARAM left = 0x41;
	WPARAM back = 0x53;
	WPARAM right = 0x44;
	WPARAM crouch = VK_LCONTROL;
	WPARAM jump = VK_SPACE;
	//const WPARAM shoot = ;
	//const WPARAM hook =

};

