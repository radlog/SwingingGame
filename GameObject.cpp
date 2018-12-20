#include "GameObject.h"
#include "Physics3D.h"

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
	if (!is_grounded)
	{
		air_time += timer.deltaTime();
		if (is_kinetic)
		{
			const double falling_velocity = Physics3D::gravity * (air_time*air_time);
			transform.up(falling_velocity * timer.deltaTime());
		}
	}else
	{		
		air_time = 0.0f;
	}
	
}

void GameObject::collided(XMVECTOR target)
{

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
