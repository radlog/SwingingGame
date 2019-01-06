#pragma once
#include "Character.h"
#include "Camera.h"

class Player : public Character
{
public:
	Player();
	~Player();
	
	explicit Player(LPCSTR name, bool fps);
	explicit Player(LPCSTR name, Model *model = nullptr, Transform transform = Transform(), bool fps = false);

	void update_input() const;
	void update(VGTime timer);

protected:
	Camera *camera_ = nullptr;
};

