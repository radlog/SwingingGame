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

	GameObject(std::string name);
	GameObject(std::string name, Transform transform, Model model, float collision_radius);

<<<<<<< HEAD
	void spawn(XMVECTOR position);
	void render();
	void start();
	void update();
	void move(XMVECTOR position);
	void collided(XMVECTOR target);
=======
	virtual void spawn(Vector3 position);
	virtual void render();
	virtual void start();
	virtual void update();
	virtual void move(Vector3 position);
	virtual void collided(Vector3 target);
	virtual std::string get_name();
>>>>>>> refs/remotes/origin/master

protected:
	std::string name;
	Transform transform;
	Model model;
	SoundEngine sound;
	float collision_radius;
	GameData game_data;

};