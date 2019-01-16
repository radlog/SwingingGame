#include "Character.h"


Character::Character()
{

}

Character::Character(const LPCSTR name) : GameObject(name), life_(max_life_),speed_(initial_speed_)
{

}

Character::Character(const LPCSTR name, Model *model, Transform *transform) : GameObject(name, model, transform), life_(max_life_)
{
}


Character::~Character()
{
}


void Character::cut_target(Character target) const
{
	if (target.get_state() == AIRBORNE && state_ == AIRBORNE)
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

void Character::set_state(const STATE state)
{
	state_ = state;
}

void Character::set_speed_buff(const float speed, float time)
{
	speed_ = speed;
}

// set kills of character
void Character::set_stat_kills(const int kills)
{
	stats_.kills = kills;
}

// set deaths of character
void Character::set_stat_deaths(const int deaths)
{
	stats_.deaths = deaths;
}

// set score of character
void Character::set_stat_score(const int score)
{
	stats_.score = score;
}

// get stats of the character
Stats Character::get_stats()
{
	return stats_;
}

// character dies
void Character::die()
{
	stats_.deaths ++;
	transform_->translate(spawn_position_); // re-spawn character at the initial re-spawn position	
	air_time_ = 0; // reset air_time
	life_ = max_life_; // set life back to max
}

// inflicts damage to the character, the boolean says if the character died or not
bool Character::inflict(const int dmg)
{
	life_ -= dmg;
	if (life_ <= 0)
	{
		die();
		return true;
	}
	return false;
}

void Character::update(VGTime* timer)
{
	GameObject::update(timer);
	if (transform_->get_local_position().y < ground_death_position_)
		die();
}

void Character::crouch(VGTime *timer)
{
}

void Character::jump(VGTime* timer)
{
	translate(Transform::world_up, timer->delta_time() * jump_speed_ );
}

int Character::get_life() const
{
	return life_;
}




