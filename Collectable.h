#pragma once
class Character;

/**
 * \brief Represents items that can be collected on the map
 */
class Collectable : public GameObject
{
	// item type
	enum ITEM_TYPE {
		SPEEDBUFF, // speedbuff
		COIN // score
	};
public:	
	/**
	 * \brief initialises an item as coin, if nothing else is specified
	 * \param item_type defines the item type of the object
	 */
	explicit Collectable(ITEM_TYPE item_type = COIN);
	~Collectable();

	/**
	 * \brief item triggers an actor to call a method depending on the item type and gets deleted
	 * \param actor the character to be affected 
	 */
	void collected(Character actor) const;

private:
	ITEM_TYPE item_type_; // holds this objects item type
};

