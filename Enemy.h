#pragma once

extern const float chase_radius;
extern const float chase_speed;

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
	explicit Enemy(LPCSTR name, Model *model, Transform transform = Transform());

	/**
	 * \brief causes the enemy to attack a character
	 * \param target character to be attacked
	 */
	void attack(Character target) const;

	void chase_target(GameObject *target, VGTime *timer);

	
};

