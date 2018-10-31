#include "Camera.h"



Camera::Camera()
{
	transform = Transform();
	view = XMMatrixIdentity();
	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(default_fov), default_aspect_ratio, default_near_plane, default_far_plane);
}


Camera::~Camera()
{
}

Camera::Camera(float fov, float aspect_ratio, float near_plane, float far_plane)
{
	transform = Transform();
	view = XMMatrixIdentity();

	set_fov(fov);
	set_aspect_ratio(aspect_ratio);
	set_near_plane(near_plane);
	set_far_plane(far_plane);

	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(get_fov()), get_aspect_ratio(), get_near_plane(), get_far_plane());
}

void Camera::set_fov(float fov)
{
	if (fov >= min_fov && fov <= max_fov)
		this->fov = fov;
	else
		this->fov = default_fov;
}

void Camera::set_aspect_ratio(float aspect_ratio)
{
	if (aspect_ratio >= min_aspect_ratio && aspect_ratio <= max_aspect_ratio)
		this->aspect_ratio = aspect_ratio;
	else
		this->aspect_ratio = default_aspect_ratio;
}

void Camera::set_near_plane(float near_plane)
{
	if (near_plane >= min_near_plane && near_plane <= max_near_plane)
		this->near_plane = near_plane;
	else
		this->near_plane = default_near_plane;
}

void Camera::set_far_plane(float far_plane)
{
	if (far_plane <= max_far_plane && far_plane >= min_far_plane)
		this->far_plane = far_plane;
	else
		this->far_plane = default_far_plane;
}

float Camera::get_fov()
{
	return this->fov;
}

float Camera::get_aspect_ratio()
{
	return this->aspect_ratio;
}

float Camera::get_near_plane()
{
	return this->near_plane;
}

float Camera::get_far_plane()
{
	return this->far_plane;
}

XMMATRIX Camera::get_world_view_projection()
{
	world_view_projection = transform.get_world_transform() * view * projection;
	return world_view_projection;
}


