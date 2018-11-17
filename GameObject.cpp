#include "GameObject.h"



GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

GameObject::GameObject(std::string name, Transform transform, Model model, float collision_radius)
{
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

void GameObject::move(XMVECTOR position)
{
	transform.position = position;
}

void GameObject::collided(XMVECTOR target)
{

}
