#pragma once
class Enemy : public Character
{

	enum ENEMY_TYPE {
		GROUNDER,
		SWINGER
	};

public:
	Enemy();
	~Enemy();

	void attack(Character target) const;
	void cleanup() override;
	void spawn(XMVECTOR position) override;
	void render() override;
	void start() override;
	void update(VGTime timer) override;
};

