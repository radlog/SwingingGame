#include "Input.h"



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
	HRESULT hr = keyboard_->GetDeviceState(sizeof(keyboard_keys_state_), LPVOID(&keyboard_keys_state_));

	if (FAILED(hr)) {
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			keyboard_->Acquire();
		}
	}

	hr = mouse_input_->GetDeviceState(sizeof(mouse_state_), LPVOID(&mouse_state_));

	if (FAILED(hr)) {
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			mouse_input_->Acquire();
		}
	}

	if (is_key_released(DIK_L))
		paused_ = !paused_;

	if (paused_) return S_OK;

	if (is_key_pressed(DIK_ESCAPE)) DestroyWindow(hwnd_);


	auto direction = XMVectorZero();

	if (is_key_released(DIK_F)) locked_ = !locked_;

	// TODO: spawn gameobject
	//if (IsKeyPressed(DIK_P)) GameObject();
	if (fps_) mouse_moved(actor, game_time);

	if (is_key_pressed(DIK_SPACE))
		if (is_key_pressed(DIK_LSHIFT))
			direction = -actor->transform.get_local_up();
		else
			direction = actor->transform.get_local_up();

	if (is_key_released(DIK_G)) actor->set_kinetic(!actor->get_kinetic());

	if (is_key_pressed(DIK_A)) direction += -actor->transform.get_local_right();
	if (is_key_pressed(DIK_D)) direction += actor->transform.get_local_right();
	if (is_key_pressed(DIK_W))
	{
		if (locked_)direction += actor->transform.get_local_forward_horizontal();
		else direction += actor->transform.get_local_forward();
	}
	if (is_key_pressed(DIK_S))
	{
		if (locked_)direction += -actor->transform.get_local_forward_horizontal();
		else direction += -actor->transform.get_local_forward();
	}

	if (is_key_pressed(DIK_LEFT)) actor->rotate_fixed(0, static_cast<float>(-game_time->delta_time() * rot_speed_ / 10), 0);
	if (is_key_pressed(DIK_RIGHT)) actor->rotate_fixed(0, static_cast<float>(game_time->delta_time() * rot_speed_ / 10), 0);
	if (is_key_pressed(DIK_UP)) actor->rotate_fixed(static_cast<float>(-game_time->delta_time() * rot_speed_ / 10), 0, 0);
	if (is_key_pressed(DIK_DOWN)) actor->rotate_fixed(static_cast<float>(game_time->delta_time() * rot_speed_ / 10), 0, 0);


	actor->translate(direction, game_time->delta_time() * move_speed_);
	//if (is_key_pressed(DIK_A)) actor->move_left(game_time->delta_time() * move_speed_);
	//if (is_key_pressed(DIK_D)) actor->move_right(game_time->delta_time() * move_speed_);
	//if (is_key_pressed(DIK_W)) actor->move_horizontal_forward(game_time->delta_time() * move_speed_);
	//if (is_key_pressed(DIK_S)) actor->move_horizontal_backward(game_time->delta_time() * move_speed_);


	//if (is_key_pressed(DIK_LEFT)) actor->rotate_fixed(0, static_cast<float>(-game_time->delta_time() * rot_speed_ /10), 0);
	//if (is_key_pressed(DIK_RIGHT)) actor->rotate_fixed(0, static_cast<float>(game_time->delta_time() * rot_speed_/10), 0);
	//if (is_key_pressed(DIK_UP)) actor->rotate_fixed(static_cast<float>(-game_time->delta_time() * rot_speed_/10), 0, 0);
	//if (is_key_pressed(DIK_DOWN)) actor->rotate_fixed(static_cast<float>(game_time->delta_time() * rot_speed_/10), 0, 0);



	return S_OK;
}

void Input::mouse_moved(GameObject* actor, VGTime* game_time)
{
	actor->rotate_fixed(game_time->delta_time() * rot_speed_ * double(mouse_state_.lY), game_time->delta_time() * rot_speed_ * double(mouse_state_.lX), 0);

	mouse_x_ = mouse_x_center_;
	mouse_y_ = mouse_y_center_;

	SetCursorPos(mouse_x_, mouse_x_);
}

bool Input::is_key_pressed(const unsigned char di_keycode)
{
	return keyboard_keys_state_[di_keycode] & 0x80;
}

bool Input::is_key_released(const unsigned char di_keycode)
{
	if (!is_key_pressed(di_keycode) && pressed_[di_keycode])
	{
#ifdef DEBUG
		//OutputDebugString("released");
#endif
		return !((pressed_[di_keycode] = !pressed_[di_keycode]));
	}
	if (is_key_pressed(di_keycode)) pressed_[di_keycode] = true;

	return false;
}

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
