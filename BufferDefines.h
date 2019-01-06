#pragma once

// constant buffer containing world matrix
// size: 64 bytes
struct CONSTANT_BUFFER_SIMPLE
{
	XMMATRIX world_view_projection; // 64 bytes
}; 

// constant buffer containing world matrix, camera view matrix, light vector, light color, ambient light color
// size: 112 bytes
struct CONSTANT_BUFFER_LIGHTED
{
	XMMATRIX world_view_projection; // 64 bytes
	XMMATRIX view_projection; // 64 bytes
	XMVECTOR directional_light_vector; // 16 bytes
	XMVECTOR directional_light_colour; // 16 bytes
	XMVECTOR ambient_light_colour; // 16 bytes
};

// constant buffer containing world matrix, camera view matrix, light vector, light color, ambient light color, game time
// size: 128 bytes
struct CONSTANT_BUFFER_TIME_SCALED
{
	XMMATRIX world_view_projection; // 64 bytes	
	XMMATRIX view_projection; // 64 bytes
	XMVECTOR directional_light_vector; // 16 bytes
	XMVECTOR directional_light_colour; // 16 bytes
	XMVECTOR ambient_light_colour; // 16 bytes
	float game_time; // 4 bytes
	float packing[3]; //12 bytes
}; 

// constant buffer containing world matrix, camera view matrix, light vector, light color, ambient light color, color modifier, game time
// size: 144 bytes
struct CONSTANT_BUFFER_FULL 
{
	XMMATRIX world_view_projection;// 64 bytes
	XMMATRIX view_projection; // 64 bytes
	XMVECTOR directional_light_vector; // 16 bytes
	XMVECTOR directional_light_colour; // 16 bytes
	XMVECTOR ambient_light_colour; // 16 bytes
	XMVECTOR rgb_amount; // 16 bytes
	float game_time; // 4 bytes	
	float packing[3]; //12 bytes
}; 


