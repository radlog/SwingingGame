#pragma once
#include "Player.h";
#include "Command.h";
class Input
{
public:
	Input();
	~Input();

	Input(Player* actor);
	//void handle_input(KeyState key);

private:
	Player* actor;
	Command* w_button;
	Command* a_button;
	Command* s_button;
	Command* d_button;
	Command* space_button;
	Command* c_button;
	Command* jump_button;
	Command* shoot_button;
};

