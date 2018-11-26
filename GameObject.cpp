#include "GameObject.h"



GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

GameObject::GameObject(std::string name)
{
}

GameObject::GameObject(std::string name, Transform transform, Model model)
{
	this->name = name;
	this->transform = transform;
	this->model = model;

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
