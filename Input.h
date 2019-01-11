#pragma once
#include "VGTime.h"
#include "Character.h"


class GameObject;
class Character;

// basic input class using direct input to process mouse, keyboard and gamepad 
class Input
{
public:
	Input();
	~Input(); 

	HRESULT initialise_input(HINSTANCE instance, HWND hwnd); // initialise input and binding it to the program instance and window handle

	HRESULT update_input(GameObject* actor, VGTime* game_time); // updates all input and causes the given actor to move accordingly with deltaTime
	void mouse_moved(GameObject* actor, VGTime* game_time); // tracks mouse movement and causes the given actor to move accordingly with deltaTime
	bool is_key_pressed(unsigned char di_keycode); // determines whether given keycode was pressed
	bool is_key_released(unsigned char di_keycode); // determines whether given keycode was released
	void cleanup() const; // cleanup pointers to prevent memory leak
private:
	HWND hwnd_; // window handle to bind the input to
	bool paused_ = false; // is input handling paused
	bool fps_ = true; // first person view	

	IDirectInput8 *direct_input_; // pointer to direct input instance
	IDirectInputDevice8 *keyboard_; // pointer to keyboard input device
	unsigned char keyboard_keys_state_[256]; // array of keystates
	bool pressed_[256]; // array of pressed keys

	IDirectInputDevice8 *mouse_input_; // mouse input device
	DIMOUSESTATE mouse_state_; // mouse state tracking all necessary mouse events

	const double jump_speed_ = 8.0f;
	const double rot_speed_ = 10.0; // rotation look speed for mouse movement
	const double move_speed_ = 8.0; // movement speed

	const long mouse_x_center_ = 480; // mouse x position of the screen center
	const long mouse_y_center_ = 640; // mouse y position of the screen center

	long mouse_x_ = mouse_x_center_; // current mouse x position
	long mouse_y_ = mouse_y_center_; // current mouse y position

	bool locked_ = true; // says whether to use the forward vector locked horizontally or not
	bool fly_mode_ = false; // says whether player can use jump - even when he is in air - or not
	bool jumping_;
};
