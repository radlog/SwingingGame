
#include "main.h"
#include <sstream>
#include "LavaFloor.h"
//using namespace std;
d3dfw* dx_handle = d3dfw::getInstance();

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


Model *model_test;
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
void DrawMap();
void UpdateAI();
void UpdateSound();
void UpdateGraphics();
void EndGame();

void UpdateLava(XMMATRIX view_projection, float time);

Skybox skybox;

POS_TEX_NORM_COL_VERTEX *plane_vertices;
unsigned int *plane_indices;

void DebugUTIL();

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
	upperPlatforms[10].Draw(view_projection);
	UpdateLava(view_projection, timer->totalTime());

	DrawMap();


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


void DrawMap()
{
	//for (size_t i = 0; i < upperPlatformCount; i++)
//{
//	upperPlatforms[i].Draw(view_projection/*,D3D11_PRIMITIVE_TOPOLOGY_POINTLIST*/);
//}
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

	lavaFloor.get_model()->UpdateConstantBuffer_TIME_SCALED(lavaFloor.transform.world * view_projection, directional_light_shines_from, directional_light_colour, ambient_light_colour, time);
	lavaFloor.Draw(view_projection);
}

void DebugUTIL()
{
	//std::ostringstream ss;
	//ss << time;
	//string s = ss.str() + "\n";
	//OutputDebugString(s.c_str());
}

void LoadLava()
{
	lavaFloor = LavaFloor("assets/lava_selfmade_diffuse.png", "assets/lava_selfmade_diffuse.png");
}


void LoadContent()
{
	XMVECTOR sphere_scale = XMVectorSet(10.0f, 1.0f, 10.0f, 0.0f);
	XMVECTOR rotation = XMQuaternionIdentity();

	camera = new Camera();
	timer = new VGTime();
	skybox = Skybox("assets/purple_nebular.dds");

	char filename[] = "assets/Sphere.obj";
	model_test = new Model(dx_handle->device, dx_handle->immediateContext, filename);

	for (size_t i = 0; i < upperPlatformCount; i++)
	{
		upperPlatforms[i] = GameObject("upperPlatform" + i, Transform(sphere_scale, rotation, XMVectorSet(i, i, 1.0f, 0.0f)), *model_test);
	}

	for (size_t i = 0; i < middlePlatformCount; i++)
	{

	}

	for (size_t i = 0; i < lowerPlatformCount; i++)
	{

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
