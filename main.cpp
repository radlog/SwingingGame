
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



Enemy enemy;

double time_since_last_frame = 0;
double second = 1;
static int fps = 0;
static int frames = 0;
// game objects
Player *player;
GameObject* floor_object;
GameObject *lava;
LavaFloor *lava_floor;
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
			render_frame(player->get_fps_camera());
			//render_frame(player.get_top_down_camera()); // second camera (top down view) for map overview
		}
	}

	end_game();
	return static_cast<int>(msg.wParam);
}



void render_frame(Camera *camera)
{
	const auto view_projection = camera->calculate_view_projection(); // get camera view projection to project the 3d space to 2d 
	const auto sky_lock = XMMatrixTranslationFromVector(camera->transform.get_local_position()) * view_projection; // move skybox transform with the camera
	
	dx_handle->clear_rtv(); // clear the render target view

	// draw start
	//skybox.draw(sky_lock); // draw skybox
	scene_root.update_constant_buffer_time_scaled(view_projection, view_projection, directional_light_shines_from, directional_light_colour, ambient_light_colour, timer->total_time()); // update constant buffer values for every gameobject in the scene
	scene_root.draw(view_projection); // draw scene
	// draw end

	dx_handle->get_swap_chain()->Present(0, 0);	// swap back buffer with front buffer
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

	player->update(timer); // update player separately because it has an input update in it methods
	scene_root.update(timer); // update scene with all its children
}

void end_game()
{
	timer->stop();
}

void update_ai()
{

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
	
}


// loads all objects in the scene and the scene root
void load_content()
{
	
	const auto scale = XMVectorSplatOne(); // scale 1,1,1
	const auto rotation = XMQuaternionIdentity(); // identity rotation


	timer = new VGTime(); // the game timer for steady movement
	skybox = Skybox("assets/purple_nebular.dds"); // skybox that loads a cubemap

	//cube = new Model("assets/cube.obj", CB_STATE_TIME_SCALED, MESH); // simple cube model used for enemies

	//island_model = new Model("assets/FloatingIsland_001.obj", CB_STATE_TIME_SCALED); // platform model i made by myself
	island_model = new Model("assets/cube.obj", CB_STATE_TIME_SCALED); // platform model i made by myself
	island_model->load_texture("assets/lava_selfmade_DIFFUSE.png"); // texture of the platform model

	// this floor was only there for testing collision
	//floor_model = new Floor(); // model for the ground
	//floor_object = new GameObject("floor", floor_model, Transform()); // ground gameobject
	//floor_object->transform.translate(floor_model->get_transform()->get_local_position());


	player = new Player("player1"); // the player gameobject
	player->transform = Transform(XMVectorSplatOne(), XMQuaternionIdentity(), XMVectorSet(0, 5, -10, 0)); // set initial player position
	player->update(timer); // call one update to set player to the correct position

	//enemy = Enemy("enemy", cube, Transform(scale, rotation, XMVectorSet(0, 10, 0.0f, 0.0f))); // enemy gameobject

	lava_floor = new LavaFloor("assets/lava_selfmade_diffuse.png"); // lava gameobject
	lava = new GameObject("lava", lava_floor, Transform(),GROUND); // ground gameobject
	lava->transform.translate(lava_floor->get_transform()->get_local_position()); // translate the lave to be centered along the x and y axis

	scene_root = GameObject("scene_root");
	scene_root.set_kinetic(false);
	scene_root.add_child(player);
	//scene_root.add_child(floor_object);
	//scene_root.add_child(&enemy);
	scene_root.add_child(lava);

	// initialise lower platforms
	for (size_t i = 0; i < sqrt(lower_platform_count); i++)
	{
		for (size_t j = 0; j < sqrt(lower_platform_count); j++)
		{
			const auto pl = new GameObject("lower_Platform", island_model, Transform(XMVectorSplatOne(), rotation, XMVectorSet(i * 10, 0, j * 10,0)));
			//lower_platforms.push_back(*pl);
			scene_root.add_child(pl);
		}
	}

	for (size_t i = 0; i < sqrt(middle_platform_count); i++)
	{
		for (size_t j = 0; j < sqrt(middle_platform_count); j++)
		{
			const auto pl = new GameObject("middle_platform", island_model, Transform(XMVectorSplatOne(), rotation, XMVectorSet(i * 10, 20, j * 10, 0)));
			//lower_platforms.push_back(*pl);
			scene_root.add_child(pl);
		}
	}

	for (size_t i = 0; i < sqrt(upper_platform_count); i++)
	{
		for (size_t j = 0; j < sqrt(upper_platform_count); j++)
		{
			const auto pl = new GameObject("upper_platform", island_model, Transform(XMVectorSplatOne(), rotation, XMVectorSet(i * 10, 40, j * 10, 0)));
			//lower_platforms.push_back(*pl);
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
