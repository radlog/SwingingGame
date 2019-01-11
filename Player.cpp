#include "Player.h"




Player::Player()
{
}


Player::~Player()
{
}


Player::Player(const LPCSTR name, const bool fps) : Character(name)
{
	if (fps)
	{
		fps_camera_ = new Camera();
		fps_camera_->transform = transform;
		children_.push_back(fps_camera_);
	}
	top_down_camera_ = new Camera();
	top_down_camera_->transform = Transform(XMVectorSplatOne(), XMQuaternionIdentity(), XMVectorSet(0, 100, 0, 0));
	top_down_camera_->rotate(XMConvertToRadians(90), 0, 0);
	is_kinetic_ = true;
}

Player::Player(LPCSTR name, Model* model, const Transform transform, const bool fps) : Character(name, model, transform)
{
	if (model == nullptr) model_ = new Model("assets/crate.jpg"); // 
	if (fps)
	{
		fps_camera_ = new Camera();
		fps_camera_->transform = transform;
		children_.push_back(fps_camera_);
	}
	top_down_camera_ = new Camera();
	top_down_camera_->transform = Transform(XMVectorSplatOne(), XMQuaternionIdentity(), XMVectorSet(0, 100, 0, 0));
	top_down_camera_->rotate(XMConvertToRadians(90), 0, 0);
	is_kinetic_ = true;
}



void Player::update_input(VGTime *timer)
{
	dx_handle_->input->update_input(this, timer);
}


void Player::update(VGTime *timer) {
	GameObject::update(timer);
	update_input(timer);
	update_camera();
}


void Player::update_camera() const
{
	// TODO:: update map transform optionally
	fps_camera_->transform = transform;
}


Camera* Player::get_fps_camera() const
{
	return fps_camera_;
}

Camera* Player::get_top_down_camera() const
{
	return top_down_camera_;
}


