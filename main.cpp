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

//using namespace std;

struct POS_COL_VERTEX
{
	XMFLOAT3 Pos;
	XMFLOAT4 Col;
};

struct POS_COL_TEX_VERTEX
{
	XMFLOAT3 Pos;
	XMFLOAT4 Col;
	XMFLOAT2 Texture0;
};

struct POS_COL_TEX_NORM_VERTEX
{
	XMFLOAT3 Pos;
	XMFLOAT4 Col;
	XMFLOAT2 Texture0;
	XMFLOAT3 Normal;
};

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
Model *model_test;
//std::vector<GameObject> gameObjects(2);

bool enable_glowing = false;
const float rot_speed = 10.0f;
const float move_speed = 10.0f;
const float look_speed = 10.0f;

float mouse_x_center = 480.0f;
float mouse_y_center = 640.0f;

float mouse_x = mouse_x_center;
float mouse_y = mouse_y_center;

XMVECTOR directional_light_shines_from = XMVectorSet(0.0f,0.0f,-1.0f,0.0f);
XMVECTOR directional_light_colour = XMVectorSet(1.0f, 1.0f, 0.0f, 1.0f); // green
XMVECTOR ambient_light_colour = XMVectorSet(0.1f, 0.1f, 0.1f, 1.0f); // dark grey

CONSTANT_BUFFER0 cb0;

VGTime* timer;

HINSTANCE g_hInst = NULL;
HWND g_hWnd = NULL;

D3D_DRIVER_TYPE g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device* device = NULL;
ID3D11DeviceContext* immediateContext = NULL;
IDXGISwapChain* g_pSwapChain = NULL;
ID3D11RenderTargetView* g_pBackBufferRTView = NULL;
ID3D11Buffer* g_pConstantBuffer0;

ID3D11Buffer* g_pVertexBuffer;
ID3D11VertexShader* g_pVertexShader;
ID3D11PixelShader* g_pPixelShader;
ID3D11InputLayout* g_pInputLayout;
ID3D11DepthStencilView* g_pZBuffer;
ID3D11SamplerState * g_pSampler0;
IDirectInput8 *g_direct_input;
IDirectInputDevice8 *g_keyboard_device;
unsigned char g_keyboard_keys_state[256];

IDirectInputDevice8 *mouse_input;
DIMOUSESTATE mouse_state;

ID3D11ShaderResourceView *texture;


//g_Title is a replace for g_TutorialName
char g_Title[100] = "Swing to Win(g)";


HRESULT InitialiseWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT InitialiseD3D();
HRESULT InitialiseGraphics(void);
HRESULT InitialiseInput();
void ShutdownD3D();
void RenderFrame(void);
//void AlterVertices(POS_COL_VERTEX* vert, WPARAM message);


// methods
void UpdateAI();
HRESULT UpdateInput();
void UpdateSound();
void UpdateGraphics();
bool IsKeyPressed(unsigned char DI_keycode);
void MouseMoved();

//void MoveCamera();
const float cube_scale = 1.0f;

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


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	ShowCursor(false);
	camera = new Camera();

	timer = new VGTime();
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (FAILED(InitialiseWindow(hInstance, nCmdShow)))
	{
		DXTRACE_MSG("Failed to create Window");
		return 0;
	}

	MSG msg = { 0 };

	if (FAILED(InitialiseD3D()))
	{
		DXTRACE_MSG("Failed to initialise DirectX");
		return 0;
	}

	if (FAILED(InitialiseGraphics()))
	{
		DXTRACE_MSG("Failed to initialise graphics");
		return 0;
	}


	if (FAILED(InitialiseInput()))
	{
		DXTRACE_MSG("Failed to initialise Input");
		return 0;
	}

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
			UpdateInput();
			//UpdateSound();
			//UpdateGraphics();
			RenderFrame();
		}

	}

	timer->stop();
	return (int)msg.wParam;
}

