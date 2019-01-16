#include "Camera.h"



Camera::Camera()
{
	transform_ = new Transform(XMVectorSet(1,1,1,0),XMQuaternionIdentity(), XMVectorSet(0.0, 0.0, -20.0, 0.0));	
	//XMStoreFloat4x4(&world, transform.calculate_world_transform());
	view_ = XMMatrixIdentity();
	projection_ = XMMatrixPerspectiveFovLH(XMConvertToRadians(default_fov_), default_aspect_ratio_, default_near_plane_, default_far_plane_);
}


Camera::~Camera()
{
}

Camera::Camera(const LPCSTR name, const float fov, const float aspect_ratio, const float near_plane, const float far_plane) : GameObject(name), look_at_(XMVectorSet(0,0,0,0))
{
	transform_ = new Transform();
	view_ = XMMatrixIdentity();

	this->fov_ = fov;
	this->aspect_ratio_ = aspect_ratio;
	this->near_plane_ = near_plane;
	this->far_plane_ = far_plane;

	projection_ = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), aspect_ratio, near_plane, far_plane);
}

void Camera::set_fov(float fov)
{
	if (fov >= min_fov_ && fov <= max_fov_)
		this->fov_ = fov;
	else
		this->fov_ = default_fov_;
}

void Camera::set_aspect_ratio(float aspect_ratio)
{
	if (aspect_ratio >= min_aspect_ratio_ && aspect_ratio <= max_aspect_ratio_)
		this->aspect_ratio_ = aspect_ratio;
	else
		this->aspect_ratio_ = default_aspect_ratio_;
}

void Camera::set_near_plane(float near_plane)
{
	if (near_plane >= min_near_plane_ && near_plane <= max_near_plane_)
		this->near_plane_ = near_plane;
	else
		this->near_plane_ = default_near_plane_;
}

void Camera::set_far_plane(float far_plane)
{
	if (far_plane <= max_far_plane_ && far_plane >= min_far_plane_)
		this->far_plane_ = far_plane;
	else
		this->far_plane_ = default_far_plane_;
}


float Camera::get_fov() const
{
	return this->fov_;
}

float Camera::get_aspect_ratio() const
{
	return this->aspect_ratio_;
}

float Camera::get_near_plane() const
{
	return this->near_plane_;
}

float Camera::get_far_plane() const
{
	return this->far_plane_;
}


XMMATRIX Camera::calculate_view_projection()
{
	XMVECTOR determinant;
	view_ = XMMatrixInverse(&determinant, transform_->calculate_world_transform());
	view_projection = XMMatrixMultiply(view_, projection_);
	return view_projection;
}

void Camera::look_at(XMVECTOR target) const
{
}




