#pragma once
#include "GameObject.h"
#include "Collectable.h"

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

class Character : protected GameObject
{

public:
	Character();
	~Character();

	Character(int life, State state);
	void cut_target(Character target);
	void throw_at(Character target);

	State get_State();


private:
	int life;
	Collectable collected;
	State state = Standing;



};

