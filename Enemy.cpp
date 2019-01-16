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
	path_->clear();
}

Enemy::Enemy(const LPCSTR name) : Character(name), path_counter_(0)
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

// attack target
void Enemy::attack(Character *target)
{
	target->inflict(damage); // inflict damage to the target
}

void Enemy::chase_target(GameObject* target, VGTime *timer)
{ 
	const auto player = typeid(*target).name() == typeid(Player).name(); // compare type of target to player
	
	if (!player) return; // do not continue if target is not a player

	const auto target_position = target->get_transform()->get_local_position(); // get target position
	const auto position = transform_->get_local_position(); // get position of enemy
	if (dist(position, target_position) < chase_radius) 
	{
		const auto direction = target->get_transform()->get_local_position() - transform_->get_local_position();
		const auto fixed_direction = XMVectorSet(direction.x, 0, direction.z, 0); // chase player moving only horizontally on the x and z axis
		// if the enemy has recovered, it can try chasing the player
		if (timer->total_time() - time_since_collision_ >= time_to_recover_)
		{
			// if collided with target or the distance is less than the radius to prevent stuttering
			if (translate(fixed_direction, timer->delta_time()* chase_speed) || dist(position, target_position) <=  sphere_collider_->get_radius()) 
			{
				time_since_collision_ = timer->total_time(); // reset time when the collision happened
				attack(dynamic_cast<Character*>(target)); // attack the player
			}
		}else
		{
			step_back(timer, -fixed_direction); // step back as long as the recovery time for the enemy says
		}
	}
	else
	{
		// patrol along the given path if no player is near
		patrol(timer);
	}
}

void Enemy::update(VGTime* timer)
{
	Character::update(timer);
	// loop through all children of parent, which should always be the scene root 
	for (auto& i : parent_->get_children())
	{
		chase_target(i, timer); // chase child target
	}

}

// set the patrol path for the enemy to follow
void Enemy::set_path(vector<XMVECTOR> *path)
{
	path_ = path; 
}

// override check_collision to add enemy to enemy collision and its behaviour 
bool Enemy::check_collision(GameObject* target)
{
	const auto collided = GameObject::check_collision(target);
	// if collision occured and the target is an enemy
	if (typeid(*target).name() == typeid(Enemy).name() && collided)
	{		
		// move back in the opposite direction the other enemy is
		translate(transform_->get_local_position() - target->get_transform()->get_local_position(), target->get_sphere_collider()->get_radius() + sphere_collider_->get_radius()); 
		return collided;
	}
	return collided;
}

void Enemy::step_back(VGTime* timer, const XMVECTOR direction)
{
	// step back while the enemy is not recovered
	if (timer->total_time() - time_since_collision_ < time_to_recover_)
	{
		translate(direction, timer->delta_time() * speed_); // translate to the direction 
	}
	else
	{
		time_since_collision_ = 0; // reset collision timer when enemy recovered
	}
}

void Enemy::patrol(VGTime* timer)
{
	// follow the path to the next node until it reaches the limit distance to switch to the next node
	if(dist(path_->at(path_counter_),transform_->get_local_position()) <= switch_limit_)
	{
		path_counter_++; // node counter
		if (path_counter_ >= path_->size() - 1) // ensure the node counter does not stack overflow
			path_counter_ = 0;
	}
	translate(path_->at(path_counter_) - transform_->get_local_position(), timer->delta_time() * chase_speed); // translate towards the current node
}



