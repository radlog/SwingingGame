
#include "main.h"
#include <sstream>
#include "LavaFloor.h"
#include <dwrite.h>
#include <d2d1.h>
#include "Floor.h"
#include "Player.h"
//using namespace std;
D3Dfw *dx_handle = D3Dfw::get_instance();

double time_since_last_frame = 0;
double second = 1;
static int fps = 0;
static int frames = 0;
// game objects
GameObject test;
GameObject lava;
LavaFloor lava_floor;
Floor test_floor;
Character player;
Camera *camera;
const int upper_platform_count = 100; // 3000 * 3312 vertices seems to slow down the process when rotating -> consider optimizations for rotations
const int middle_platform_count = 100;
const int lower_platform_count = 100;
GameObject upper_platforms[upper_platform_count];
GameObject middle_platforms[middle_platform_count];
GameObject lower_platforms[lower_platform_count];

// text render factory and format
IDWriteFactory *dwrite_factory;
IDWriteTextFormat *text_format;

Model *model_test;
Model *platform;
Input input;

bool enable_glowing = false;


XMVECTOR directional_light_shines_from = XMVectorSet(0.0f, 0.5f, -1.0f, 0.0f);
XMVECTOR directional_light_colour = XMVectorSet(1.0f, 1.0f, 0.0f, 1.0f); // green
XMVECTOR ambient_light_colour = XMVectorSet(0.1f, 0.1f, 0.1f, 1.0f); // dark grey



VGTime* timer;

// the window title
char g_title[100] = "Swing to Win(g)";


// methods
HRESULT init_dx(HINSTANCE instance, HINSTANCE prev_instance, LPSTR lp_cmd_line, int n_cmd_show);
HRESULT initialise_window(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK wnd_proc(HWND, UINT, WPARAM, LPARAM);

void load_content();
void load_lava();

void render_frame(void);
void draw_map(XMMATRIX view_projection);
void update_ai();
void update_sound();
void update_graphics();
void end_game();

void update_lava(XMMATRIX view_projection, float time);

Skybox skybox;

POS_TEX_NORM_COL_VERTEX *plane_vertices;
unsigned int *plane_indices;

void debug_util(int str);

int WINAPI WinMain(const HINSTANCE instance, const HINSTANCE prev_instance, const LPSTR lp_cmd_line, const int n_cmd_show)
{

	init_dx(instance, prev_instance, lp_cmd_line, n_cmd_show);
	load_content();

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
			dx_handle->input->update_input(camera, timer);
			//UpdateSound();
			//UpdateGraphics();
			render_frame();
		}
	}

	end_game();
	return static_cast<int>(msg.wParam);
}



void render_frame(void)
{
	const XMMATRIX view_projection = camera->calculate_view_projection();

	// clear the render target view
	dx_handle->clear_rtv();

	// draw here
	//test.Draw(view_projection);
	skybox.draw(XMMatrixTranslationFromVector(camera->transform.local_position) * view_projection);
	//upperPlatforms[0].update(*timer);
	//upperPlatforms[0].Draw(view_projection);
	//upperPlatforms[1].update(*timer);
	//upperPlatforms[1].Draw(view_projection);

	
	draw_map(view_projection);
	//if(!upperPlatforms[0].collided(upperPlatforms[1]))
	//{
	//	upperPlatforms[0].transform.right(timer->deltaTime() * 10);
	//}
	//UpdateLava(view_projection, timer->totalTime());


	test_floor.draw(view_projection);
	//DebugUTIL(timer->deltaTime());
	debug_util(timer->getFPS());
	// swap back buffer with front buffer
	dx_handle->swap_chain->Present(0, 0);

}


LRESULT CALLBACK wnd_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
	return 0;
}


void draw_map(const XMMATRIX view_projection)
{
	for (size_t i = 0; i < upper_platform_count; i++)
{
	upper_platforms[i].draw(view_projection/*,D3D11_PRIMITIVE_TOPOLOGY_POINTLIST*/);
	middle_platforms[i].draw(view_projection/*,D3D11_PRIMITIVE_TOPOLOGY_POINTLIST*/);
	lower_platforms[i].draw(view_projection/*,D3D11_PRIMITIVE_TOPOLOGY_POINTLIST*/);
}
}

