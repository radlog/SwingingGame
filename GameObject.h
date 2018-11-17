#pragma once
#include <string>
#include "Transform.h"
#include "Model.h"
#include "SoundEngine.h"
#include "GameData.h"

class GameObject {
public:
	GameObject();
	~GameObject();

	GameObject(std::string name, Transform transform, Model model, float collision_radius);

	void spawn(XMVECTOR position);
	void render();
	void start();
	void update();
	void move(XMVECTOR position);
	void collided(XMVECTOR target);

protected:
	std::string name;
	Transform transform;
	Model model;
	SoundEngine sound;
	float collision_radius;
	GameData game_data;

};