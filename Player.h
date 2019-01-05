#pragma once
#include "Character.h"
#include "Camera.h"


class Player : public Character
{
public:
	Player();
	~Player();

	explicit Player(LPCSTR name, LPCSTR tag = "player", bool fps = false);

	void update_input() const;

	void update(VGTime timer) override;

	void spawn(XMVECTOR position) override;
	void render() override;
	void start() override;	
	void cleanup() override;
private:
	Camera camera_;
};

