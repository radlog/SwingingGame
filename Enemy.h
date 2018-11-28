#pragma once
class Enemy : public Character
{

	enum Enemy_Type {
		Grounder,
		Swinger
	};

public:
	Enemy();
	~Enemy();

	void attack(Character target);
};

