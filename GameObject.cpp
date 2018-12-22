#include "GameObject.h"
#include "Physics3D.h"
#include "MathHelper.h"

GameObject::~GameObject()
{
}

GameObject::GameObject()
{
}


GameObject::GameObject(LPCSTR name)
{
	dx_handle = d3dfw::getInstance();
	device = dx_handle->device;
	immediateContext = dx_handle->immediateContext;
	this->name = name;
}

GameObject::GameObject(LPCSTR name, Transform transform, Model model) : GameObject(name)
{
	this->name = name;
	this->transform = transform;
	this->model = model;
}


void GameObject::Draw(XMMATRIX view_projection, bool use_default_cb, D3D11_PRIMITIVE_TOPOLOGY mode)
{
	model.Draw(transform.world* view_projection, use_default_cb, mode);
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
	
	if (is_kinetic)
	{
		if (!is_grounded)
		{
			air_time += timer.deltaTime();
			const double falling_velocity = Physics3D::gravity * (air_time*air_time);
			transform.up(falling_velocity * timer.deltaTime());
		}
		else
		{
			air_time = 0.0f;
		}
	}
	
}

bool GameObject::collided(GameObject target)
{
	
	SphereCollider tar_collider = target.get_model()->getCollisionSphere();
	SphereCollider orig_collider = model.getCollisionSphere();
	
	return dist(tar_collider.localPosition * target.transform.local_position, orig_collider.localPosition * transform.local_position) < tar_collider.collisionRadius + orig_collider.collisionRadius;
}

LPCSTR GameObject::get_name()
{
	return name;
}

Model* GameObject::get_model()
{
	return &model;
}

void GameObject::Cleanup()
{
}
