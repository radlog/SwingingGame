#include "Player.h"



Player::Player()
{
}


Player::~Player()
{
}

Player::Player(const LPCSTR name, const LPCSTR tag, const bool fps) : Character(name, tag, fps)
{
	//if (fps)
}



void Player::update_input() const
{

}

void Player::update(const VGTime timer) {
	GameObject::update(timer);
	update_input();
}

void Player::spawn(XMVECTOR position)
{
}

void Player::render()
{
}

void Player::start()
{
}


void Player::cleanup()
{
}
