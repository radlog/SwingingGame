#include "GameObject.h"
#include "Physics3D.h"


GameObject::~GameObject()
{
}

GameObject::GameObject()
{
}

GameObject::GameObject(const LPCSTR name, const TAG tag) : model_(nullptr)
{
	dx_handle_ = D3Dfw::get_instance();
	device_ = dx_handle_->get_device();
	immediate_context_ = dx_handle_->get_immediate_context();
	name_ = name;
	tag_ = tag;
}

GameObject::GameObject(const LPCSTR name, Model *model, const Transform transform, const TAG tag) : GameObject(name, tag)
{
	this->transform = transform;
	model_ = model;	
}

void GameObject::update(VGTime timer)
{
	if (!is_kinetic_)
	{
		if (!is_grounded_)
		{
			air_time_ += timer.delta_time();
			const auto falling_velocity = Physics3D::gravity * (air_time_*air_time_);
			transform.up(falling_velocity * timer.delta_time());
		}
		else
		{
			air_time_ = 0.0f;
		}
	}
	for (auto& i : children_)
	{
		i->update(timer);
	}

}

void GameObject::draw(const XMMATRIX view_projection, const bool use_default_cb, const D3D11_PRIMITIVE_TOPOLOGY mode)
{
	if (model_) model_->draw(transform.get_world()* view_projection, use_default_cb, mode);
	for (auto& i : children_)
	{
		i->draw(view_projection, use_default_cb, mode);
	}
}

void GameObject::translate(XMVECTOR direction, float speed)
{
	transform.translate(direction, speed);
	for (auto& i : children_)
	{
		i->translate(direction, speed);
	}
}

void GameObject::move_horizontal_forward(float speed)
{
	transform.horizontal_forward(speed);
	for (auto& i : children_)
	{
		i->move_horizontal_forward(speed);
	}
}

void GameObject::move_horizontal_backward(float speed)
{
	transform.horizontal_forward(-speed);
	for (auto& i : children_)
	{
		i->move_horizontal_backward(speed);
	}
}

void GameObject::move_forward(float speed)
{
	transform.forward(speed);
	for (auto& i : children_)
	{
		i->move_forward(speed);
	}
}

void GameObject::move_backward(float speed)
{
	transform.forward(-speed);
	for (auto& i : children_)
	{
		i->move_backward(speed);
	}
}

void GameObject::move_right(float speed)
{
	transform.right(speed);
	for (auto& i : children_)
	{
		i->move_right(speed);
	}
}

void GameObject::move_left(float speed)
{
	transform.right(-speed);
	for (auto& i : children_)
	{
		i->move_left(speed);
	}
}

void GameObject::move_up(float speed)
{
	transform.up(speed);
	for (auto& i : children_)
	{
		i->move_up(speed);
	}
}

void GameObject::move_down(float speed)
{
	transform.up(-speed);
	for (auto& i : children_)
	{
		i->move_down(speed);
	}
}

void GameObject::rotate_fixed(float pitch, float yaw, float roll)
{
	transform.rotate_fixed(pitch, yaw, roll);
	for (auto& i : children_)
	{
		i->rotate_fixed(pitch, yaw, roll);
	}
}

void GameObject::rotate(float pitch, float yaw, float roll)
{
	transform.rotate(pitch, yaw, roll);
	for (auto& i : children_)
	{
		i->rotate(pitch, yaw, roll);
	}
}

vector<GameObject*> GameObject::get_children() const
{
	return children_;
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

bool GameObject::collided(GameObject target) const
{
	const auto s_tar_collider = target.get_model()->get_collision_sphere();
	const auto s_orig_collider = model_->get_collision_sphere();

	if (dist(s_tar_collider.local_position + target.transform.get_local_position(),
		s_orig_collider.local_position + transform.get_local_position()) >= s_tar_collider.collision_radius +
		s_orig_collider.collision_radius)
		return false;

	return true;

	switch (tag_)
	{
	case DEFAULT:

	case GROUND:

		break;
	case MODEL:

		break;
	case POLY:

		break;
	case CHARACTER:

		break;
	case PLAYER:

		break;
	default:
		return false;
	}

	return false;
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

void GameObject::add_child(GameObject* child)
{
	children_.push_back(child);
}

bool GameObject::remove_child(GameObject* child)
{
	for (auto i = 0; i < children_.size(); i++)
	{
		if (child == children_[i])
		{
			children_.erase(children_.begin() + i);
			return true;
		}
		if (children_[i]->remove_child(child)) return true;
	}
	return false;
}

void GameObject::set_grounded(bool grounded)
{
	is_grounded_ = grounded;
}

void GameObject::cleanup()
{
	model_->cleanup();
}