void RenderFrame(void)
{
	double delta = timer->deltaTime();
	//enable_glowing = true;
	if (enable_glowing)
	{
		static bool glow;
		delta *= 10;
		if (cb0.RedAmount <= 0.01f || cb0.GreenAmount <= 0.01f || cb0.BlueAmount <= 0.01f) glow = true;
		else if (cb0.RedAmount >= 0.99f || cb0.GreenAmount >= 0.99f || cb0.BlueAmount >= 0.99f) glow = false;

		if (glow)
		{
			cb0.RedAmount += 0.1f * delta;
			cb0.GreenAmount += 0.1f * delta;
			cb0.BlueAmount += 0.1f * delta;
		}
		else
		{
			cb0.RedAmount -= 0.1f * delta;
			cb0.GreenAmount -= 0.1f * delta;
			cb0.BlueAmount -= 0.1f * delta;
		}
	}
	else
	{
		cb0.RedAmount = 1.0f;
		cb0.GreenAmount = 1.0f;
		cb0.BlueAmount = 1.0f;
	}

	//camera->look_at(XMVectorSet(0.0, 0.0, -4.0, 0.0));

	const auto view_projection = camera->get_view_projection();
	const auto cube_rotation = XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(timer->totalTime() * 4, timer->totalTime() * 2, timer->totalTime() * 3));
	
	//cb0.WorldViewProjection = cube_rotation * view_projection;
	cb0.WorldViewProjection = XMMatrixIdentity() * view_projection;

	XMMATRIX transpose;
	transpose = XMMatrixTranspose(cb0.WorldViewProjection);

	cb0.directional_light_colour = directional_light_colour;
	cb0.ambient_light_colour = ambient_light_colour;
	cb0.directional_light_vector = XMVector3Normalize( XMVector3Transform(directional_light_shines_from, transpose));

	//immediateContext->UpdateSubresource(g_pConstantBuffer0, 0, nullptr, &cb0, 0, 0);

	//immediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer0);

	//float rgba_clear_colour[4] = { 0.1f,0.2f,0.6f,1.0f };
	//float rgba_clear_colour[4] = { 0.1f,0.1f,0.5f,1.0f };
	float rgba_clear_colour[4] = { 0.0f,0.0f,0.0f,1.0f };
	immediateContext->ClearRenderTargetView(g_pBackBufferRTView, rgba_clear_colour);
	immediateContext->ClearDepthStencilView(g_pZBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//UINT stride = sizeof(POS_COL_TEX_NORM_VERTEX);
	//UINT offset = 0;
	//immediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	immediateContext->PSSetSamplers(0, 0, &g_pSampler0);
	immediateContext->PSSetShaderResources(0, 1, &texture);

	//immediateContext->Draw(36, 0);
	model_test->Draw(view_projection);

	// RENDER HERE

	g_pSwapChain->Present(0, 0);
}


HRESULT InitialiseGraphics()
{
	HRESULT hr = S_OK;
	
	char filename[] = "assets/Sphere.obj";
	model_test = new Model(device, immediateContext,filename);	
	



	// create constant buffer
	D3D11_BUFFER_DESC constant_buffer_desc;
	ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));

	constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT; // can use UpdateSubresrource() to update
	constant_buffer_desc.ByteWidth = sizeof(cb0); // MUST be a multiple of 16, calculate from CB struct
	constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	hr = device->CreateBuffer(&constant_buffer_desc, NULL, &g_pConstantBuffer0);

	if (FAILED(hr))
		return hr;

	// Set up and create vertex buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC; //Dynamic -> used by CPU and GPU
	bufferDesc.ByteWidth = sizeof(textured_normal_cube); // Size of the buffer, 3 vertices
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // use as a vertex buffer
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // allow cpu access
	hr = device->CreateBuffer(&bufferDesc, NULL, &g_pVertexBuffer); // create buffer

	if (FAILED(hr)) {
		return hr;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;



	// Lock the buffer to allow writing
	immediateContext->Map(g_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);

	// copy the data
	memcpy(ms.pData, textured_normal_cube, sizeof(textured_normal_cube));

	// unlock the buffer
	immediateContext->Unmap(g_pVertexBuffer, NULL);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// load and compile pixel and vertex shaders - use vs_5_0 to target DX11 hardware only
	ID3DBlob *VS, *PS, *error;
	hr = D3DX11CompileFromFile("shaders.hlsl", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, &error, 0);
	hr = D3DX11CompileFromFile("shaders.hlsl", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, &error, 0);

	// create shader objects
	hr = device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &g_pVertexShader);
	hr = device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &g_pPixelShader);

	// set the shader objects as active
	immediateContext->VSSetShader(g_pVertexShader, 0, 0);
	immediateContext->PSSetShader(g_pPixelShader, 0, 0);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// create and set the input layout object
	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		{"POSITION", 0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0}
	};

	hr = device->CreateInputLayout(iedesc, ARRAYSIZE(iedesc), VS->GetBufferPointer(), VS->GetBufferSize(), &g_pInputLayout);

	immediateContext->IASetInputLayout(g_pInputLayout);


	D3DX11CreateShaderResourceViewFromFile(device, "assets/crate.jpg", nullptr, nullptr, &texture, nullptr);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	D3D11_SAMPLER_DESC  sampler_desc;
	ZeroMemory(&sampler_desc, sizeof(sampler_desc));
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
}

