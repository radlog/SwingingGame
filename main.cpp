
#include "main.h"
#include <sstream>
#include "LavaFloor.h"
#include <dwrite.h>
#include <d2d1.h>
#include "Floor.h"
#include "Player.h"
#include "Cube.h"

//using namespace std;
D3Dfw *dx_handle = D3Dfw::get_instance();

vector<GameObject> gameobjects;

GameObject cube_one;
GameObject cube_two;

double time_since_last_frame = 0;
double second = 1;
static int fps = 0;
static int frames = 0;
// game objects
Player player;
GameObject test;
GameObject lava;
LavaFloor lava_floor;
Floor test_floor;
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
void update(VGTime timer);
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
			//UpdateAI();
			//dx_handle->input->update_input(camera, timer);
			player.update(*timer);
			update(*timer);
			//upper_platforms[0].update(*timer);
			//dx_handle->input->update_input(&upper_platforms[0], timer);
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
	const XMMATRIX view_projection = player.get_camera()->calculate_view_projection();
	const XMMATRIX sky_lock = XMMatrixTranslationFromVector(player.get_camera()->transform.get_local_position()) * view_projection;
	// clear the render target view
	dx_handle->clear_rtv();

	// draw here
	skybox.draw(sky_lock);
	//upperPlatforms[0].update(*timer);
	//upperPlatforms[0].Draw(view_projection);
	//upperPlatforms[1].update(*timer);
	//upperPlatforms[1].Draw(view_projection);


	//draw_map(view_projection);
	//for(auto& i: gameobjects)
	//{
	//	i.draw(view_projection);
	//}
	if (!cube_one.collided(cube_two))
	{
		cube_one.transform.right(timer->delta_time() * 10);
	}

	cube_one.draw(view_projection);
	cube_two.draw(view_projection);
	//update_lava(view_projection, timer->total_time());
	player.draw(view_projection);

	test_floor.draw(view_projection);



	//DebugUTIL(timer->deltaTime());
	debug_util(timer->get_fps());
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



void update(VGTime timer)
{
	// test
	/*if (player.collided(test_floor))
	{
		player.set_grounded(true);
	}*/
	for (auto& i : gameobjects)
	{
		i.update(timer);
		/*for(auto& j : gameobjects)
		{
			if(i.collided(j))
			{

			}
		}*/
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
	test_floor = Floor("assets/crate.jpg");

	UINT numverts;
	POS_TEX_NORM_COL_VERTEX* platform_placeholder = Geometry::cube_ptnc(&numverts);
	platform = new Model(CB_STATE_SIMPLE);
	platform->load_geo_model(platform_placeholder, numverts, sizeof(POS_TEX_NORM_COL_VERTEX));
	platform->load_texture("assets/FloatingIsland_DIFFUSE.png");
	auto *cube = new Cube();


	player = Player("player1", cube, Transform(XMVectorSet(1, 1, 1, 0), XMQuaternionIdentity(), XMVectorSet(0, 10, -40, 0)));

	cube_one = GameObject("cube_one", cube, Transform(scale, rotation, XMVectorSet(0, 0, 1.0f, 0.0f)));
	cube_two = GameObject("cube_two", cube, Transform(scale, rotation, XMVectorSet(10, 0, 1.0f, 0.0f)));












	//auto plat_collision_radius = platform->get_collision_sphere().collision_radius;
	//plat_collision_radius = 2.0f;


	//// initialise upper platforms
	//for (size_t i = 0; i < upper_platform_count; i++)
	//{
	//	upper_platforms.push_back(GameObject("upperPlatform", platform, Transform(platform_scale, rotation, XMVectorSet(i * plat_collision_radius * 3, 0, 1.0f, 0.0f))));
	//}
	//// initialise middle platforms
	//for (size_t i = 0; i < middle_platform_count; i++)
	//{
	//	middle_platforms.push_back(GameObject("middlePlatform", platform, Transform(platform_scale, rotation, XMVectorSet(i * plat_collision_radius * 3, 0, 1.0f, 0.0f))));
	//}
	//// initialise lower platforms
	//for (size_t i = 0; i < lower_platform_count; i++)
	//{
	//	lower_platforms.push_back(GameObject("lowerPlatform", platform, Transform(platform_scale, rotation, XMVectorSet(i * plat_collision_radius * 3, 0, 1.0f, 0.0f))));
	//}

	//gameobjects.insert(gameobjects.end(), upper_platforms.begin(), upper_platforms.end());
	//gameobjects.insert(gameobjects.end(), middle_platforms.begin(), middle_platforms.end());
	//gameobjects.insert(gameobjects.end(), lower_platforms.begin(), lower_platforms.end());
	//player.add_child(&upper_platforms[0]);
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
