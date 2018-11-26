#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dx11.h>
#include <dxerr.h>
#include <xnamath.h>
#include <dinput.h>
//#include <Xinput.h>
//#include <iostream>
//#include <vector>
//#include <typeinfo>
//#include <io.h>


#include "VGTime.h"
#include "GameObject.h"
#include "Enemy.h"
#include "Player.h"
#include "Camera.h"
#include "objfilemodel.h"
#include "Model.h"
#include "d3dfw.h"
#include "Input.h"
#include "Geometry.h"

//using namespace std;



struct CONSTANT_BUFFER0
{
	XMMATRIX WorldViewProjection; // 64 bytes
	XMVECTOR directional_light_vector; // 16 bytes
	XMVECTOR directional_light_colour; // 16 bytes
	XMVECTOR ambient_light_colour; // 16 bytes
	float RedAmount; // 4 bytes
	float GreenAmount; // 4 bytes
	float BlueAmount; // 4 bytes
	float scale; // 4 bytes
	//XMFLOAT3 packing_bytes; // 12 bytes
}; // 128 bytes



// game objects
Camera* camera;
const int upperPlatformCount = 100; // 3000 * 3312 vertices seems to slow down the prozess when rotating -> consider optimizations for rotations
const int middlePlatformCount = 100;
const int lowerPlatformCount = 100;
GameObject upperPlatforms[upperPlatformCount];
GameObject middlePlatforms[middlePlatformCount];
GameObject lowerPlatforms[lowerPlatformCount];

Model *model_test;
Input input;

bool enable_glowing = false;


XMVECTOR directional_light_shines_from = XMVectorSet(0.0f,0.0f,-1.0f,0.0f);
XMVECTOR directional_light_colour = XMVectorSet(1.0f, 1.0f, 0.0f, 1.0f); // green
XMVECTOR ambient_light_colour = XMVectorSet(0.1f, 0.1f, 0.1f, 1.0f); // dark grey

CONSTANT_BUFFER0 cb0;

VGTime* timer;

HINSTANCE g_hInst = NULL;
HWND g_hWnd = NULL;



//g_Title is a replace for g_TutorialName
char g_Title[100] = "Swing to Win(g)";


HRESULT InitialiseWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void Cleanup();
void RenderFrame(void);
//void AlterVertices(POS_COL_VERTEX* vert, WPARAM message);


// methods
void UpdateAI();
void UpdateSound();
void UpdateGraphics();

void LoadContent();

//void MoveCamera();
const float cube_scale = 1.0f;

TexturedCube geo_cube;


