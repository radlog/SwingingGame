#include "GameObject.h"
#include "Physics3D.h"
#include "Player.h"


GameObject::~GameObject()
{
}

GameObject::GameObject()
{
}

// initialize gameobject with name, tag
GameObject::GameObject(const LPCSTR name, const TAG tag) : parent_(nullptr), model_(nullptr), sphere_collider_(nullptr), mesh_collider_(nullptr)
{
	transform_ = new Transform(); // initialize transform
	dx_handle_ = D3Dfw::get_instance(); // get directX instance
	device_ = dx_handle_->get_device(); // get device
	immediate_context_ = dx_handle_->get_immediate_context(); // get device context
	name_ = name; // set name
	tag_ = tag; // set tag
}

// initialize gameobject with name, model, transform, tag
GameObject::GameObject(const LPCSTR name, Model *model, Transform *transform, const TAG tag) : GameObject(name, tag)
{
	this->transform_ = transform; // initialize transform
	model_ = model; // set model

	const auto sc = model->get_bounding_sphere(); // get models bounding sphere
	const auto mc = model->get_mesh_collider();
	// copy data to sphere collider if model has one
	if (sc != nullptr)
	{
		const auto s = sizeof(SphereCollider);
		sphere_collider_ = new SphereCollider();
		memcpy(sphere_collider_, model_->get_bounding_sphere(), s);
		sphere_collider_->set_world_position(sphere_collider_->get_origin() + transform->get_local_position()); // set sphere collider world position using position of this object
	}
	// copy data to mesh collider if model has one
	if (mc != nullptr)
	{
		const auto s = sizeof(MeshCollider);
		mesh_collider_ = new MeshCollider();
		memcpy(mesh_collider_, model_->get_mesh_collider(), s);
		mesh_collider_->set_world_position(mesh_collider_->get_origin() + transform->get_local_position()); // set mesh collider world position using position of this object
	}
}

// update with game timer
void GameObject::update(VGTime *timer)
{
	// check if object is not kinetic
	if (!is_kinetic_)
	{
		// check if object is not grounded
		if (!is_grounded_)
		{
			air_time_ += timer->delta_time(); // accumulate air time
			const auto falling_velocity = Physics3D::gravity * (air_time_*air_time_); // common gravity equation to accumulate velocity
			translate(Transform::world_down, falling_velocity * timer->delta_time()); // translate down with gravity velocity
		}
	}

	// update all children
	for (auto& i : children_)
	{
		i->update(timer);
	}
}

// draw method that calls the draw of the model
void GameObject::draw(const XMMATRIX view_projection, const bool use_default_cb, const D3D11_PRIMITIVE_TOPOLOGY mode)
{
	if (model_) model_->draw(transform_->get_world()* view_projection, use_default_cb, mode); // call model draw
	for (auto& i : children_) // call all children to draw
	{
		i->draw(view_projection, use_default_cb, mode);
	}
}

// move object in a direction at given speed
bool GameObject::translate(XMVECTOR direction, const float speed)
{
	transform_->translate(direction, speed); // call transform translate

	auto identity = XMMatrixIdentity(); // identity matrix to update collision of the parent object which should be always the root

	// only go further if parent exists
	if (parent_)
	{
		parent_->update_collision_tree(&identity, transform_->get_local_scale().x); // update collision tree 

		// check collision with parent and all its children
		if (check_collision(parent_))
		{
			// if object touches the ground
			if (is_grounded_)
				direction += Transform::world_down; // add an down direction to prevent stuttering when ground is not even
			// use the direction and reverse it to undo the translation due to collision and push the object a bit back
			transform_->translate(-direction, speed * push_back_speed); 
			update_transform(&XMMatrixIdentity()); // update children transform
			return true;
		}
	}
	is_grounded_ = false; // grounded is always false, if there is no collision
	update_transform(&XMMatrixIdentity()); // update children transform
	return false;
}

