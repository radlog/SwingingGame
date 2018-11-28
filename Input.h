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

	Input(Character* actor);
	void handle_input(WPARAM key);
	HRESULT InitialiseInput(HINSTANCE hInstance, HWND hWnd);

	HRESULT UpdateInput(GameObject* actor, VGTime* gameTime);
	void MouseMoved(GameObject* actor, VGTime* gameTime);
	bool IsKeyPressed(unsigned char DI_keycode);
	bool IsKeyReleased(unsigned char DI_keycode);
	void Cleanup();
private:
	HWND hWnd;
	bool paused = false;
	bool fps = true; // first person view
	GameObject* actor;


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

	const double rot_speed = 10.0;
	const double move_speed = 30.0;

	long mouse_x_center = 480;
	long mouse_y_center = 640;

	long mouse_x = mouse_x_center;
	long mouse_y = mouse_y_center;

};
#endif
