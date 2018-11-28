#pragma once
#include "GameObject.h"
#include "Collectable.h"


#ifndef CHARACTER_H
#define CHARACTER_H

enum State {
	Standing,
	Crouching,
	Moving,
	Airborne,
	Accelerating,
	Hooking
};

struct stats {
	int kills;
	int deaths;
	int score;
};

class Character : public GameObject
{

public:
	Character();
	Character(std::string name);
	Character(std::string name, Transform transform, Model model, float collision_radius, int life, State state);
	~Character();

	Character(int life, State state);
	void cut_target(Character target);
	void throw_at(Character target);

	State get_State();


protected:
	int life;
	Collectable collected;
	State state = Standing;



};

#endif