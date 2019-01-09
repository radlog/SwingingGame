#include "Character.h"


Character::Character()
{

}

Character::Character(LPCSTR name) : GameObject(name)
{

}

Character::Character(LPCSTR name, Model *model, Transform transform) : GameObject(name, model, transform)
{
}


Character::~Character()
{
}


void Character::cut_target(Character target) const
{
	if(target.get_state() == AIRBORNE && state_ == AIRBORNE)
	{
		
	}
}

void Character::throw_at(Character target) const
{
	
}

STATE Character::get_state() const
{
	return this->state_;
}

void Character::set_speed_buff(float speed, float time)
{
	speed_ = speed;
}

// set kills of character
void Character::set_stat_kills(int kills)
{
}

// set deaths of character
void Character::set_stat_deaths(int deaths)
{
}

// set score of character
void Character::set_stat_score(int score)
{
}

// get stats of the character
Stats Character::get_stats()
{
	return stats_;
}

// character dies
void Character::die()
{
	stats_.deaths += 1;
}

// inflicts damage to the character, the boolean says if the character died or not
bool Character::inflict(float dmg)
{
	if (life_ -= dmg <= 0)
	{
		die();
		return true;
	}
	return false;
}




