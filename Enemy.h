#pragma once
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

	/**
	 * \brief causes the enemy to attack a character
	 * \param target character to be attacked
	 */
	void attack(Character target) const;
};

