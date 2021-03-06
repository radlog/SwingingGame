#pragma once
#include "GameObject.h"
#include "Collectable.h"


class Model;

// defines physical movement states of the character
enum STATE {
	STANDING,
	CROUCHING,
	MOVING,
	AIRBORNE,
	ACCELERATING,
	HOOKING
};

// describes character stats kills, deaths, score
struct Stats {
	int kills;
	int deaths;
	int score;
};

/**
 * \brief Character class for controlable interactive character
 */
class Character : public GameObject
{

public:
	Character();
	~Character();
	/**
	 * \brief initialises a character with name
	 * \param name characters name
	 */
	explicit Character(LPCSTR name);
	/**
	 * \brief initialises a character with name, model, transform
	 * \param name characters name
	 * \param model characters model
	 * \param transform initial starting transform
	 */
	Character(LPCSTR name, Model *model, Transform *transform);

	/**
	 * \brief cuts the selected character off the hook
	 * \param target the character to cut off
	 */
	virtual void cut_target(Character target) const;
	/**
	 * \brief throw collected item at target
	 * \param target the character to throw at
	 */
	virtual void throw_at(Character target) const;

	/**
	 * \brief gets characters current movement state
	 * \return current STATE
	 */
	virtual STATE get_state() const;


	virtual void set_state(STATE state);

	/**
	 * \brief increases characters movement speed for a specific time
	 * \param speed increase value of the speed
	 * \param time float value in seconds
	 */
	virtual void set_speed_buff(float speed, float time);
	/**
	 * \brief sets characters kills
	 * \param kills int value
	 */
	virtual void set_stat_kills(int kills);
	/**
	 * \brief sets characters deaths
	 * \param deaths int value
	 */
	virtual void set_stat_deaths(int deaths);
	/**
	 * \brief sets characters score
	 * \param score int value
	 */
	virtual void set_stat_score(int score);
	/**
	 * \brief gets characters current stats
	 * \return Stats
	 */
	virtual Stats get_stats();
	/**
	 * \brief kill the character
	 */
	virtual void die();
	/**
	 * \brief inflicts damage to the character
	 * \param dmg damage to be inflicted
	 * \return character dead
	 */
	virtual bool inflict(int dmg);
	void update(VGTime *timer)override;
	void crouch(VGTime *timer);
	void jump(VGTime *timer);
	void hook(VGTime *timer);
	int get_life() const;
protected:
	Stats stats_; // character stats
	Collectable collected_; // collected items
	STATE state_ = STANDING; // character movement state
	int life_; // character life
	float speed_; // character movement speed
	const double jump_speed_ = 8.0f; // character jump speed
	const XMVECTOR spawn_position_ = XMVectorSet(0, 30, 0, 0);
	const int max_life_ = 100;
	const float initial_speed_ = 10;
	const float ground_death_position_ = -10;
};

