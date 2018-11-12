#pragma once
#include "Input.h";
class Player
{
public:
	Player();
	~Player();

	void update_input();

private:
	Input* input;
};

