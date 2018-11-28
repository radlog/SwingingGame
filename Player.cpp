#include "Player.h"



Player::Player()
{
}


Player::~Player()
{
}

Player::Player(std::string name): Character(name)
{
	
}

void Player::update_input()
{

}

void Player::update() {
	GameObject::update();
	update_input();
}
