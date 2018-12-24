#pragma once
#include "Character.h"
#include "Camera.h"


class Player : public Character
{
public:
	Player();
	~Player();

	Player(LPCSTR name, bool fps = false);

	void update_input();

	void update(VGTime timer);

private:
	Camera camera;
};

