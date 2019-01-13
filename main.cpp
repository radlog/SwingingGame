
#include "main.h"
#include <sstream>
#include "LavaFloor.h"
#include <dwrite.h>
#include <d2d1.h>
#include "Floor.h"
#include "Player.h"
#include "GeoCube.h"
#include "Model.h"
#include "Enemy.h"
#include "ObjCube.h"

//using namespace std;
D3Dfw *dx_handle = D3Dfw::get_instance();

GameObject scene_root;

vector<GameObject> gameobjects;

GameObject* m1;
GameObject* m2;
GameObject* m3;
GameObject* m4;
GameObject* m5;
GameObject* m6;
GameObject* floor_object;
GameObject* cube_one;
GameObject* cube_two;
GameObject* cube_three;
GameObject* cube_four;
GameObject* cube_five;
Enemy enemy;

double time_since_last_frame = 0;
double second = 1;
static int fps = 0;
static int frames = 0;
// game objects
Player player;
GameObject test;
GameObject lava;
LavaFloor lava_floor;
Floor *floor_model;
Camera *camera;
const int upper_platform_count = 100; // 3000 * 3312 vertices seems to slow down the process when rotating -> consider optimizations for rotations
const int middle_platform_count = 100;
const int lower_platform_count = 100;
vector<GameObject> upper_platforms;
vector<GameObject> middle_platforms;
vector<GameObject> lower_platforms;

// text render factory and format
IDWriteFactory *dwrite_factory;
IDWriteTextFormat *text_format;


Model* cube;
Model *island_model;
Input input;

bool enable_glowing = false;


XMVECTOR directional_light_shines_from = XMVectorSet(0.0f, 0.5f, 1.0f, 0.0f);
XMVECTOR directional_light_colour = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f); // green
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

void render_frame(Camera *camera);
void draw_map(XMMATRIX view_projection);
void update(VGTime *timer);
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

	MSG msg = { nullptr };

	while (msg.message != WM_QUIT)
	{
		timer->tick();
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			update(timer);
			render_frame(player.get_fps_camera());
			//render_frame(player.get_top_down_camera());
		}
	}

	end_game();
	return static_cast<int>(msg.wParam);
}