HRESULT InitialiseInput()
{
	HRESULT hr;
	ZeroMemory(g_keyboard_keys_state, sizeof(g_keyboard_keys_state));

	// initialise input factory
	hr = DirectInput8Create(g_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_direct_input, NULL);
	if (FAILED(hr)) return hr;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////keyboard init////////////////////////////////////////////////////////////////
	// create keyboard
	hr = g_direct_input->CreateDevice(GUID_SysKeyboard, &g_keyboard_device, NULL);
	if (FAILED(hr)) return hr;

	// set keyboard format
	hr = g_keyboard_device->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr)) return hr;

	// set keyboards behaviour of interaction between other instances of the same device on different processes and the system
	hr = g_keyboard_device->SetCooperativeLevel(g_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr)) return hr;

	// acquire the setup device
	hr = g_keyboard_device->Acquire();
	if (FAILED(hr)) return hr;

	////////////////////////////////////////////////////////keyboard init////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////mouse init///////////////////////////////////////////////////////////////////
	// create mouse
	hr = g_direct_input->CreateDevice(GUID_SysMouse, &mouse_input, NULL);
	if (FAILED(hr)) return hr;

	// set data format for the mouse
	hr = mouse_input->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr)) return hr;

	// set interaction of mouse between processes and other instances of the same device
	hr = mouse_input->SetCooperativeLevel(g_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr)) return hr;

	// acquire setup mouse
	hr = mouse_input->Acquire();
	if (FAILED(hr)) return hr;

	////////////////////////////////////////////////////////mouse init///////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}


HRESULT InitialiseWindow(HINSTANCE hInstance, int nCmdShow)
{

	// window title
	char Name[100] = "Swing around\0";

	// Register class
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Needed for non-D3D apps
	wcex.lpszClassName = Name;

	if (!RegisterClassEx(&wcex)) return E_FAIL;

	// Create window
	g_hInst = hInstance;
	RECT rc = { 0, 0, 640, 480 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	g_hWnd = CreateWindow(Name, g_Title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left,
		rc.bottom - rc.top, NULL, NULL, hInstance, NULL);
	if (!g_hWnd)
		return E_FAIL;

	ShowWindow(g_hWnd, nCmdShow);

	return S_OK;
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

HRESULT InitialiseD3D() {
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(g_hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;


	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE, // comment out this line if you need to test D3D 11.0 functionality on hardware that doesn't support it
		D3D_DRIVER_TYPE_WARP, // comment this out also to use reference device
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL,
			createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &g_pSwapChain,
			&device, &g_featureLevel, &immediateContext);
		if (SUCCEEDED(hr))
			break;
	}

	if (FAILED(hr))
		return hr;



	ID3D11Texture2D* pBackBufferTexture = NULL;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBufferTexture);

	if (FAILED(hr)) return hr;

	hr = device->CreateRenderTargetView(pBackBufferTexture, NULL, &g_pBackBufferRTView);

	pBackBufferTexture->Release();

	if (FAILED(hr)) return hr;

	// create z buffer texture
	D3D11_TEXTURE2D_DESC tex2dDesc;
	ZeroMemory(&tex2dDesc, sizeof(tex2dDesc));

	tex2dDesc.Width = width;
	tex2dDesc.Height = height;
	tex2dDesc.ArraySize = 1;
	tex2dDesc.MipLevels = 1;
	tex2dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tex2dDesc.SampleDesc.Count = sd.SampleDesc.Count;
	tex2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex2dDesc.Usage = D3D11_USAGE_DEFAULT;

	ID3D11Texture2D *pZBufferTexture;
	hr = device->CreateTexture2D(&tex2dDesc, NULL, &pZBufferTexture);

	if (FAILED(hr)) return hr;

	// create z buffer
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));

	dsvDesc.Format = tex2dDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	device->CreateDepthStencilView(pZBufferTexture, &dsvDesc, &g_pZBuffer);
	pZBufferTexture->Release();

	immediateContext->OMSetRenderTargets(1, &g_pBackBufferRTView, g_pZBuffer);

	D3D11_VIEWPORT viewport;

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (FLOAT)width;
	viewport.Height = (FLOAT)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	immediateContext->RSSetViewports(1, &viewport);


	return S_OK;

}

