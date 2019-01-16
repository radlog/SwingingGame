#include "Input.h"
#include "Player.h"


Input::Input()
{

}


Input::~Input()
{
}


HRESULT Input::initialise_input(const HINSTANCE instance, const HWND hwnd)
{
	this->hwnd_ = hwnd;
	HRESULT hr;
	ZeroMemory(keyboard_keys_state_, sizeof(keyboard_keys_state_));

	// initialise input factory
	hr = DirectInput8Create(instance, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(&direct_input_), NULL);
	if (FAILED(hr)) return hr;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////keyboard init////////////////////////////////////////////////////////////////
	// create keyboard
	hr = direct_input_->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	if (FAILED(hr)) return hr;

	// set keyboard format
	hr = keyboard_->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr)) return hr;

	// set keyboards behaviour of interaction between other instances of the same device on different processes and the system
	hr = keyboard_->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr)) return hr;

	// acquire the setup device
	hr = keyboard_->Acquire();
	if (FAILED(hr)) return hr;

	////////////////////////////////////////////////////////keyboard init////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////mouse init///////////////////////////////////////////////////////////////////
	// create mouse
	hr = direct_input_->CreateDevice(GUID_SysMouse, &mouse_input_, NULL);
	if (FAILED(hr)) return hr;

	//mouse_input_->set

	// set data format for the mouse
	hr = mouse_input_->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr)) return hr;

	// set interaction of mouse between processes and other instances of the same device
	hr = mouse_input_->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr)) return hr;

	// acquire setup mouse
	hr = mouse_input_->Acquire();
	if (FAILED(hr)) return hr;

	////////////////////////////////////////////////////////mouse init///////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

HRESULT Input::update_input(GameObject* actor, VGTime* game_time)
{
	// update keyboard input and write state in keyboard_keys_state_
	HRESULT hr = keyboard_->GetDeviceState(sizeof(keyboard_keys_state_), LPVOID(&keyboard_keys_state_));

	if (FAILED(hr)) {
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			keyboard_->Acquire(); // reacquire keyboard when input signal was lost
		}
	}

	// update mouse input and write state in the mouse_state_
	hr = mouse_input_->GetDeviceState(sizeof(mouse_state_), LPVOID(&mouse_state_));

	if (FAILED(hr)) {
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			mouse_input_->Acquire(); // reacquire mouse when input signal was lost
		}
	}

	if (is_key_released(DIK_L)) // pause first person control and all other input
		paused_ = !paused_;

	if (paused_) return S_OK; // do not check for other keys if input is paused

	if (is_key_pressed(DIK_ESCAPE)) // exit game
	{
		cleanup();
		DestroyWindow(hwnd_);
	}

	const auto actor_type = typeid(*actor).name(); // get type of actor
	const auto player_type = typeid(Player).name(); // get type if player
	auto direction = XMVectorZero(); // set initial direction

	if (is_key_released(DIK_F)) locked_ = !locked_; // toggle movement lock to limit movement to x and z or allow all axis

	if (fps_) mouse_moved(actor, game_time); // check mouse movement and rotate in adequate direction

	// DEVELOPERS ONLY
#ifdef DEBUG
	if (is_key_pressed(DIK_H)) fly_mode_ = !fly_mode_; // toggle fly mode
#endif

	if (is_key_pressed(DIK_LSHIFT)) // crouch key
	{
		if (fly_mode_) direction = -Transform::world_up; // move down if fly_mode_ is on
		else if (actor_type == player_type) static_cast<Player*>(actor)->crouch(game_time); // crouch if fly_mode_ is off
	}
	// DEVELOPERS ONLY 
#ifdef DEBUG
	if (is_key_pressed(DIK_SPACE) )
	{
		if (fly_mode_) direction = Transform::world_up; // move up in world space
	}