// rotate object with fixed x angle along all three axis
void GameObject::rotate_fixed(const float pitch, const float yaw, const float roll)
{
	transform_->rotate_fixed(pitch, yaw, roll);
	for (auto& i : children_)
	{
		i->rotate_fixed(pitch, yaw, roll);
	}
}

// rotate object along all three axis
void GameObject::rotate(const float pitch, const float yaw, const float roll)
{
	transform_->rotate(pitch, yaw, roll);
	for (auto& i : children_)
	{
		i->rotate(pitch, yaw, roll);
	}
}

// get list of children
vector<GameObject*> GameObject::get_children() const
{
	return children_;
}

// set kinetic status
void GameObject::set_kinetic(const bool kinetic)
{
	is_kinetic_ = kinetic;
	air_time_ = 0.0f; // reset air time to prevent gravity to accumulate
}

// get kinetix status
bool GameObject::get_kinetic() const
{
	return is_kinetic_;
}

// get sphere collider
SphereCollider* GameObject::get_sphere_collider()
{
	return sphere_collider_;
}

// get mesh collider
MeshCollider* GameObject::get_mesh_collider()
{
	return mesh_collider_;
}

// get tag
TAG GameObject::get_tag() const
{
	return tag_;
}

bool GameObject::check_collision(GameObject* target)
{
	// only check collision, if the target is not itself and this target has a sphere collider
	if (target == this || sphere_collider_ == nullptr) return false; 

	const auto sc = target->get_sphere_collider(); // get target sphere collider
	// if target has a sphere collider 
	if (sc)
	{
		// if spheres collided with each other
		if (sphere_collider_->check_collision(sc))
		{
			const auto mc = target->get_mesh_collider(); // get target mesh collider
			if (mc == nullptr) // only proceed if mesh collider is not nullptr
				return false;
			// if sphere collider collided with mesh collider
			if (sphere_collider_->check_collision(mc)) 
			{
				// set grounded if the y position of this object is higher than the targets position y together with its radius
				set_grounded(target->transform_->get_local_position().y + target->get_sphere_collider()->get_radius() - 1 < transform_->get_local_position().y);
				// let this object die if its a player and the collided other object is LAVA
				if (target->get_tag() == LAVA && typeid(*this).name() == typeid(Player).name()) static_cast<Player*>(this)->die();
				return true;
			}
		}
	}

	// check collision for all targets' children
	for (auto& i : target->get_children())
	{
		if (check_collision(i))
			return true;
	}

	// check collision for all children
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

// set transform
void GameObject::set_transform(Transform *transform)
{
	transform_= transform;
}

// update transform for this object and all children
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

// update collision tree for this object and all children
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

// update time scaled constant buffer for this object and for all children
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

// get name
LPCSTR GameObject::get_name() const
{
	return name_;
}

// get model
Model* GameObject::get_model() const
{
	return model_;
}

// set parent
void GameObject::set_parent(GameObject* parent)
{
	parent_ = parent;
}

// remove parent
void GameObject::remove_parent(GameObject* child) const
{
	child->parent_ = nullptr;
}

// add a child to children list
void GameObject::add_child(GameObject* child)
{
	child->set_parent(this); // set parent of the child to this
	children_.push_back(child); // add child to children list
}

// remove child from children list
bool GameObject::remove_child(GameObject *child)
{
	// loop through all children
	for (auto i = 0; i < children_.size(); i++)
	{		
		// if this object holds the child
		if (child == children_[i])
		{
			children_.erase(children_.begin() + i); // erase child from list
			child->remove_parent(this); // remove this as parent
			return true;
		}
	}
	return false;
}

// set grounded status
void GameObject::set_grounded(bool grounded)
{
	is_grounded_ = grounded;
	if (grounded)
	{
		air_time_ = 0;
	}
}

// return grounded statuse
bool GameObject::get_grounded() const
{
	return is_grounded_;
}

// cleanup to avoid memory leaks
void GameObject::cleanup()
{
	model_->cleanup();
	for (auto& i : children_)
		i->cleanup();
}
