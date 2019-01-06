#include "Player.h"



Player::Player()
{
}


Player::~Player()
{
}

Player::Player(const LPCSTR name,const bool fps) : Character(name)
{

}

Player::Player(LPCSTR name, Model* model, Transform transform, bool fps) : Character(name, model, transform)
{
	if (model == nullptr) model_ = new Model("assets/crate.jpg");
	if (fps)
	{
		camera_ = new Camera();
	}
}


void Player::update_input() const
{

}

void Player::update(const VGTime timer) {
	GameObject::update(timer);
	update_input();
}


