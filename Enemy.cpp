#include "Enemy.h"
#include "Player.h"
#include "MathHelper.h"

const double chase_radius = 20.0f;
const double chase_speed = 10.0f;
const int damage = 20;

Enemy::Enemy()
{
}


Enemy::~Enemy()
{
}

Enemy::Enemy(LPCSTR name) : Character(name), path_counter_(0)
{
}

Enemy::Enemy(const LPCSTR name, Model* model, Transform *transform) : Character(name, model, transform), path_counter_(0)
{
	const auto position = transform_->get_local_position();
	// initialize standard path
	path_ = new vector<XMVECTOR>
	{
		position + XMVectorSet(-10,0,0,0),
		position + XMVectorSet(10,0,0,0),
		position + XMVectorSet(10,0,-10,0),
		position + XMVectorSet(10,0,10,0)
	};
}

void Enemy::attack(Character *target) const
{
	target->inflict(damage);
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
				attack(dynamic_cast<Character*>(target));
			}
		}else
		{
			step_back(timer, -fixed_direction);
		}
	}
	else
	{
		patrol(timer);
	}
}

void Enemy::update(VGTime* timer)
{
	Character::update(timer);
	//patrol(timer);
	for (auto& i : parent_->get_children())
	{
		chase_target(i, timer);
	}

}

void Enemy::set_path(vector<XMVECTOR> *path)
{
	path_ = path;
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

void Enemy::patrol(VGTime* timer)
{
	if(dist(path_->at(path_counter_),transform_->get_local_position()) <= switch_limit_)
	{
		path_counter_++;
		if (path_counter_ >= path_->size() - 1)
			path_counter_ = 0;
	}
	translate(path_->at(path_counter_) - transform_->get_local_position(), timer->delta_time() * chase_speed);
}



