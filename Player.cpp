#include "Player.h"



Player::Player()
{
}


Player::~Player()
{
}

Player::Player(LPCSTR name, LPCSTR tag, bool fps) : Character(name, tag, fps)
{
	//if (fps)
}



void Player::update_input() const
{

}

void Player::update(VGTime timer) {
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



LPCSTR Player::get_name()
{
	return name_;
}

void Player::cleanup()
{
}
