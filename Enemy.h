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
	void attack(Character target) const;


	void chase_target(GameObject *target, VGTime *timer);
	void update(VGTime * timer) override;

private:
	double time_since_collision_ = 0;
	float time_to_recover_ = 4;
	void step_back(VGTime* timer, XMVECTOR direction);
};

