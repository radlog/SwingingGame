#pragma once
class Character;

/**
 * \brief 
 */
class Collectable : public GameObject
{

	enum ITEM_TYPE {
		SPEEDBUFF,
		COIN
	};
public:
	Collectable();
	~Collectable();

	explicit Collectable(ITEM_TYPE item_type);


	void collected(Character actor) const;

	void spawn(XMVECTOR position) override;
	void render() override;
	void start() override;
	void update(VGTime timer) override;
	void cleanup() override;
private:
	ITEM_TYPE item_type_;
};

