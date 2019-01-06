#pragma once
#include "Character.h"
#include "Camera.h"

class Player : public Character
{
public:
	Player();
	~Player();
	
	explicit Player(LPCSTR name, bool fps);
	explicit Player(LPCSTR name, Model *model = nullptr, Transform transform = Transform(), bool fps = true);

	void update_input(const VGTime timer);
	void update(VGTime timer);
	void update_camera();
	Camera *get_camera();

protected:
	Camera *camera_ = nullptr;
};

