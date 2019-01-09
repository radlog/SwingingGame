#include "Enemy.h"

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
	auto direction =  target->transform.get_local_position() - transform.get_local_position();
	translate(direction , timer->delta_time()* 10.0f);
}



