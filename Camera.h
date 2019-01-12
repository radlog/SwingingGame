#pragma once
#include "GameObject.h"


/**
 * \brief Simple Camera used to get the correct projection and render the scene
 */
class Camera : public GameObject
{
public:
	Camera();
	~Camera();


	/**
	 * \brief initialises the camera with name, field of view, aspect ratio, near plane, far plane
	 * \param name name the camera
	 * \param fov field of view
	 * \param aspect_ratio aspect ratio of the projection
	 * \param near_plane near plane clipping
	 * \param far_plane far plane clipping
	 */
	explicit Camera(LPCSTR name, float fov = 80.0f, float aspect_ratio = 640.0f / 480.0f, float near_plane = 0.01f,
		float far_plane = 300.0f);

	/**
	 * \brief sets the field of view of the camera
	 * \param fov field of view
	 */
	void set_fov(float fov);
	/**
	 * \brief sets the aspect ratio of the camera
	 * \param aspect_ratio aspect ratio
	 */
	void set_aspect_ratio(float aspect_ratio);
	/**
	 * \brief sets the near plane of the camera
	 * \param near_plane near plane
	 */
	void set_near_plane(float near_plane);
	/**
	 * \brief sets the far plane of the camera
	 * \param far_plane far plane
	 */
	void set_far_plane(float far_plane);
	/**
	 * \brief gets the field of view of the camera
	 */
	float get_fov() const;
	/**
	 * \brief gets the aspect ratio of the camera
	 */
	float get_aspect_ratio() const;
	/**
	 * \brief gets the near plane of the camera
	 */
	float get_near_plane() const;
	/**
	 * \brief gets the far plane of the camera
	 */
	float get_far_plane() const;

	
	XMMATRIX view_projection; // the projection of the camera	
	/**
	 * \brief calculates the view projection using current transform values
	 * \return XMMATRIX world
	 */
	XMMATRIX calculate_view_projection();

	void look_at(XMVECTOR target) const;

private:

	float fov_; // field of view
	float aspect_ratio_; // aspect ratio
	float near_plane_; // near plane
	float far_plane_; // far plane

	XMVECTOR look_at_; // look at
	XMMATRIX projection_; // projection
	XMMATRIX view_; // view


	//////////////////////////////////////////////////////////////////CONSTANTS///////////////////////////////////////////////////////////////////////////
	//FOV
	const float default_fov_ = 80.0f; // default field of view
	const float min_fov_ = 45.0f; // mininum field of view
	const float max_fov_ = 114.0f; // maximum field of view
	//ASPECT_RATIO
	const float default_aspect_ratio_ = 640.0f / 480.0f; // default aspect ratio
	const float min_aspect_ratio_ = 0.1f; // minimum aspect ratio
	const float max_aspect_ratio_ = 1.0f; // maximum aspect ratio
	//NEAR_PLANE
	const float default_near_plane_ = 0.1f; // default near plane
	const float min_near_plane_ = 1.0f; // minimum near plane
	const float max_near_plane_ = 15.0f; // maximum near plane
	//FAR_PLANE
	const float default_far_plane_ = 300.0f; // default far plane
	const float min_far_plane_ = 16.0f; // minimum far plane
	const float max_far_plane_ = 100.0f; // maximum far plane
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

};