POS_COL_VERTEX cube[] =
{
	// back face 
	{XMFLOAT3(-cube_scale, cube_scale, cube_scale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-cube_scale, -cube_scale, cube_scale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(cube_scale, cube_scale, cube_scale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(cube_scale, cube_scale, cube_scale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-cube_scale, -cube_scale, cube_scale),XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(cube_scale, -cube_scale, cube_scale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	// front face
	{XMFLOAT3(-cube_scale, -cube_scale, -cube_scale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-cube_scale, cube_scale, -cube_scale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(cube_scale, cube_scale, -cube_scale),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-cube_scale, -cube_scale, -cube_scale),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(cube_scale, cube_scale, -cube_scale),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(cube_scale, -cube_scale, -cube_scale),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	// left face
	{XMFLOAT3(-cube_scale, -cube_scale, -cube_scale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(-cube_scale, -cube_scale, cube_scale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(-cube_scale, cube_scale, -cube_scale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(-cube_scale, -cube_scale, cube_scale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(-cube_scale, cube_scale, cube_scale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(-cube_scale, cube_scale, -cube_scale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	// right face
	{XMFLOAT3(cube_scale, -cube_scale, cube_scale),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(cube_scale, -cube_scale, -cube_scale),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(cube_scale, cube_scale, -cube_scale),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(cube_scale, cube_scale, cube_scale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(cube_scale, -cube_scale, cube_scale),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(cube_scale, cube_scale, -cube_scale),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	// bottom face
	{XMFLOAT3(cube_scale, -cube_scale, -cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(cube_scale, -cube_scale, cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-cube_scale, -cube_scale, -cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(cube_scale, -cube_scale, cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-cube_scale, -cube_scale, cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-cube_scale, -cube_scale, -cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	// top face
	{XMFLOAT3(cube_scale, cube_scale, cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(cube_scale, cube_scale, -cube_scale),XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-cube_scale, cube_scale, -cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-cube_scale, cube_scale, cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(cube_scale, cube_scale, cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-cube_scale, cube_scale, -cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)}

};

POS_COL_TEX_VERTEX textured_cube[] =
{
	// back face 
	{XMFLOAT3(-cube_scale, cube_scale, cube_scale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f,0.0f)},
	{XMFLOAT3(-cube_scale, -cube_scale, cube_scale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f,1.0f)},
	{XMFLOAT3(cube_scale, cube_scale, cube_scale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,0.0f)},
	{XMFLOAT3(cube_scale, cube_scale, cube_scale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,0.0f)},
	{XMFLOAT3(-cube_scale, -cube_scale, cube_scale),XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f,1.0f)},
	{XMFLOAT3(cube_scale, -cube_scale, cube_scale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,1.0f)},
	// front face
	{XMFLOAT3(-cube_scale, -cube_scale, -cube_scale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f,1.0f)},
	{XMFLOAT3(-cube_scale, cube_scale, -cube_scale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f,0.0f)},
	{XMFLOAT3(cube_scale, cube_scale, -cube_scale),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,0.0f)},
	{XMFLOAT3(-cube_scale, -cube_scale, -cube_scale),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),XMFLOAT2(0.0f,1.0f)},
	{XMFLOAT3(cube_scale, cube_scale, -cube_scale),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,0.0f)},
	{XMFLOAT3(cube_scale, -cube_scale, -cube_scale),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,1.0f)},
	// left face
	{XMFLOAT3(-cube_scale, -cube_scale, -cube_scale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f,1.0f)},
	{XMFLOAT3(-cube_scale, -cube_scale, cube_scale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f,0.0f)},
	{XMFLOAT3(-cube_scale, cube_scale, -cube_scale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f,1.0f)},
	{XMFLOAT3(-cube_scale, -cube_scale, cube_scale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f,0.0f)},
	{XMFLOAT3(-cube_scale, cube_scale, cube_scale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f,0.0f)},
	{XMFLOAT3(-cube_scale, cube_scale, -cube_scale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f,1.0f)},
	// right face
	{XMFLOAT3(cube_scale, -cube_scale, cube_scale),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f,0.0f)},
	{XMFLOAT3(cube_scale, -cube_scale, -cube_scale),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f,1.0f)},
	{XMFLOAT3(cube_scale, cube_scale, -cube_scale),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f,1.0f)},
	{XMFLOAT3(cube_scale, cube_scale, cube_scale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f,0.0f)},
	{XMFLOAT3(cube_scale, -cube_scale, cube_scale),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f,0.0f)},
	{XMFLOAT3(cube_scale, cube_scale, -cube_scale),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f,1.0f)},
	// bottom face
	{XMFLOAT3(cube_scale, -cube_scale, -cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,1.0f)},
	{XMFLOAT3(cube_scale, -cube_scale, cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,0.0f)},
	{XMFLOAT3(-cube_scale, -cube_scale, -cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),XMFLOAT2(0.0f,1.0f)},
	{XMFLOAT3(cube_scale, -cube_scale, cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),XMFLOAT2(1.0f,0.0f)},
	{XMFLOAT3(-cube_scale, -cube_scale, cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),XMFLOAT2(0.0f,0.0f)},
	{XMFLOAT3(-cube_scale, -cube_scale, -cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),XMFLOAT2(0.0f,1.0f)},
	// top face
	{XMFLOAT3(cube_scale, cube_scale, cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,0.0f)},
	{XMFLOAT3(cube_scale, cube_scale, -cube_scale),XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,1.0f)},
	{XMFLOAT3(-cube_scale, cube_scale, -cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f,1.0f)},
	{XMFLOAT3(-cube_scale, cube_scale, cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f,0.0f)},
	{XMFLOAT3(cube_scale, cube_scale, cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,0.0f)},
	{XMFLOAT3(-cube_scale, cube_scale, -cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f,1.0f)}
};

POS_COL_TEX_NORM_VERTEX textured_normal_cube[] =
{
	// back face 
	{XMFLOAT3(-cube_scale, cube_scale, cube_scale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f,0.0f), XMFLOAT3(0.0f,0.0f,1.0f)},
	{XMFLOAT3(-cube_scale, -cube_scale, cube_scale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,0.0f,1.0f)},
	{XMFLOAT3(cube_scale, cube_scale, cube_scale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,0.0f,1.0f)},
	{XMFLOAT3(cube_scale, cube_scale, cube_scale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,0.0f,1.0f)},
	{XMFLOAT3(-cube_scale, -cube_scale, cube_scale),XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,0.0f,1.0f)},
	{XMFLOAT3(cube_scale, -cube_scale, cube_scale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,1.0f), XMFLOAT3(0.0f,0.0f,1.0f)},
	// front face
	{XMFLOAT3(-cube_scale, -cube_scale, -cube_scale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,0.0f,-1.0f)},
	{XMFLOAT3(-cube_scale, cube_scale, -cube_scale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f,0.0f), XMFLOAT3(0.0f,0.0f,-1.0f)},
	{XMFLOAT3(cube_scale, cube_scale, -cube_scale),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,0.0f,-1.0f)},
	{XMFLOAT3(-cube_scale, -cube_scale, -cube_scale),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,0.0f,-1.0f)},
	{XMFLOAT3(cube_scale, cube_scale, -cube_scale),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,0.0f,-1.0f)},
	{XMFLOAT3(cube_scale, -cube_scale, -cube_scale),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,1.0f), XMFLOAT3(0.0f,0.0f,-1.0f)},
	// left face
	{XMFLOAT3(-cube_scale, -cube_scale, -cube_scale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f,1.0f), XMFLOAT3(-1.0f,0.0f,0.0f)},
	{XMFLOAT3(-cube_scale, -cube_scale, cube_scale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f,0.0f), XMFLOAT3(-1.0f,0.0f,0.0f)},
	{XMFLOAT3(-cube_scale, cube_scale, -cube_scale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f,1.0f), XMFLOAT3(-1.0f,0.0f,0.0f)},
	{XMFLOAT3(-cube_scale, -cube_scale, cube_scale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f,0.0f), XMFLOAT3(-1.0f,0.0f,0.0f)},
	{XMFLOAT3(-cube_scale, cube_scale, cube_scale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f,0.0f), XMFLOAT3(-1.0f,0.0f,0.0f)},
	{XMFLOAT3(-cube_scale, cube_scale, -cube_scale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f,1.0f), XMFLOAT3(-1.0f,0.0f,0.0f)},
	// right face
	{XMFLOAT3(cube_scale, -cube_scale, cube_scale),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f,0.0f), XMFLOAT3(1.0f,0.0f,0.0f)},
	{XMFLOAT3(cube_scale, -cube_scale, -cube_scale),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f,1.0f), XMFLOAT3(1.0f,0.0f,0.0f)},
	{XMFLOAT3(cube_scale, cube_scale, -cube_scale),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f,1.0f), XMFLOAT3(1.0f,0.0f,0.0f)},
	{XMFLOAT3(cube_scale, cube_scale, cube_scale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f,0.0f), XMFLOAT3(1.0f,0.0f,0.0f)},
	{XMFLOAT3(cube_scale, -cube_scale, cube_scale),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f,0.0f), XMFLOAT3(1.0f,0.0f,0.0f)},
	{XMFLOAT3(cube_scale, cube_scale, -cube_scale),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f,1.0f), XMFLOAT3(1.0f,0.0f,0.0f)},
	// bottom face
	{XMFLOAT3(cube_scale, -cube_scale, -cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,1.0f), XMFLOAT3(0.0f,-1.0f,0.0f)},
	{XMFLOAT3(cube_scale, -cube_scale, cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,-1.0f,0.0f)},
	{XMFLOAT3(-cube_scale, -cube_scale, -cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,-1.0f,0.0f)},
	{XMFLOAT3(cube_scale, -cube_scale, cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,-1.0f,0.0f)},
	{XMFLOAT3(-cube_scale, -cube_scale, cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),XMFLOAT2(0.0f,0.0f), XMFLOAT3(0.0f,-1.0f,0.0f)},
	{XMFLOAT3(-cube_scale, -cube_scale, -cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,-1.0f,0.0f)},
	// top face
	{XMFLOAT3(cube_scale, cube_scale, cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,1.0f,0.0f)},
	{XMFLOAT3(cube_scale, cube_scale, -cube_scale),XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,1.0f), XMFLOAT3(0.0f,1.0f,0.0f)},
	{XMFLOAT3(-cube_scale, cube_scale, -cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,1.0f,0.0f)},
	{XMFLOAT3(-cube_scale, cube_scale, cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f,0.0f), XMFLOAT3(0.0f,1.0f,0.0f)},
	{XMFLOAT3(cube_scale, cube_scale, cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,1.0f,0.0f)},
	{XMFLOAT3(-cube_scale, cube_scale, -cube_scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,1.0f,0.0f)}
};

