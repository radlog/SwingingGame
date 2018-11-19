#pragma once
#include "Character.h"
#include "Input.h";
<<<<<<< HEAD
#include "Character.h"
class Input;
=======
#include <string>
class Input;


>>>>>>> refs/remotes/origin/master
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

