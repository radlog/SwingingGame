#include "Character.h"



Character::Character()
{
	
}

Character::Character(LPCSTR name)
{
	GameObject::GameObject(name);
}

Character::Character(LPCSTR name, Transform transform, Model model, float collision_radius, int life, State state)
{
}


Character::~Character()
{
}

Character::Character(int life, State state)
{
}

void Character::cut_target(Character target)
{
}

void Character::throw_at(Character target)
{
}

State Character::get_State()
{
	return this->state;
}
