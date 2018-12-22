
#include "main.h"
#include <sstream>
#include "LavaFloor.h"
#include <dwrite.h>
#include <d2d1.h>
//using namespace std;
d3dfw* dx_handle = d3dfw::getInstance();

double time_since_last_frame = 0;
double second = 1;
static int fps = 0;
static int frames = 0;
// game objects
GameObject test;
GameObject lava;
LavaFloor lavaFloor;
Camera* camera;
const int upperPlatformCount = 100; // 3000 * 3312 vertices seems to slow down the process when rotating -> consider optimizations for rotations
const int middlePlatformCount = 100;
const int lowerPlatformCount = 100;
GameObject upperPlatforms[upperPlatformCount];
GameObject middlePlatforms[middlePlatformCount];
GameObject lowerPlatforms[lowerPlatformCount];

// text render factory and format
IDWriteFactory* dwrite_factory;
IDWriteTextFormat* text_format;

Model *model_test;
Model *platform;
Input input;

bool enable_glowing = false;


XMVECTOR directional_light_shines_from = XMVectorSet(0.0f, 0.5f, -1.0f, 0.0f);
XMVECTOR directional_light_colour = XMVectorSet(1.0f, 1.0f, 0.0f, 1.0f); // green
XMVECTOR ambient_light_colour = XMVectorSet(0.1f, 0.1f, 0.1f, 1.0f); // dark grey



VGTime* timer;

//g_Title is a replace for g_TutorialName
char g_Title[100] = "Swing to Win(g)";


// methods
HRESULT InitD3D(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
HRESULT InitialiseWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void LoadContent();
void LoadLava();

void RenderFrame(void);
void DrawMap(XMMATRIX view_projection);
void UpdateAI();
void UpdateSound();
void UpdateGraphics();
void EndGame();

void UpdateLava(XMMATRIX view_projection, float time);

Skybox skybox;

POS_TEX_NORM_COL_VERTEX *plane_vertices;
unsigned int *plane_indices;

void DebugUTIL(int str);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	InitD3D(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	LoadContent();

	MSG msg = { 0 };

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
			dx_handle->input->UpdateInput(camera, timer);
			//UpdateSound();
			//UpdateGraphics();
			RenderFrame();
		}
	}

	EndGame();
	return (int)msg.wParam;
}



void RenderFrame(void)
{
	const XMMATRIX view_projection = camera->calculate_view_projection();

	// clear the render target view
	dx_handle->ClearRTV();

	// draw here
	//test.Draw(view_projection);
	skybox.Draw(XMMatrixTranslationFromVector(camera->transform.local_position) * view_projection);
	//upperPlatforms[0].update(*timer);
	//upperPlatforms[0].Draw(view_projection);
	//upperPlatforms[1].update(*timer);
	//upperPlatforms[1].Draw(view_projection);

	
	DrawMap(view_projection);
	//if(!upperPlatforms[0].collided(upperPlatforms[1]))
	//{
	//	upperPlatforms[0].transform.right(timer->deltaTime() * 10);
	//}
	UpdateLava(view_projection, timer->totalTime());



	//DebugUTIL(timer->deltaTime());
	DebugUTIL(timer->getFPS());
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
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}


void DrawMap(XMMATRIX view_projection)
{
	for (size_t i = 0; i < upperPlatformCount; i++)
{
	upperPlatforms[i].Draw(view_projection/*,D3D11_PRIMITIVE_TOPOLOGY_POINTLIST*/);
	middlePlatforms[i].Draw(view_projection/*,D3D11_PRIMITIVE_TOPOLOGY_POINTLIST*/);
	lowerPlatforms[i].Draw(view_projection/*,D3D11_PRIMITIVE_TOPOLOGY_POINTLIST*/);
}
}

void EndGame()
{
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

void UpdateLava(XMMATRIX view_projection,float time)
{
	lavaFloor.get_model()->UpdateConstantBuffer_TIME_SCALED(lavaFloor.transform.world * view_projection,view_projection, directional_light_shines_from, directional_light_colour, ambient_light_colour, time);
	lavaFloor.Draw(view_projection);// , false, D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void DebugUTIL(int str)
{
	std::ostringstream ss;
	ss << str;
	string s = ss.str() + "\n";
	OutputDebugString(s.c_str());
}

void LoadLava()
{
	lavaFloor = LavaFloor("assets/lava_selfmade_diffuse.png");
}


void LoadContent()
{

	

	XMVECTOR platform_scale = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
	XMVECTOR rotation = XMQuaternionIdentity();

	camera = new Camera();
	timer = new VGTime();
	skybox = Skybox("assets/purple_nebular.dds");

	//char filename[] = "assets/FloatingIsland_001.obj";

	model_test = new Model(dx_handle->device, dx_handle->immediateContext,(char*) "assets/Sphere.obj");
	model_test->LoadTexture("assets/FloatingIsland_DIFFUSE.png");
	POS_TEX_NORM_COL_VERTEX* platform_placeholder =  Geometry::pos_tex_norm_col_cube(1.0f);
	platform = new Model(dx_handle->device, dx_handle->immediateContext);
	platform->LoadGeoModel(platform_placeholder, 36, sizeof(POS_TEX_NORM_COL_VERTEX));
	platform->LoadTexture("assets/FloatingIsland_DIFFUSE.png");

	float plat_collision_radius = platform->getCollisionSphere().collisionRadius;
	plat_collision_radius = 2.0f;

	for (size_t i = 0; i < upperPlatformCount; i++)
	{
		upperPlatforms[i] = GameObject("upperPlatform" + i, Transform(platform_scale, rotation, XMVectorSet(i * plat_collision_radius * 3,0, 1.0f, 0.0f)), *platform);
	}

	for (size_t i = 0; i < middlePlatformCount; i++)
	{
		middlePlatforms[i] = GameObject("upperPlatform" + i, Transform(platform_scale, rotation, XMVectorSet(i * plat_collision_radius * 3, 0, 1.0f, 0.0f)), *platform);
	}

	for (size_t i = 0; i < lowerPlatformCount; i++)
	{
		lowerPlatforms[i] = GameObject("upperPlatform" + i, Transform(platform_scale, rotation, XMVectorSet(i * plat_collision_radius * 3, 0, 1.0f, 0.0f)), *platform);
	}

	LoadLava();

	timer->start();
}

HRESULT InitD3D(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HRESULT hr = S_OK;
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (FAILED(dx_handle->InitialiseWindow(hInstance, nCmdShow, WndProc)))
	{
		DXTRACE_MSG("Failed to create Window");
		return 0;
	}

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

	ShowCursor(false);

	return hr;
}
