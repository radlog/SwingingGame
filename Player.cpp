#include "Player.h"

#if DEBUG
const XMVECTOR camera_offset = XMVectorSet(0,30,0,0); 
#endif

const XMVECTOR top_down_distance = XMVectorSet(0, 50, 0, 0);
const float player_col_radius = 0.15f;
const float rotation_down = XMConvertToRadians(90);

Player::Player()
{
}


Player::~Player()
{
}

// initialize player with name, first person view flag
Player::Player(const LPCSTR name, const bool fps) : Character(name)
{
	init_cameras(fps); // initialize cameras
}

// initialize player with name, model, transform, first person view flag
Player::Player(const LPCSTR name, Model* model, Transform *transform, const bool fps) : Character(name, model, transform)
{
	if (model == nullptr) model_ = new Model(); // create new model if no model was set
	init_cameras(fps); // initialize cameras
}

// update input
void Player::update_input(VGTime *timer)
{
	dx_handle_->input->update_input(this, timer); // use directX instance to update input with actor set as player
}

// update player
void Player::update(VGTime *timer) {
	Character::update(timer); // call character update
	update_input(timer); // update input
	update_camera(); // update camera position
}

// update camera transform
void Player::update_camera() const
{
	fps_camera_->set_transform(transform_); // set transform to players position
}

// get first person camera
Camera* Player::get_fps_camera() const
{
	return fps_camera_;
}

// get top down camera
Camera* Player::get_top_down_camera() const
{
	return top_down_camera_;
}

// initialize cameras
void Player::init_cameras(const bool fps)
{
	// if first person flag is set
	if (fps)
	{
		fps_camera_ = new Camera(); // create first person camera
		fps_camera_->set_transform(transform_); // set camera transform to players transform
		children_.push_back(fps_camera_); // add camera as child
	}
	top_down_camera_ = new Camera(); // create top down camera
	top_down_camera_->set_transform(new Transform(XMVectorSplatOne(), XMQuaternionIdentity(), top_down_distance)); // set camera transform to a top down distance with initial scale and rotation 
	top_down_camera_->rotate(rotation_down,0,0); // rotate camera to look down
	is_kinetic_ = false; // player shall not be kinetic
	sphere_collider_ = new SphereCollider(transform_->get_local_position(), player_col_radius); // initialize sphere collider with players collision radius
}


