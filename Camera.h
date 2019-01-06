#pragma once
#include "GameObject.h"

class Camera : public GameObject
{
public:
	Camera();
	~Camera();

	explicit Camera(LPCSTR name, float fov = 80.0f, float aspect_ratio = 640.0f / 480.0f, float near_plane = 0.1f, float far_plane = 300.0f);

	void set_fov(float fov);
	void set_aspect_ratio(float aspect_ratio);
	void set_near_plane(float near_plane);
	void set_far_plane(float far_plane);

	float get_fov() const;
	float get_aspect_ratio() const;
	float get_near_plane() const;
	float get_far_plane() const;

	XMMATRIX view_projection;
	XMMATRIX calculate_view_projection();

	void look_at(XMVECTOR target) const;

private:

	float fov_;
	float aspect_ratio_;
	float near_plane_;
	float far_plane_;

	XMVECTOR look_at_;
	XMMATRIX projection_, view_;


	//////////////////////////////////////////////////////////////////CONSTANTS///////////////////////////////////////////////////////////////////////////
	//FOV
	const float default_fov_ = 80.0f;
	const float min_fov_ = 45.0f;
	const float max_fov_ = 114.0f;
	//ASPECT_RATIO
	const float default_aspect_ratio_ = 640.0f/480.0f;
	const float min_aspect_ratio_ = 0.1f;
	const float max_aspect_ratio_ = 1.0f;
	//NEAR_PLANE
	const float default_near_plane_ = 0.1f;
	const float min_near_plane_ = 1.0f;
	const float max_near_plane_ = 15.0f;
	//FAR_PLANE
	const float default_far_plane_ = 300.0f;
	const float min_far_plane_ = 16.0f;
	const float max_far_plane_ = 100.0f;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

};

