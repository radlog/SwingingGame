#pragma once
#include "Character.h"
#include "Input.h";
<<<<<<< HEAD
#include "Character.h"
class Input;
=======
#include <string>
class Input;


>>>>>>> cbb1e29bc125ff8c4a9d230abb164da70f956360
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