const float skyboxScale = 20.0f;
POS_COL_VERTEX skybox[] =
{
	// back face 
	{XMFLOAT3(-skyboxScale, -skyboxScale, skyboxScale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-skyboxScale, skyboxScale, skyboxScale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(skyboxScale, skyboxScale, skyboxScale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-skyboxScale, -skyboxScale, skyboxScale),XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(skyboxScale, skyboxScale, skyboxScale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(skyboxScale, -skyboxScale, skyboxScale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	// front face
	{XMFLOAT3(-skyboxScale, skyboxScale, -skyboxScale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-skyboxScale, -skyboxScale, -skyboxScale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(skyboxScale, skyboxScale, -skyboxScale),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(skyboxScale, skyboxScale, -skyboxScale),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-skyboxScale, -skyboxScale, -skyboxScale),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(skyboxScale, -skyboxScale, -skyboxScale),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	// left face
	{XMFLOAT3(-skyboxScale, -skyboxScale, skyboxScale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(-skyboxScale, -skyboxScale, -skyboxScale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(-skyboxScale, skyboxScale, -skyboxScale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(-skyboxScale, skyboxScale, skyboxScale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(-skyboxScale, -skyboxScale, skyboxScale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(-skyboxScale, skyboxScale, -skyboxScale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	// right face
	{XMFLOAT3(skyboxScale, -skyboxScale, -skyboxScale),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(skyboxScale, -skyboxScale, skyboxScale),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(skyboxScale, skyboxScale, -skyboxScale),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(skyboxScale, -skyboxScale, skyboxScale),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(skyboxScale, skyboxScale, skyboxScale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(skyboxScale, skyboxScale, -skyboxScale),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
	// bottom face
	{XMFLOAT3(skyboxScale, -skyboxScale, skyboxScale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(skyboxScale, -skyboxScale, -skyboxScale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-skyboxScale, -skyboxScale, -skyboxScale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-skyboxScale, -skyboxScale, skyboxScale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(skyboxScale, -skyboxScale, skyboxScale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-skyboxScale, -skyboxScale, -skyboxScale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	// top face
	{XMFLOAT3(skyboxScale, skyboxScale, -skyboxScale),XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(skyboxScale, skyboxScale, skyboxScale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-skyboxScale, skyboxScale, -skyboxScale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(skyboxScale, skyboxScale, skyboxScale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-skyboxScale, skyboxScale, skyboxScale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-skyboxScale, skyboxScale, -skyboxScale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)}

};

POS_COL_VERTEX shape_1[] =
{
	{XMFLOAT3(0.2f,0.2f,0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(0.9f,-0.9f,0.0f), XMFLOAT4(0.2f, 0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(-0.9f,-0.9f,0.0f), XMFLOAT4(0.0f, 0.5f, 0.2f, 0.5f)}
};



d3dfw* dx_handle;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Geometry::create_cube(1.0f, &geo_cube);

	dx_handle = d3dfw::getInstance();
	ShowCursor(false);
	camera = new Camera();

	timer = new VGTime();
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (FAILED(dx_handle->InitialiseWindow(hInstance, nCmdShow, WndProc)))
	{
		DXTRACE_MSG("Failed to create Window");
		return 0;
	}

	MSG msg = { 0 };

	if (FAILED(dx_handle->InitialiseD3D()))
	{
		DXTRACE_MSG("Failed to initialise DirectX");
		return 0;
	}

	if (FAILED(dx_handle->InitialiseInput()))
	{
		DXTRACE_MSG("Failed to initialise Input");
		return 0;
	}
	
	char filename[] = "assets/Sphere.obj";
	model_test = new Model(dx_handle->device, dx_handle->immediateContext, filename);


	LoadContent();

	timer->start();

	while (msg.message != WM_QUIT)
	{
		timer->tick();
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			//UpdateAI();
			dx_handle->input->UpdateInput(camera,timer);
			//UpdateSound();
			//UpdateGraphics();
			RenderFrame();
		}
	}

	Cleanup();
	return (int)msg.wParam;
}

void RenderFrame(void)
{
	const XMMATRIX view_projection = camera->calculate_view_projection();
	
	// clear the render target view
	dx_handle->ClearRTV();

	// draw here
	for (size_t i = 0; i < upperPlatformCount; i++)
	{
		upperPlatforms[i].Draw(view_projection/*,D3D11_PRIMITIVE_TOPOLOGY_POINTLIST*/);
	}

	// swap back buffer with front buffer
	dx_handle->swapChain->Present(0, 0);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
		//case WM_KEYDOWN:
		//	//AlterVertices(shape_1, wParam);
		//	//MoveCamera(wParam);
		//	if (wParam == VK_ESCAPE)
		//		DestroyWindow(g_hWnd);
		//case WM_MOUSEMOVE:
		//	//camera->transform.rotate(0, abs(pos.x) - abs(x), 0);
		//	return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}


void Cleanup()
{
	dx_handle->Cleanup();
	model_test->Cleanup();

	timer->stop();
}

void UpdateAI()
{

}



void UpdateSound()
{

}

void UpdateGraphics()
{

}

void LoadContent()
{
	XMVECTOR scale = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
	XMVECTOR rotation = XMQuaternionIdentity();
//	XMVECTOR position = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
	for (size_t i = 0; i < upperPlatformCount; i++)
	{
		upperPlatforms[i] = GameObject("upperPlatform" + i, Transform(scale, rotation, XMVectorSet(i, i, 1.0f, 0.0f)), *model_test);	
	}

	for (size_t i = 0; i < middlePlatformCount; i++)
	{

	}

	for (size_t i = 0; i < lowerPlatformCount; i++)
	{

	}
}


//void AlterVertices(POS_COL_VERTEX* vert, WPARAM message) {
//
//	int size = sizeof(vert);
//	//OutputDebugString("" + message);
//
//	switch (message)
//	{
//	case VK_RIGHT:
//		for (int i = 0; i < size; i++)
//		{
//			vert[i].Pos.x += 0.01f;
//		}
//		break;
//	case VK_LEFT:
//		for (int i = 0; i < size; i++)
//		{
//			vert[i].Pos.x -= 0.01f;
//		}
//		break;
//	case VK_UP:
//		for (int i = 0; i < size; i++)
//		{
//			vert[i].Pos.y += 0.01f;
//		}
//		break;
//	case VK_DOWN:
//		for (int i = 0; i < size; i++)
//		{
//			vert[i].Pos.y -= 0.01f;
//		}
//		break;
//	}
//
//
//	D3D11_MAPPED_SUBRESOURCE ms;
//
//	// Lock the buffer to allow writing
//	g_pImmediateContext->Map(g_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
//
//	// copy the data
//	memcpy(ms.pData, cube, sizeof(cube));
//
//	// unlock the buffer
//	g_pImmediateContext->Unmap(g_pVertexBuffer, NULL);
//}