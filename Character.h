#pragma once
#include "GameObject.h"
#include "Collectable.h"


enum state {
	standing,
	crouching,
	moving,
	airborne,
	accelerating,
	hooking
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
	explicit Character(LPCSTR name, LPCSTR tag = "character", bool fps = false);
	Character(LPCSTR name, Transform transform, Model model, float collision_radius, int life, state state, LPCSTR tag, bool fps = false);
	~Character();

	Character(int life, state state, LPCSTR name = "no_identity", LPCSTR tag = "default", bool fps = false);
	void cut_target(Character target) const;
	void throw_at(Character target) const;

	state get_state() const;

	void spawn(XMVECTOR position) override;
	void render() override;
	void start() override;
	void update(VGTime timer) override;
	void set_speed_buff(float speed, float time);
	void set_stat_kills(int kills);
	void set_stat_deaths(int deaths);
	void set_stat_score(int score);
	Stats get_stats();
	void die();
	bool inflict(float dmg);
protected:
	Stats stats_;
	int life_;
	Collectable collected_;
	state state_ = standing;
	float speed_;
};

