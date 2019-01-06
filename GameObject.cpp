#include "GameObject.h"
#include "Physics3D.h"


GameObject::~GameObject()
{
}

GameObject::GameObject()
{
}


GameObject::GameObject(const LPCSTR name)
{
	dx_handle_ = D3Dfw::get_instance();
	device_ = dx_handle_->get_device();
	immediate_context_ = dx_handle_->get_immediate_context();
	this->name_ = name;
}

GameObject::GameObject(const LPCSTR name, Model *model, const Transform transform) : GameObject(name)
{
	this->transform = transform;
	this->model_ = model;
}


void GameObject::draw(const XMMATRIX view_projection, const bool use_default_cb, const D3D11_PRIMITIVE_TOPOLOGY mode)
{
	model_->draw(transform.get_world()* view_projection, use_default_cb, mode);
}

void GameObject::move_horizontal_forward(float speed)
{
	transform.horizontal_forward(speed);
}

void GameObject::move_horizontal_backward(float speed)
{
	transform.horizontal_forward(-speed);
}

void GameObject::move_forward(float speed)
{
	transform.forward(speed);
}

void GameObject::move_backward(float speed)
{
	transform.forward(-speed);
}

void GameObject::move_right(float speed)
{
	transform.right(speed);
}

void GameObject::move_left(float speed)
{
	transform.right(-speed);
}

void GameObject::move_up(float speed)
{
	transform.up(speed);
}

void GameObject::move_down(float speed)
{
	transform.up(-speed);
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
			air_time_ += timer.delta_time();
			const double falling_velocity = Physics3D::gravity * (air_time_*air_time_);
			transform.up(falling_velocity * timer.delta_time());
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

	return dist(tar_collider.local_position * target.transform.get_local_position(), orig_collider.local_position * transform.get_local_position()) < tar_collider.collision_radius + orig_collider.collision_radius;
}

LPCSTR GameObject::get_name() const
{
	return name_;
}

Model* GameObject::get_model() const
{
	return model_;
}

void GameObject::set_model(Model* model)
{
	model_ = model;
}

void GameObject::cleanup()
{
	model_->cleanup();
}
