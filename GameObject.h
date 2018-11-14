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

	GameObject(std::string name);
	GameObject(std::string name, Transform transform, Model model, float collision_radius);

	virtual void spawn(Vector3 position);
	virtual void render();
	virtual void start();
	virtual void update();
	virtual void move(Vector3 position);
	virtual void collided(Vector3 target);
	virtual std::string get_name();

protected:
	std::string name;
	Transform transform;
	Model model;
	SoundEngine sound;
	float collision_radius;
	GameData game_data;

};