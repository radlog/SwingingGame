#include "GameObject.h"
#include "Physics3D.h"


GameObject::~GameObject()
{
}

GameObject::GameObject()
{
}


GameObject::GameObject(LPCSTR name, LPCSTR tag)
{
	dx_handle_ = D3Dfw::get_instance();
	device_ = dx_handle_->get_device();
	immediate_context_ = dx_handle_->get_immediate_context();
	this->name_ = name;
	this->tag_ = tag;
}

GameObject::GameObject(LPCSTR name, const Transform transform, Model *model, LPCSTR tag) : GameObject(name, tag)
{
	this->transform = transform;
	this->model_ = model;
}


void GameObject::draw(XMMATRIX view_projection, bool use_default_cb, D3D11_PRIMITIVE_TOPOLOGY mode)
{
	model_->draw(transform.world* view_projection, use_default_cb, mode);
}


void GameObject::spawn(XMVECTOR position)
{
}

void GameObject::render()
{
}

void GameObject::start()
{
}

void GameObject::update(VGTime timer)
{

	if (is_kinetic_)
	{
		if (!is_grounded_)
		{
			air_time_ += timer.deltaTime();
			const double falling_velocity = Physics3D::gravity * (air_time_*air_time_);
			transform.up(falling_velocity * timer.deltaTime());
		}
		else
		{
			air_time_ = 0.0f;
		}
	}

}


bool GameObject::collided(GameObject target) const
{
	const SphereCollider tar_collider = target.get_model()->get_collision_sphere();
	const SphereCollider orig_collider = model_->get_collision_sphere();

	return dist(tar_collider.local_position * target.transform.local_position, orig_collider.local_position * transform.local_position) < tar_collider.collision_radius + orig_collider.collision_radius;
}

LPCSTR GameObject::get_name() const
{
	return name_;
}

Model* GameObject::get_model()
{
	return model_;
}

void GameObject::cleanup()
{
	model_->cleanup();
}


LPCSTR GameObject::get_collision_type(COLLISION_TYPE collision_type)
{
	switch (collision_type)
	{
	case 0: return "default";
	case 1: return "ground";
	case 2: return "model";
	case 3: return "poly";
	default: return "default";
	}
}
