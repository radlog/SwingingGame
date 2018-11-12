#pragma once
#include <d3d11.h>
#include <string>
#include "Transform.h"
#include "Model.h"
#include "GameData.h"
#include "SoundEngine.h"



class GameObject
{
	

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

private:
	std::string name;
	Transform transform;
	Model model;
	SoundEngine* engine;
	float collision_radius;
	GameData game;
};

