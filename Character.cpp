#include "Character.h"



Character::Character()
{

}

Character::Character(LPCSTR name, LPCSTR tag, bool fps) : GameObject(name, tag)
{

}

Character::Character(LPCSTR name, Transform transform, Model model, float collision_radius, int life, state state, LPCSTR tag, bool fps) : Character(name, tag, fps)
{
}


Character::~Character()
{
}

Character::Character(int life, state state, LPCSTR name, LPCSTR tag, bool fps) : Character(name, tag, fps)
{
}

void Character::cut_target(Character target) const
{
	if(target.get_state() == airborne && state_ == airborne)
	{
		
	}
}

void Character::throw_at(Character target) const
{
	
}

state Character::get_state() const
{
	return this->state_;
}

void Character::spawn(XMVECTOR position)
{
}

void Character::render()
{
}

void Character::start()
{
}

void Character::update(VGTime timer)
{
}

void Character::set_speed_buff(float speed, float time)
{
	speed_ = speed;
}

void Character::set_stat_kills(int kills)
{
}

void Character::set_stat_deaths(int deaths)
{
}

void Character::set_stat_score(int score)
{
}

Stats Character::get_stats()
{
	return stats_;
}




