#include "Input.h"



Input::Input()
{

}


Input::~Input()
{
}

Input::Input(Character * actor)
{
	this->actor = actor;
}

void Input::handle_input(WPARAM key)
{
	//if(actor )
	//State state = actor->get_State();

	//switch (state)
	//{
	//case Crouching:
	//	break;
	//case Standing:
	//	break;
	//case Airborne:
	//	break;		
	//case Moving:
	//	break;
	//case Accelerating:
	//	break;
	//case Hooking:
	//	break;
	//}


	switch (key)
	{
		// represets move forward
	case 0x57:

		break;
		// represets move left
	case 0x41:
		break;
		// represets move back
	case 0x53:
		break;
		// represets move right
	case 0x44:
		break;
		// represents jump
	case VK_SPACE:
		break;
		// represents crouch
	case VK_LCONTROL:
		break;
	}
}

HRESULT Input::InitialiseInput(HINSTANCE hInstance, HWND hWnd)
{
	this->hWnd = hWnd;
	HRESULT hr;
	ZeroMemory(keyboardKeysState, sizeof(keyboardKeysState));

	// initialise input factory
	hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, NULL);
	if (FAILED(hr)) return hr;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////keyboard init////////////////////////////////////////////////////////////////
	// create keyboard
	hr = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	if (FAILED(hr)) return hr;

	// set keyboard format
	hr = keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr)) return hr;

	// set keyboards behaviour of interaction between other instances of the same device on different processes and the system
	hr = keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr)) return hr;

	// acquire the setup device
	hr = keyboard->Acquire();
	if (FAILED(hr)) return hr;

	////////////////////////////////////////////////////////keyboard init////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////mouse init///////////////////////////////////////////////////////////////////
	// create mouse
	hr = directInput->CreateDevice(GUID_SysMouse, &mouseInput, NULL);
	if (FAILED(hr)) return hr;

	// set data format for the mouse
	hr = mouseInput->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr)) return hr;

	// set interaction of mouse between processes and other instances of the same device
	hr = mouseInput->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr)) return hr;

	// acquire setup mouse
	hr = mouseInput->Acquire();
	if (FAILED(hr)) return hr;

	////////////////////////////////////////////////////////mouse init///////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

HRESULT Input::UpdateInput(GameObject* actor, VGTime* gameTime)
{
	
	HRESULT hr = NULL;
	hr = keyboard->GetDeviceState(sizeof(keyboardKeysState), (LPVOID)&keyboardKeysState);

	if (FAILED(hr)) {
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			keyboard->Acquire();
		}
	}

	hr = mouseInput->GetDeviceState(sizeof(mouseState), (LPVOID)&mouseState);

	if (FAILED(hr)) {
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			mouseInput->Acquire();
		}
	}

	if (IsKeyReleased(DIK_L)) 
		paused = !paused;

	if (paused) return S_OK;

	if(fps)	MouseMoved(actor, gameTime);

	if (IsKeyPressed(DIK_ESCAPE)) DestroyWindow(hWnd);

	// TODO:spawn gameobject
	//if (IsKeyPressed(DIK_P)) GameObject();

	if (IsKeyPressed(DIK_A)) actor->transform.right(static_cast<float> (-gameTime->deltaTime() * move_speed));
	if (IsKeyPressed(DIK_D)) actor->transform.right(static_cast<float>(gameTime->deltaTime() * move_speed));
	if (IsKeyPressed(DIK_W)) actor->transform.horizontal_forward(static_cast<float>(gameTime->deltaTime()*move_speed));
	if (IsKeyPressed(DIK_S)) actor->transform.horizontal_forward(static_cast<float>(-gameTime->deltaTime()*move_speed));


	if (IsKeyPressed(DIK_LEFT)) actor->transform.rotate_fixed(0, static_cast<float>(-gameTime->deltaTime() * rot_speed /10), 0);
	if (IsKeyPressed(DIK_RIGHT)) actor->transform.rotate_fixed(0, static_cast<float>(gameTime->deltaTime() * rot_speed/10), 0);
	if (IsKeyPressed(DIK_UP)) actor->transform.rotate_fixed(static_cast<float>(-gameTime->deltaTime() * rot_speed/10), 0, 0);
	if (IsKeyPressed(DIK_DOWN)) actor->transform.rotate_fixed(static_cast<float>(gameTime->deltaTime() * rot_speed/10), 0, 0);

	return S_OK;
}

void Input::MouseMoved(GameObject* actor, VGTime* gameTime)
{
	mouse_x += mouseState.lX;
	mouse_y += mouseState.lY;

	actor->transform.rotate_fixed(static_cast<float> (mouseState.lY * gameTime->deltaTime() * rot_speed), static_cast<float> (mouseState.lX * gameTime->deltaTime() * rot_speed), 0);

	mouse_x = mouse_x_center;
	mouse_y = mouse_y_center;
	SetCursorPos(mouse_x, mouse_x);
}

bool Input::IsKeyPressed(unsigned char DI_keycode)
{
	return keyboardKeysState[DI_keycode] & 0x80;
}

bool Input::IsKeyReleased(unsigned char DI_keycode)
{
	if (!IsKeyPressed(DI_keycode) && pressed[DI_keycode])
	{
#ifdef DEBUG
		OutputDebugString("released");
#endif
		return !((pressed[DI_keycode] = !pressed[DI_keycode]));
	}
	if (IsKeyPressed(DI_keycode)) pressed[DI_keycode] = true;

	return false;
}

void Input::Cleanup()
{
	if (keyboard) {
		keyboard->Unacquire();
		keyboard->Release();
	}
	if (mouseInput)
	{
		mouseInput->Unacquire();
		mouseInput->Release();
	}
}
