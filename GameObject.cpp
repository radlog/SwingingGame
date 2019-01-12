#include "GameObject.h"
#include "Physics3D.h"


GameObject::~GameObject()
{
}

GameObject::GameObject()
{
}

GameObject::GameObject(const LPCSTR name, const TAG tag) : model_(nullptr), collider_(nullptr), parent_(nullptr)
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

	const auto col = model_->get_collider();
	if (col != nullptr) {
		auto col_id = typeid(*col).name();
		if (col_id == typeid(SphereCollider).name())
		{
			const auto s = sizeof(SphereCollider);
			malloc(s);
			collider_ = new Collider();
			memcpy(collider_, model_->get_collider(), s);
		}
		else
		{
			const auto s = sizeof(MeshCollider);
			malloc(s);
			collider_ = new Collider();
			memcpy(collider_, model_->get_collider(), s);
		}
	}
	if (collider_) collider_->set_world_position(collider_->get_origin() + transform.get_local_position());
}

void GameObject::update(VGTime *timer)
{
	if (!is_kinetic_)
	{
		if (!is_grounded_)
		{
			air_time_ += timer->delta_time();
			const auto falling_velocity = Physics3D::gravity * (air_time_*air_time_);
			translate(Transform::world_up, falling_velocity * timer->delta_time());
		}
	}

	update_transform(&XMMatrixIdentity());
	update_collision_tree(&XMMatrixIdentity(), transform.get_world_scale().x);
}

void GameObject::draw(const XMMATRIX view_projection, const bool use_default_cb, const D3D11_PRIMITIVE_TOPOLOGY mode)
{
	if (model_) model_->draw(transform.get_world()* view_projection, use_default_cb, mode);
	for (auto& i : children_)
	{
		i->draw(view_projection, use_default_cb, mode);
	}
}

void GameObject::translate(const XMVECTOR direction, const float speed)
{
	transform.translate(direction, speed);

	auto identity = XMMatrixIdentity();

	if (parent_)
	{
		parent_->update_collision_tree(&identity, parent_->transform.get_world_scale().x);

		if (check_collision(parent_))
		{
			transform.translate(-direction, speed * push_back_speed);
			return;
		}
	}

	for (auto& i : children_)
	{
		i->translate(direction, speed);
	}
	//if (collider_) collider_->set_world_position(collider_->get_origin() + transform.get_local_position());

}

void GameObject::rotate_fixed(const float pitch, const float yaw, const float roll)
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

void GameObject::set_kinetic(const bool kinetic)
{
	is_kinetic_ = kinetic;
	air_time_ = 0.0f;
}

bool GameObject::get_kinetic() const
{
	return is_kinetic_;
}


Collider* GameObject::get_collider()
{
	return collider_;
}

bool GameObject::check_collision(GameObject* target)
{

	if (target == this || collider_ == nullptr) return false;

	is_grounded_ = false;
	if (model_ && target->get_model())
	{

		if (collider_->check_collision(target->get_collider()))
		{
			set_grounded(target->transform.get_local_position().y < transform.get_local_position().y);
			return true;
		}


	}

	for (auto& i : target->get_children())
	{
		if (check_collision(i))
			return true;
	}

	for (auto& i : children_)
	{
		if (i->check_collision(target))
			return true;
	}


	return false;
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

void GameObject::update_transform(XMMATRIX *world)
{
	auto local_world = transform.get_local_world() * * world;
	transform.set_world(local_world);

	// update transform of children
	for (auto& i : children_)
	{
		i->update_transform(&local_world);
	}
}

void GameObject::update_collision_tree(XMMATRIX* world, const float scale)
{
	transform.set_world_scale(transform.get_local_scale() * scale);
	auto local_world = transform.get_world();
	local_world *= *world;

	if (collider_) collider_->set_world_position(collider_->get_origin() + transform.get_local_position());


	for (auto& i : children_)
	{
		i->update_collision_tree(&local_world, scale);
	}
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

void GameObject::set_parent(GameObject* parent)
{
	parent_ = parent;
}

void GameObject::remove_parent(GameObject* child)
{
	child->parent_ = nullptr;
}

void GameObject::add_child(GameObject* child)
{
	child->set_parent(this);
	children_.push_back(child);
}

bool GameObject::remove_child(GameObject *child)
{
	for (auto i = 0; i < children_.size(); i++)
	{
		if (child == children_[i])
		{
			children_.erase(children_.begin() + i);
			return true;
		}
		if (children_[i]->remove_child(child))
		{
			child->remove_parent(this);
			return true;
		}
	}
	return false;
}



void GameObject::set_grounded(bool grounded)
{
	is_grounded_ = grounded;
	if (grounded)
	{
		air_time_ = 0;
	}
}

bool GameObject::get_grounded() const
{
	return is_grounded_;
}


void GameObject::set_collider(Collider *col)
{
	collider_ = col;
}




void GameObject::cleanup()
{
	model_->cleanup();
}
