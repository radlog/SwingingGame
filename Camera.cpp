#include "Camera.h"



Camera::Camera()
{
	projection_ = (XMMatrixPerspectiveFovLH(XMConvertToRadians(default_fov_), default_aspect_ratio_, default_near_plane_, default_far_plane_));
	view_ = XMMatrixIdentity();
	transform_ = new Transform();	
}


Camera::~Camera()
{
}

Camera::Camera(const LPCSTR name, const float fov, const float aspect_ratio, const float near_plane, const float far_plane) : GameObject(name)
{		
	transform_ = new Transform();
	view_ = XMMatrixIdentity();

	fov_ = fov;
	aspect_ratio_ = aspect_ratio;
	near_plane_ = near_plane;
	far_plane_ = far_plane;

	projection_ = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), aspect_ratio, near_plane, far_plane);
}

// set field of view
void Camera::set_fov(const float fov)
{
	if (fov >= min_fov_ && fov <= max_fov_)
		this->fov_ = fov;
	else
		this->fov_ = default_fov_;
}

// set aspect ratio
void Camera::set_aspect_ratio(const float aspect_ratio)
{
	if (aspect_ratio >= min_aspect_ratio_ && aspect_ratio <= max_aspect_ratio_)
		this->aspect_ratio_ = aspect_ratio;
	else
		this->aspect_ratio_ = default_aspect_ratio_;
}

// set near plane distance
void Camera::set_near_plane(const float near_plane)
{
	if (near_plane >= min_near_plane_ && near_plane <= max_near_plane_)
		this->near_plane_ = near_plane;
	else
		this->near_plane_ = default_near_plane_;
}

// set far plane distance
void Camera::set_far_plane(const float far_plane)
{
	if (far_plane <= max_far_plane_ && far_plane >= min_far_plane_)
		this->far_plane_ = far_plane;
	else
		this->far_plane_ = default_far_plane_;
}

// get field of view
float Camera::get_fov() const
{
	return this->fov_;
}

// get aspect ratio
float Camera::get_aspect_ratio() const
{
	return this->aspect_ratio_;
}

// get near plane distance
float Camera::get_near_plane() const
{
	return this->near_plane_;
}

// set near plane distance
float Camera::get_far_plane() const
{
	return this->far_plane_;
}

// calculate the view projection
XMMATRIX Camera::calculate_view_projection()
{
	XMVECTOR determinant;
	view_ = XMMatrixInverse(&determinant, transform_->calculate_world_transform()); // inverse view to get the correct transform of the camera
	view_projection = XMMatrixMultiply(view_, projection_); // multiply view with projection to get the view projection
	return view_projection;
}