void end_game()
{
	timer->stop();
}

void update_ai()
{

}



void update_sound()
{

}

void update_graphics()
{

}

void update_lava(const XMMATRIX view_projection, const float time)
{
	lava_floor.get_model()->UpdateConstantBuffer_TIME_SCALED(lava_floor.transform.world * view_projection,view_projection, directional_light_shines_from, directional_light_colour, ambient_light_colour, time);
	lava_floor.draw(view_projection);// , false, D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void debug_util(const int str)
{
	std::ostringstream ss;
	ss << str;
	string s = ss.str() + "\n";
	OutputDebugString(s.c_str());
}

void load_lava()
{
	lava_floor = LavaFloor("assets/lava_selfmade_diffuse.png");
}


void load_content()
{

	player = Player("player1");

	const XMVECTOR platform_scale = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
	const XMVECTOR rotation = XMQuaternionIdentity();

	camera = new Camera();
	timer = new VGTime();
	skybox = Skybox("assets/purple_nebular.dds");
	test_floor = Floor("assets/crate.jpg");

	//char filename[] = "assets/FloatingIsland_001.obj";

	model_test = new Model(dx_handle->device, dx_handle->immediate_context,(char*) "assets/Sphere.obj");
	model_test->LoadTexture("assets/FloatingIsland_DIFFUSE.png");
	POS_TEX_NORM_COL_VERTEX* platform_placeholder =  Geometry::pos_tex_norm_col_cube(1.0f);
	platform = new Model(dx_handle->device, dx_handle->immediate_context);
	platform->LoadGeoModel(platform_placeholder, 36, sizeof(POS_TEX_NORM_COL_VERTEX));
	platform->LoadTexture("assets/FloatingIsland_DIFFUSE.png");

	float plat_collision_radius = platform->getCollisionSphere().collisionRadius;
	plat_collision_radius = 2.0f;

	for (size_t i = 0; i < upper_platform_count; i++)
	{
		upper_platforms[i] = GameObject(&"upperPlatform"[i], Transform(platform_scale, rotation, XMVectorSet(i * plat_collision_radius * 3,0, 1.0f, 0.0f)), *platform);
	}

	for (size_t i = 0; i < middle_platform_count; i++)
	{
		middle_platforms[i] = GameObject(&"upperPlatform"[i], Transform(platform_scale, rotation, XMVectorSet(i * plat_collision_radius * 3, 0, 1.0f, 0.0f)), *platform);
	}

	for (size_t i = 0; i < lower_platform_count; i++)
	{
		lower_platforms[i] = GameObject(&"upperPlatform"[i], Transform(platform_scale, rotation, XMVectorSet(i * plat_collision_radius * 3, 0, 1.0f, 0.0f)), *platform);
	}

	//LoadLava();

	timer->start();
}

HRESULT init_dx(const HINSTANCE instance, HINSTANCE prev_instance, const LPSTR lp_cmd_line, const int n_cmd_show)
{
	auto hr = S_OK;
	UNREFERENCED_PARAMETER(prev_instance);
	UNREFERENCED_PARAMETER(lp_cmd_line);

	if (FAILED(hr = dx_handle->initialise_window(instance, n_cmd_show, wnd_proc)))
	{
		DXTRACE_MSG("Failed to create Window");
		return hr;
	}

	if (FAILED(hr = dx_handle->initialise_dx()))
	{
		DXTRACE_MSG("Failed to initialise DirectX");
		return hr;
	}

	if (FAILED(hr = dx_handle->initialise_input()))
	{
		DXTRACE_MSG("Failed to initialise Input");
		return hr;
	}

	ShowCursor(false);

	return hr;
}

HRESULT initialise_window(HINSTANCE hInstance, int nCmdShow)
{
	return 0;
}
