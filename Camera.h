#pragma once
#include "Transform.h"
class Camera
{
public:
	Camera();
	~Camera();

	Camera(float fov, float aspect_ratio, float near_plane, float far_plane);

	void set_fov(float fov);
	void set_aspect_ratio(float aspect_ratio);
	void set_near_plane(float near_plane);
	void set_far_plane(float far_plane);

	float get_fov() const;
	float get_aspect_ratio() const;
	float get_near_plane() const;
	float get_far_plane() const;
	
	XMMATRIX get_view_projection();
	Transform transform;

	void look_at(XMVECTOR target);

private:



	float fov_;
	float aspect_ratio_;
	float near_plane_;
	float far_plane_;

	XMVECTOR look_at_;
	XMMATRIX view_projection_;
	XMMATRIX projection_, view_;


	//////////////////////////////////////////////////////////////////CONSTANTS///////////////////////////////////////////////////////////////////////////
	//FOV
	const float default_fov_ = 80.0;
	const float min_fov_ = 45.0;
	const float max_fov_ = 114.0;
	//ASPECT_RATIO
	const float default_aspect_ratio_ = 640.0/480.0;
	const float min_aspect_ratio_ = 0.1;
	const float max_aspect_ratio_ = 1.0;
	//NEAR_PLANE
	const float default_near_plane_ = 1.0;
	const float min_near_plane_ = 1.0;
	const float max_near_plane_ = 15.0;
	//FAR_PLANE
	const float default_far_plane_ = 100.0;
	const float min_far_plane_ = 16.0;
	const float max_far_plane_ = 100.0;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

};

