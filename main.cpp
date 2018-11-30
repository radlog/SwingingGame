#include <Windows.h>
#include <dxgi.h>
#include <dxerr.h>
#include <xnamath.h>

#include "Camera.h"
#include "GameObject.h"
#include "Skybox.h"
#include "main.h"


//using namespace std;
d3dfw* dx_handle = d3dfw::getInstance();


// game objects
GameObject test;
Camera* camera;
const int upperPlatformCount = 100; // 3000 * 3312 plane_vertices seems to slow down the process when rotating -> consider optimizations for rotations
const int middlePlatformCount = 100;
const int lowerPlatformCount = 100;
GameObject upperPlatforms[upperPlatformCount];
GameObject middlePlatforms[middlePlatformCount];
GameObject lowerPlatforms[lowerPlatformCount];

POS_TEX_VERTEX* skybox_desc;
Model sky_model;

Model *model_test;
Input input;

bool enable_glowing = false;


XMVECTOR directional_light_shines_from = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
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
void RenderFrame(void);
void DrawMap();
void UpdateAI();
void UpdateSound();
void UpdateGraphics();
void EndGame();
//void AlterVertices(POS_COL_VERTEX* vert, WPARAM message);






float s = 1.0f;
const int n = 5;


Skybox skybox;

POS_TEX_NORM_COL_VERTEX *plane_vertices;
unsigned int *plane_indices;



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
	test.Draw(view_projection);
	upperPlatforms[10].Draw(view_projection);
	skybox.Draw(view_projection);
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

void LoadContent()
{
	XMVECTOR scale = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
	XMVECTOR rotation = XMQuaternionIdentity();
	camera = new Camera();
	timer = new VGTime();
	skybox = Skybox("assets/crate.jpg");

	char filename[] = "assets/Sphere.obj";
	model_test = new Model(dx_handle->device, dx_handle->immediateContext, filename);

	//Geometry::create_textured_skybox();
	//Model sky(dx_handle->device, dx_handle->immediateContext);
	//sky.LoadGeoModel(, ARRAYSIZE(textured_normal_cube), sizeof(textured_normal_cube[0]));
	//est = GameObject("test", Transform(scale, rotation, XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)), sky);

	//plane_indices = new unsigned int[5 * 5 * 6];
	//Geometry::create_indexed_tiled_textured_normal_plane(&plane_vertices, plane_indices, 5, 1);

	Model plane = Model(dx_handle->device, dx_handle->immediateContext);
	Geometry::create_indexed_tiled_textured_normal_plane(&plane_vertices, &plane_indices, 5, 1.0f);
	plane.LoadGeoModel(plane_vertices, (5 + 1)*(5 + 1), sizeof(POS_TEX_NORM_COL_VERTEX), plane_indices, 5 * 5 * 6);
	test = GameObject("test", Transform(scale, rotation, XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f)), plane);

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