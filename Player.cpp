#include "Player.h"

const XMVECTOR camera_offset = XMVectorSet(0,30,0,0);


Player::Player()
{
}


Player::~Player()
{
}


Player::Player(const LPCSTR name, const bool fps) : Character(name)
{
	init_cameras(fps);
}

Player::Player(LPCSTR name, Model* model, const Transform transform, const bool fps) : Character(name, model, transform)
{
	if (model == nullptr) model_ = new Model("assets/crate.jpg"); // 
	init_cameras(fps);
}



void Player::update_input(VGTime *timer)
{
	dx_handle_->input->update_input(this, timer);
}


void Player::update(VGTime *timer) {
	Character::update(timer);
	update_input(timer);
	update_camera();
}


void Player::update_camera() const
{
	// TODO:: update map transform optionally
	fps_camera_->transform = transform;
	//fps_camera_->transform.set_world_position(transform.get_local_position() + camera_offset);
}


Camera* Player::get_fps_camera() const
{
	return fps_camera_;
}

Camera* Player::get_top_down_camera() const
{
	return top_down_camera_;
}

void Player::init_cameras(bool fps)
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
	sphere_collider_ = new SphereCollider(transform.get_local_position(), 0.5f);
}


