#pragma once
#include "GameObject.h"
#include "Collectable.h"

class Character : protected GameObject
{
	enum State {
		Standing,
		Crouching,
		Moving,
		Jumping,
		Accelerating,
		Hooking
	};

	struct stats {
		int kills;
		int deaths;
		int score;
	};

public:
	Character();
	~Character();

	Character(int life, State state);
	void cut_target(Character target);
	void throw_at(Character target);

private:
	int life;
	Collectable collected;




};