#endif

	if (is_key_pressed(DIK_SPACE) || mouse_state_.lZ > 0) // set grounded state if space is pressed
	{
		actor->set_grounded(false);
		static_cast<Player*>(actor)->set_state(AIRBORNE);
	}

	// execute this only, if the actor is a player
	if (actor_type == player_type)
	{
		if (!actor->get_grounded() && static_cast<Player*>(actor)->get_state() == AIRBORNE)
			static_cast<Player*>(actor)->jump(game_time); // jump while player state is airborne and is not grounded
		else static_cast<Player*>(actor)->set_state(STANDING); // set player to standing
	}

	if (is_key_released(DIK_G)) actor->set_kinetic(!actor->get_kinetic()); // toggle kinetic state to be affected by gravity or not

	if (is_key_pressed(DIK_A)) direction += -actor->get_transform()->get_local_right(); // set direction -right
	if (is_key_pressed(DIK_D)) direction += actor->get_transform()->get_local_right(); // set direction +right
	if (is_key_pressed(DIK_W))
	{
		if (locked_)direction += actor->get_transform()->get_local_forward_horizontal(); // horizontal movement forward
		else direction += actor->get_transform()->get_local_forward(); // movement along all axis forward
	}
	if (is_key_pressed(DIK_S))
	{
		if (locked_)direction += -actor->get_transform()->get_local_forward_horizontal(); // horizontal movement backward
		else direction += -actor->get_transform()->get_local_forward(); // movement along all axis backward
	}

	// rotation with keyboard ->excluded from build
#ifdef DEBUG
	if (is_key_pressed(DIK_LEFT)) { actor->rotate_fixed(0, static_cast<float>(-game_time->delta_time() * rot_speed_), 0); 	OutputDebugStringA("left arrow : ");  OutputDebugStringA(std::to_string((-game_time->delta_time() * rot_speed_)).c_str()); OutputDebugStringA("\n"); }
	if (is_key_pressed(DIK_RIGHT)) { actor->rotate_fixed(0, static_cast<float>(game_time->delta_time() * rot_speed_), 0); OutputDebugStringA("left arrow : "); OutputDebugStringA(std::to_string((game_time->delta_time() * rot_speed_)).c_str()); 	OutputDebugStringA("\n"); }
	if (is_key_pressed(DIK_UP)) actor->rotate_fixed(static_cast<float>(-game_time->delta_time() * rot_speed_), 0, 0);
	if (is_key_pressed(DIK_DOWN)) actor->rotate_fixed(static_cast<float>(game_time->delta_time() * rot_speed_), 0, 0);
#endif


	// if direction has changed
	if (direction.x != 0 || direction.y != 0 || direction.z != 0)
		actor->translate(direction, game_time->delta_time() * move_speed_); // move in the direction with move_speed_

	return S_OK;
}

void Input::mouse_moved(GameObject* actor, VGTime* game_time)
{
	const auto speed_x = static_cast<float> (game_time->delta_time() * rot_speed_ * static_cast<float> (mouse_state_.lY)); // get mouse state y and store it
	const auto speed_y = static_cast<float>(game_time->delta_time() * rot_speed_ * static_cast<float> (mouse_state_.lX)); // get mouse state x and store it

	actor->rotate_fixed(speed_x, speed_y, 0); // rotate with a rotation lock for the x axis

	mouse_x_ = mouse_x_center_; // reset mouse x
	mouse_y_ = mouse_y_center_; // reset mouse y

	SetCursorPos(0, 0); // set cursor to default position
}

// check if the state of the keyboard has the keycode pressed
bool Input::is_key_pressed(const unsigned char di_keycode)
{
	return keyboard_keys_state_[di_keycode] & 0x80; 
}

// check if any key is released that was previously pressed
bool Input::is_key_released(const unsigned char di_keycode)
{
	// if previously pressed keys are stored, but the state has no keycode stored that is pressed
	if (!is_key_pressed(di_keycode) && pressed_[di_keycode]) 
	{
#ifdef DEBUG
		//OutputDebugString("released");
#endif
		return !((pressed_[di_keycode] = !pressed_[di_keycode])); // negate the pressed stored key and then return its negation
	}
	if (is_key_pressed(di_keycode)) pressed_[di_keycode] = true; // set pressed value in array of keys which are pressed

	return false;
}

// cleanup to avoid memory leaks 
void Input::cleanup() const
{
	if (keyboard_) {
		keyboard_->Unacquire();
		keyboard_->Release();
	}
	if (mouse_input_)
	{
		mouse_input_->Unacquire();
		mouse_input_->Release();
	}
}
