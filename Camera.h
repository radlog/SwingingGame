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

	float get_fov();
	float get_aspect_ratio();
	float get_near_plane();
	float get_far_plane();



	XMVECTOR lookat;

	XMMATRIX get_world_view_projection();
	XMMATRIX world, view, projection, world_view_projection;

	Transform transform;


	void look_at(XMVECTOR target);

private:



	float fov;
	float aspect_ratio;
	float near_plane;
	float far_plane;


	//////////////////////////////////////////////////////////////////CONSTANTS///////////////////////////////////////////////////////////////////////////
	//FOV
	const float default_fov = 80.0;
	const float min_fov = 45.0;
	const float max_fov = 114.0;
	//ASPECT_RATIO
	const float default_aspect_ratio = 640.0/480.0;
	const float min_aspect_ratio = 0.1;
	const float max_aspect_ratio = 1.0;
	//NEAR_PLANE
	const float default_near_plane = 1.0;
	const float min_near_plane = 1.0;
	const float max_near_plane = 15.0;
	//FAR_PLANE
	const float default_far_plane = 100.0;
	const float min_far_plane = 16.0;
	const float max_far_plane = 100.0;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

};

