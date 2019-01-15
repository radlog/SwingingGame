#include "Enemy.h"

const float chase_radius = 10.0f;
const float chase_speed = 10.0f;

Enemy::Enemy()
{
}


Enemy::~Enemy()
{
}

Enemy::Enemy(LPCSTR name) : Character(name)
{
}

Enemy::Enemy(LPCSTR name, Model* model, Transform transform) : Character(name, model, transform)
{
}

void Enemy::attack(Character target) const
{
}

void Enemy::chase_target(GameObject* target, VGTime *timer)
{
	const auto target_position = target->transform.get_local_position();
	const auto position = transform.get_local_position();
	if (dist(position, target_position) < chase_radius)
	{
		const auto direction = target->transform.get_local_position() - transform.get_local_position();
		const auto fixed_direction = XMVectorSet(direction.x, 0, direction.z, 0);
		translate(fixed_direction, timer->delta_time()* chase_speed);
	}
}

void Enemy::update(VGTime* timer)
{
	Character::update(timer);
	//chase_target()
}



