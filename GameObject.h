#pragma once
#include <string>
#include "Vector3.h"
#include "Transform.h"
#include "Model.h"
#include "SoundEngine.h"
#include "GameData.h"

class GameObject {
public:
	GameObject();
	~GameObject();

	GameObject(std::string name, Transform transform, Model model, float collision_radius);

	void spawn(Vector3 position);
	void render();
	void start();
	void update();
	void move(Vector3 position);
	void collided(Vector3 target);

protected:
	std::string name;
	Transform transform;
	Model model;
	SoundEngine sound;
	float collision_radius;
	GameData game_data;

};