#include "pch.h"
#include "Collider.h"


Collider::Collider() : origin_(XMVectorZero()), world_(XMVectorZero())
{
	world_ = origin_;
}


Collider::~Collider()
{
}

bool Collider::check_collision(Collider* col) 
{
	return false;
}


void Collider::set_world_position(const XMVECTOR world)
{
	world_ = world;
}

XMVECTOR Collider::get_origin() const
{
	return origin_;
}