void ShutdownD3D()
{
	if (texture) texture->Release();
	if (g_pSampler0) g_pSampler0->Release();
	if (camera) camera->~Camera();
	if (g_pPixelShader) g_pPixelShader->Release();
	if (g_pVertexBuffer) g_pVertexBuffer->Release();
	if (g_pConstantBuffer0) g_pConstantBuffer0->Release();
	if (g_pSwapChain) g_pSwapChain->Release();
	if (immediateContext) immediateContext->Release();
	if (device) device->Release();
	if (g_pBackBufferRTView) g_pBackBufferRTView->Release();
	if (g_keyboard_device) {
		g_keyboard_device->Unacquire();
		g_keyboard_device->Release();
	}

	if (mouse_input)
	{
		mouse_input->Unacquire();
		mouse_input->Release();
	}
	if (g_direct_input) g_direct_input->Release();
}

void UpdateAI()
{

}

HRESULT UpdateInput()
{
	HRESULT hr;
	//for (GameObject var : gameObjects)
	//{
	//	//if (typeid(var) != typeid(Player)) continue;
	//	//std::cout << var.get_name();
	//}
	//std::cout << gameObjects[0].get_name();

	hr = g_keyboard_device->GetDeviceState(sizeof(g_keyboard_keys_state), (LPVOID)&g_keyboard_keys_state);

	if (FAILED(hr)) {
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			g_keyboard_device->Acquire();
		}
	}

	hr = mouse_input->GetDeviceState(sizeof(mouse_state), (LPVOID)&mouse_state);

	if (FAILED(hr)) {
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			mouse_input->Acquire();
		}
	}

	MouseMoved();

	if (IsKeyPressed(DIK_ESCAPE)) DestroyWindow(g_hWnd);

	if (IsKeyPressed(DIK_A)) camera->transform.right(-timer->deltaTime() * move_speed);
	if (IsKeyPressed(DIK_D)) camera->transform.right(timer->deltaTime() * move_speed);
	if (IsKeyPressed(DIK_W)) camera->transform.horizontal_forward(timer->deltaTime()*move_speed);
	if (IsKeyPressed(DIK_S)) camera->transform.horizontal_forward(-timer->deltaTime()*move_speed);


	if (IsKeyPressed(DIK_LEFT)) camera->transform.rotate_fixed(0, -timer->deltaTime() * look_speed, 0);
	if (IsKeyPressed(DIK_RIGHT)) camera->transform.rotate_fixed(0, timer->deltaTime() * look_speed, 0);
	if (IsKeyPressed(DIK_UP)) camera->transform.rotate_fixed(-timer->deltaTime() * look_speed, 0, 0);
	if (IsKeyPressed(DIK_DOWN)) camera->transform.rotate_fixed(timer->deltaTime() * look_speed, 0, 0);

	return S_OK;
}

void UpdateSound()
{

}

void UpdateGraphics()
{

}


//void MoveCamera() {
//
//	POINT pos;
//	GetCursorPos(&pos);
//
//	//camera->transform.rotate(/*(abs(pos.y) - abs(mouse_y_center)) * timer->deltaTime() * look_speed*/ 0, (abs(pos.x) - abs(mouse_x_center)) * timer->deltaTime() * look_speed, 0);
//	camera->transform.rotate((abs(pos.y) - abs(mouse_y_center)) * timer->deltaTime() * look_speed, (abs(pos.x) - abs(mouse_x_center)) * timer->deltaTime() * look_speed, 0);
//	SetCursorPos(mouse_x_center, mouse_y_center);
//
//}


bool IsKeyPressed(unsigned char DI_keycode)
{
	return g_keyboard_keys_state[DI_keycode] & 0x80;
}

void MouseMoved()
{


	mouse_x += mouse_state.lX;

	mouse_y += mouse_state.lY;


	// tricky solution -> TODO: find an other way to do it -> directly use the mouse_state.lX and the mouse_state.lY to rotate
	//camera->transform.rotate((abs(mouse_y) - abs(mouse_y_center)) * timer->deltaTime() * move_speed, (abs(mouse_x) - abs(mouse_x_center)) * timer->deltaTime() * move_speed, 0);
	camera->transform.rotate_fixed(mouse_state.lY * timer->deltaTime() * move_speed, mouse_state.lX * timer->deltaTime() * move_speed, 0);

	mouse_x = mouse_x_center;
	mouse_y = mouse_y_center;
	SetCursorPos(mouse_x, mouse_x);
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