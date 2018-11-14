#pragma once
#include "Character.h"
#include "Input.h";
#include <string>
class Input;


class Player : public Character
{
public:
	Player();
	~Player();

	Player(std::string name);

	void update_input();

	void update();

private:
	Input* input;
};

