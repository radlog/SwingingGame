#pragma once
#include "Player.h";
#include "Command.h";
#include "d3dfw.h"
#include "VGTime.h"
class Player;


class Input
{
public:
	Input();
	~Input();

	Input(GameObject* actor);
	void handle_input(WPARAM key);
	HRESULT InitialiseInput(HINSTANCE hInstance, HWND hWnd);

	HRESULT UpdateInput(GameObject* actor, VGTime* gameTime);
	void MouseMoved(GameObject* actor, VGTime* gameTime);
	bool IsKeyPressed(unsigned char DI_keycode);
	bool IsKeyReleased(unsigned char DI_keycode);
	void Cleanup();
private:
	bool paused = false;
	bool fps = true; // first person view
	GameObject* actor;
	Command* w_button;
	Command* a_button;
	Command* s_button;
	Command* d_button;
	Command* c_button;
	Command* jump_button;
	Command* shoot_button;
	Command* hook_button;

	WPARAM* forward;
	WPARAM left = 0x41;
	WPARAM back = 0x53;
	WPARAM right = 0x44;
	WPARAM crouch = VK_LCONTROL;
	WPARAM jump = VK_SPACE;
	//const WPARAM shoot = ;
	//const WPARAM hook =

	IDirectInput8 *directInput;
	IDirectInputDevice8 *keyboard;
	unsigned char keyboardKeysState[256];
	bool pressed[256];

	IDirectInputDevice8 *mouseInput;
	DIMOUSESTATE mouseState;

	const float rot_speed = 10.0f;
	const float move_speed = 30.0f;

	float mouse_x_center = 480.0f;
	float mouse_y_center = 640.0f;

	float mouse_x = mouse_x_center;
	float mouse_y = mouse_y_center;


};

