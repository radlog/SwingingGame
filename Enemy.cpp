#include "Enemy.h"
#include "Player.h"

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

Enemy::Enemy(LPCSTR name, Model* model, Transform *transform) : Character(name, model, transform)
{
}

void Enemy::attack(Character target) const
{
}

void Enemy::chase_target(GameObject* target, VGTime *timer)
{
	auto a = typeid(*target).name();
	if (typeid(*target).name() != typeid(Player).name()) return;

	const auto target_position = target->get_transform()->get_local_position();
	const auto position = transform_->get_local_position();
	if (dist(position, target_position) < chase_radius)
	{
		const auto direction = target->get_transform()->get_local_position() - transform_->get_local_position();
		const auto fixed_direction = XMVectorSet(direction.x, 0, direction.z, 0);
		if (timer->total_time() - time_since_collision_ >= time_to_recover_)
		{
			if (translate(fixed_direction, timer->delta_time()* chase_speed) || dist(position, target_position) <=  sphere_collider_->get_radius())
			{
				time_since_collision_ = timer->total_time();
			}
		}else
		{
			step_back(timer, -fixed_direction);
		}
	}
}

void Enemy::update(VGTime* timer)
{
	//Character::update(timer);
	for (auto& i : parent_->get_children())
	{
		chase_target(i, timer);
	}
}

void Enemy::step_back(VGTime* timer, const XMVECTOR direction)
{
	if (timer->total_time() - time_since_collision_ < time_to_recover_)
	{
		translate(direction, timer->delta_time() * chase_speed);
	}
	else
	{
		time_since_collision_ = 0;
	}
}



