#pragma once
#include "Input.h";
class Player : public Character
{
public:
	Player();
	~Player();

	void update_input();

private:
	Input* input;
};

