#include "GameObject.h"
#include "Physics3D.h"
#include "Player.h"


GameObject::~GameObject()
{
}

GameObject::GameObject()
{
}

GameObject::GameObject(const LPCSTR name, const TAG tag) : parent_(nullptr), model_(nullptr), sphere_collider_(nullptr), mesh_collider_(nullptr)
{
	transform_ = new Transform();
	dx_handle_ = D3Dfw::get_instance();
	device_ = dx_handle_->get_device();
	immediate_context_ = dx_handle_->get_immediate_context();
	name_ = name;
	tag_ = tag;
}

GameObject::GameObject(const LPCSTR name, Model *model, Transform *transform, const TAG tag) : GameObject(name, tag)
{
	this->transform_ = transform;
	model_ = model;

	const auto sc = model->get_bounding_sphere();
	const auto mc = model->get_mesh_collider();
	if (sc != nullptr)
	{
		const auto s = sizeof(SphereCollider);
		sphere_collider_ = new SphereCollider();
		memcpy(sphere_collider_, model_->get_bounding_sphere(), s);
		sphere_collider_->set_world_position(sphere_collider_->get_origin() + transform->get_local_position());
	}
	if (mc != nullptr)
	{
		const auto s = sizeof(MeshCollider);
		mesh_collider_ = new MeshCollider();
		memcpy(mesh_collider_, model_->get_mesh_collider(), s);
		mesh_collider_->set_world_position(mesh_collider_->get_origin() + transform->get_local_position());
	}
}

void GameObject::update(VGTime *timer)
{
	if (!is_kinetic_)
	{
		if (!is_grounded_)
		{
			air_time_ += timer->delta_time();
			const auto falling_velocity = Physics3D::gravity * (air_time_*air_time_);
			translate(Transform::world_down, falling_velocity * timer->delta_time());
		}
	}

	for (auto& i : children_)
	{
		i->update(timer);
	}
}

void GameObject::draw(const XMMATRIX view_projection, const bool use_default_cb, const D3D11_PRIMITIVE_TOPOLOGY mode)
{
	if (model_) model_->draw(transform_->get_world()* view_projection, use_default_cb, mode);
	for (auto& i : children_)
	{
		i->draw(view_projection, use_default_cb, mode);
	}
}

bool GameObject::translate(XMVECTOR direction, const float speed)
{
	transform_->translate(direction, speed);

	auto identity = XMMatrixIdentity();

	if (parent_)
	{
		parent_->update_collision_tree(&identity, 1.0f);

		if (check_collision(parent_))
		{
			if (is_grounded_)
				direction += Transform::world_down;
			transform_->translate(-direction, speed * push_back_speed);
			update_transform(&XMMatrixIdentity());
			return true;
		}
	}
	is_grounded_ = false;
	update_transform(&XMMatrixIdentity());
	return false;
}

void GameObject::rotate_fixed(const float pitch, const float yaw, const float roll)
{
	transform_->rotate_fixed(pitch, yaw, roll);
	for (auto& i : children_)
	{
		i->rotate_fixed(pitch, yaw, roll);
	}
}

void GameObject::rotate(const float pitch, const float yaw, const float roll)
{
	transform_->rotate(pitch, yaw, roll);
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


SphereCollider* GameObject::get_sphere_collider()
{
	return sphere_collider_;
}

MeshCollider* GameObject::get_mesh_collider()
{
	return mesh_collider_;
}

TAG GameObject::get_tag() const
{
	return tag_;
}

bool GameObject::check_collision(GameObject* target)
{
	//if (tag_ == GROUND)
	//{

	//}
	if (target == this || sphere_collider_ == nullptr) return false;

	//is_grounded_ = false;
	const auto sc = target->get_sphere_collider();
	if (sphere_collider_ && sc)
	{
		if (sphere_collider_->check_collision(sc))
		{
			const auto mc = target->get_mesh_collider();
			if (mc == nullptr)
				return false;
			if (sphere_collider_->check_collision(mc))
			{
				set_grounded(target->transform_->get_local_position().y + target->get_sphere_collider()->get_radius() - 1 < transform_->get_local_position().y);
				if (target->get_tag() == LAVA && typeid(*this).name() == typeid(Player).name()) static_cast<Player*>(this)->die();
				//OutputDebugString(name_);
				//OutputDebugString("\n");
				//OutputDebugString(target->get_name());
				//OutputDebugString("\n");
				return true;
			}
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

Transform* GameObject::get_transform() const
{
	return transform_;
}


void GameObject::set_transform(Transform *transform)
{
	transform_= transform;
}


void GameObject::update_transform(XMMATRIX *world)
{
	auto local_world = transform_->get_world() * * world;
	transform_->set_world(local_world);

	// update transform of children
	for (auto& i : children_)
	{
		i->update_transform(&local_world);
	}
}

void GameObject::update_collision_tree(XMMATRIX* world, const float scale)
{
	transform_->set_world_scale(transform_->get_local_scale() * scale);
	auto local_world = transform_->get_world();
	local_world *= *world;

	if (sphere_collider_) sphere_collider_->set_world_position(sphere_collider_->get_origin() + transform_->get_local_position());
	if (mesh_collider_) mesh_collider_->set_world_position(mesh_collider_->get_origin() + transform_->get_local_position());

	for (auto& i : children_)
	{
		i->update_collision_tree(&local_world, scale);
	}
}

void GameObject::update_constant_buffer_time_scaled(const XMMATRIX world_view_projection,
	const XMMATRIX view_projection, const XMVECTOR directional_light_vector, const XMVECTOR directional_light_color,
	const XMVECTOR ambient_light_color, const float game_time)
{
	const auto local_world = XMMatrixTranspose(transform_->get_local_world()) * world_view_projection;
	transform_->set_world(local_world);

	if (model_)
	{
		model_->update_constant_buffer_time_scaled(local_world,
			view_projection, directional_light_vector, directional_light_color,
			ambient_light_color, game_time);
	}

	for (auto& i : children_)
	{
		i->update_constant_buffer_time_scaled(local_world,
			view_projection, directional_light_vector, directional_light_color,
			ambient_light_color, game_time);
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

void GameObject::cleanup()
{
	model_->cleanup();
}
