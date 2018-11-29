#include "GameObject.h"



GameObject::GameObject()
{
	dx_handle = d3dfw::getInstance();
	device = dx_handle->device;
	immediateContext = dx_handle->immediateContext;
}


GameObject::~GameObject()
{
}

GameObject::GameObject(std::string name)
{
	dx_handle = d3dfw::getInstance();
	this->name = name;
}

GameObject::GameObject(std::string name, Transform transform, Model model) : GameObject(name)
{
	this->name = name;
	this->transform = transform;
	this->model = model;
}

void GameObject::Draw(XMMATRIX view_projection)
{
	model.Draw(transform.world * view_projection);
}

void GameObject::Draw(XMMATRIX view_projection, D3D11_PRIMITIVE_TOPOLOGY mode)
{
	model.Draw(transform.world* view_projection, mode);
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

void GameObject::update()
{
}

void GameObject::collided(XMVECTOR target)
{

}

std::string GameObject::get_name()
{
	return name;
}

void GameObject::Cleanup()
{
}
