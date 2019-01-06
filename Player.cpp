#include "Player.h"




Player::Player()
{
}


Player::~Player()
{
}


Player::Player(const LPCSTR name, const bool fps) : Character(name)
{

}

Player::Player(LPCSTR name, Model* model, const Transform transform, const bool fps) : Character(name, model, transform)
{
	if (model == nullptr) model_ = new Model("assets/crate.jpg"); // 
	if (fps)
	{
		camera_ = new Camera();
		camera_->transform = transform;
		children_.push_back(camera_);
	}
}



void Player::update_input(VGTime timer) 
{
	dx_handle_->input->update_input(this, &timer);
}


void Player::update(VGTime timer) {
	GameObject::update(timer);
	update_input(timer);
	update_camera();
}


void Player::update_camera() const
{
	camera_->transform = transform;
}


Camera* Player::get_camera() const
{
	return camera_;
}


