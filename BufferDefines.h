#pragma once

struct CONSTANT_BUFFER_SIMPLE
{
	XMMATRIX WorldViewProjection;
}; // 128 bytes

struct CONSTANT_BUFFER_LIGHTED
{
	XMMATRIX WorldViewProjection;
	XMVECTOR directional_light_vector; // 16 bytes
	XMVECTOR directional_light_colour; // 16 bytes
	XMVECTOR ambient_light_colour; // 16 bytes
}; // 128 bytes

struct CONSTANT_BUFFER_TIME_SCALED
{
	XMMATRIX WorldViewProjection; // 64 bytes
	XMVECTOR directional_light_vector; // 16 bytes
	XMVECTOR directional_light_colour; // 16 bytes
	XMVECTOR ambient_light_colour; // 16 bytes
	float gameTime; // 4 bytes
	float packing[3]; //12 bytes
	//XMFLOAT3 packing; //12 bytes
}; // 128 bytes

struct CONSTANT_BUFFER_FULL 
{
	XMMATRIX WorldViewProjection;
	XMVECTOR directional_light_vector; // 16 bytes
	XMVECTOR directional_light_colour; // 16 bytes
	XMVECTOR ambient_light_colour; // 16 bytes
	XMVECTOR rgb_amount;
	float gameTime; // 4 bytes	
}; // 128 bytes


