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

GameObject::GameObject(std::string name, Transform transform, Model model, float collision_radius)
{
}

void GameObject::spawn(Vector3 position)
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

void GameObject::move(Vector3 position)
{
}

void GameObject::collided(Vector3 target)
{
}

std::string GameObject::get_name()
{
	return name;
}