void render_frame(Camera *camera)
{
	const auto view_projection = camera->calculate_view_projection();
	const auto sky_lock = XMMatrixTranslationFromVector(camera->transform.get_local_position()) * view_projection;
	// clear the render target view
	dx_handle->clear_rtv();

	// draw here
	skybox.draw(sky_lock);

	// update constant buffer values for every gameobject in the scene
	scene_root.update_constant_buffer_time_scaled(view_projection, view_projection, directional_light_shines_from, directional_light_colour, ambient_light_colour, timer->delta_time());
	scene_root.draw(view_projection);
	player.draw(view_projection);
	cube_one->draw(view_projection);
	//cube_two->draw(view_projection);
	//lower_platforms[0].draw(view_projection);
	//test_floor.draw(view_projection);
	//enemy.draw(view_projection);

	//DebugUTIL(timer->deltaTime());
	//debug_util(timer->get_fps());
	// swap back buffer with front buffer
	dx_handle->get_swap_chain()->Present(0, 0);
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



void update(VGTime* timer)
{
	//dx_handle->input->update_input(cube_one, timer);
	//update_lava(view_projection, timer->total_time());
	//lower_platforms[0].translate(Transform::world_left, 10 * timer->delta_time());
	cube_one->translate(Transform::world_left, 10 * timer->delta_time());
	player.update(timer);
	scene_root.update(timer);

	//UpdateSound();
	//UpdateGraphics();
	//enemy.chase_target(&player, timer);
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
	lava_floor.get_model()->update_constant_buffer_time_scaled(lava_floor.transform.get_world() * view_projection, view_projection, directional_light_shines_from, directional_light_colour, ambient_light_colour, time);
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
	const auto scale = XMVectorSplatOne();
	const auto rotation = XMQuaternionIdentity();

	camera = new Camera();
	timer = new VGTime();
	skybox = Skybox("assets/purple_nebular.dds");


	UINT numverts;

	//auto *cube = new ObjCube();
	//auto *pl_cube = new GeoCube();
	//cube->set_shader_file("lighted_shader.hlsl");

	cube = new Model("assets/cube.obj", CB_STATE_TIME_SCALED, MESH);

	island_model = new Model("assets/FloatingIsland_001.obj", CB_STATE_TIME_SCALED, NOTHING);
	//island_model->set_shader_file("lighted_shader.hlsl");
	island_model->load_texture("assets/lava_selfmade_DIFFUSE.png");

	floor_model = new Floor();
	floor_object = new GameObject("floor", floor_model, Transform());


	cube_one = new GameObject("cube_one", cube, Transform(scale, rotation, XMVectorSet(-10, 0, 0.0f, 0.0f)));
	cube_two = new GameObject("cube_two", cube, Transform(scale, rotation, XMVectorSet(0, 0, 0.0f, 0.0f)));
	cube_three = new GameObject("cube_three", cube, Transform(scale, rotation, XMVectorSet(0, 0, 0.0f, 0.0f)));
	cube_four = new GameObject("cube_four", cube, Transform(scale, rotation, XMVectorSet(5, 0, 0.0f, 0.0f)));
	cube_five = new GameObject("cube_five", cube, Transform(scale, rotation, XMVectorSet(10, 0, 0.0f, 0.0f)));


	cube_one->set_kinetic(true);
	cube_two->set_kinetic(true);
	cube_three->set_kinetic(true);
	cube_four->set_kinetic(true);
	cube_five->set_kinetic(true);

	cube_one->add_child(cube_two);
	//cube_two.set_kinetic(true);

	//cube_one.add_child(&cube_two);
	//cube_one.add_child(&cube_three);
	//cube_three.add_child(&cube_four);

	player = Player("player1");
	player.transform = Transform(XMVectorSplatOne(), XMQuaternionIdentity(), XMVectorSet(0, 5, -10, 0));
	player.update(timer);


	//cube_one->add_child(cube_two);
	enemy = Enemy("enemy", cube, Transform(scale, rotation, XMVectorSet(0, 10, 0.0f, 0.0f)));


	scene_root = GameObject("scene_root");
	scene_root.set_kinetic(true);
	scene_root.add_child(&player);
	scene_root.add_child(floor_object);
	scene_root.add_child(&enemy);


	//scene_root.add_child(cube_one);
	//scene_root.add_child(cube_two);
	//scene_root.add_child(cube_three);
	//scene_root.add_child(cube_four);


	//scene_root.add_child(cube_one);
	//scene_root.add_child(cube_two);
	//scene_root.add_child(cube_four);
	//scene_root.add_child(&test_floor);

	//auto plat_collision_radius = platform->get_collision_sphere().collision_radius;
	//plat_collision_radius = 2.0f;


	// initialise upper platforms
	//for (size_t i = 0; i < upper_platform_count; i++)
	//{
	//	upper_platforms.push_back(GameObject("upperPlatform", platform, Transform(XMVectorSplatOne(), rotation, XMVectorSet(i * 3, 0, 1.0f, 0.0f))));
	//}
	//// initialise middle platforms
	//for (size_t i = 0; i < middle_platform_count; i++)
	//{
	//	middle_platforms.push_back(GameObject("middlePlatform", platform, Transform(XMVectorSplatOne(), rotation, XMVectorSet(i * 3, 0, 1.0f, 0.0f))));
	//}
	// initialise lower platforms
	for (size_t i = 0; i < sqrt(lower_platform_count); i++)
	{
		for (size_t j = 0; j < sqrt(lower_platform_count); j++)
		{
			const auto pl = new GameObject("lowerPlatform", island_model, Transform(XMVectorSplatOne(), rotation, XMVectorSet(i * 10, 0, j * 10,0)));
			lower_platforms.push_back(*pl);
			scene_root.add_child(pl);
		}
	}

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

LRESULT CALLBACK wnd_proc(const HWND hwnd, const UINT message, const WPARAM w_param, const LPARAM l_param)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, w_param, l_param);
	}
	return 0;
}


/*
HRESULT initialise_window(HINSTANCE hInstance, int nCmdShow)
{
	return 0;
}
*/
