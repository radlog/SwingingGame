#pragma once
#include "Character.h"
#include "Camera.h"

class Player : public Character
{
public:
	Player();
	~Player();
	/**
	 * \brief initialises a player with name and fps mode set 
	 * \param name players name
	 * \param fps first person mode
	 */
	explicit Player(LPCSTR name, bool fps);
	/**
	 * \brief initialises a player with name, model, transform and fps mode set 
	 * \param name players name
	 * \param model players model
	 * \param transform starting transform
	 * \param fps first person mode
	 */
	explicit Player(LPCSTR name, Model *model, Transform transform = Transform(), bool fps = true);
	/**
	 * \brief process input using the direct input class of the current directX instance
	 * \param timer using VGTime to update with deltaTime
	 */
	void update_input(const VGTime timer);
	/**
	 * \brief update method combining all necessary update methods in correct order
	 * \param timer using VGTime to update with deltaTime
	 */
	void update(VGTime timer);
	/**
	 * \brief move camera along with the players position
	 */
	void update_camera() const;
	/**
	 * \brief get the Camera member of the class
	 * \return Camera
	 */
	Camera *get_camera() const;

protected:
	Camera *camera_ = nullptr; //the camera attached to the player
};

