#pragma once
#include "VGTime.h"
#include "Character.h"

#ifndef INPUT_H
#define INPUT_H
class GameObject;
class Character;

class Input
{
public:
	Input();
	~Input();

	explicit Input(Character* actor);
	void handle_input(WPARAM key);
	HRESULT initialise_input(HINSTANCE instance, HWND hwnd);

	HRESULT update_input(GameObject* actor, VGTime* game_time);
	void mouse_moved(GameObject* actor, VGTime* game_time);
	bool is_key_pressed(unsigned char di_keycode);
	bool is_key_released(unsigned char di_keycode);
	void cleanup() const;
private:
	HWND hwnd_;
	bool paused_ = false;
	bool fps_ = true; // first person view
	GameObject* actor_;

	IDirectInput8 *direct_input_;
	IDirectInputDevice8 *keyboard_;
	unsigned char keyboard_keys_state_[256];
	bool pressed_[256];

	IDirectInputDevice8 *mouse_input_;
	DIMOUSESTATE mouse_state_;

	const double rot_speed_ = 10.0;
	const double move_speed_ = 30.0;

	long mouse_x_center_ = 480;
	long mouse_y_center_ = 640;

	long mouse_x_ = mouse_x_center_;
	long mouse_y_ = mouse_y_center_;

};
#endif
