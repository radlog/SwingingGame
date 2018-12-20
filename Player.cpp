#include "Player.h"



Player::Player()
{
}


Player::~Player()
{
}

Player::Player(LPCSTR name): Character(name)
{
	
}

void Player::update_input()
{

}

void Player::update(VGTime timer) {
	GameObject::update(timer);
	update_input();
}
