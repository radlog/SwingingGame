#pragma once
#include "GameObject.h"
#include "Collectable.h"

enum STATE {
	STANDING,
	CROUCHING,
	MOVING,
	AIRBORNE,
	ACCELERATING,
	HOOKING
};

struct Stats {
	int kills;
	int deaths;
	int score;
};

class Character : public GameObject
{

public:
	Character();
	~Character();
	explicit Character(LPCSTR name);
	Character(LPCSTR name, Model *model, Transform transform = Transform());

	virtual void cut_target(Character target) const;
	virtual void throw_at(Character target) const;

	virtual STATE get_state() const;

	virtual void set_speed_buff(float speed, float time);
	virtual void set_stat_kills(int kills);
	virtual void set_stat_deaths(int deaths);
	virtual void set_stat_score(int score);
	virtual Stats get_stats();
	virtual void die();
	virtual bool inflict(float dmg);
protected:
	Stats stats_;
	int life_ = 100;
	Collectable collected_;
	STATE state_ = STANDING;
	float speed_ = 100;
};

