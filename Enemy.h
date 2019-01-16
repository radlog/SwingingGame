#pragma once

extern const double chase_radius;
extern const double chase_speed;

class Enemy : public Character
{
	// enemy type ground unit or swinging unit
	enum ENEMY_TYPE {
		GROUNDER,
		SWINGER
	};
	
public:
	Enemy();
	~Enemy();
	
	explicit Enemy(LPCSTR name);
	explicit Enemy(LPCSTR name, Model *model, Transform *transform);

	/**
	 * \brief causes the enemy to attack a character
	 * \param target character to be attacked
	 */
	void attack(Character *target) const; // attack a target


	void chase_target(GameObject *target, VGTime *timer); // chases target
	
	void update(VGTime * timer) override; // update enemy logic
	void set_path(vector<XMVECTOR> *path);

private:



	vector<XMVECTOR> *path_;
	double time_since_collision_ = 0; // total game time is stored, when enemy collides
	float time_to_recover_ = 4; // time that enemy flees when collided
	float switch_limit_ = 1; // maximum distance to the next node in path to switch to another
	void step_back(VGTime* timer, XMVECTOR direction); // move enemy away from the target it collided with
	void patrol(VGTime* timer);
	int path_counter_;
};

