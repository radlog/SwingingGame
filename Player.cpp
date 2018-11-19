#include "Player.h"



Player::Player()
{
}


Player::~Player()
{
	input = new Input(this);
}

Player::Player(std::string name)
{
	Character::Character(name);
}

void Player::update_input()
{

}

void Player::update() {
	GameObject::update();
	update_input();
}
