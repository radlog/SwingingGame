#include "main.h"

#include "LavaFloor.h"
#include "Floor.h"
#include "Player.h"
#include "GeoCube.h"
#include "Model.h"
#include "Enemy.h"
#include "Text2D.h"

#include "Camera.h"
#include "GameObject.h"
#include "Skybox.h"

// directX instance handle
D3Dfw *dx_handle = D3Dfw::get_instance();

// 2d text
Text2D *fps_info;
Text2D *stat_info;

// models
Model* cube;
Model *island_model;
Model* enemy;

// game objects
GameObject scene_root; // root object
GameObject* floor_object;
GameObject *lava;
Player *player;
LavaFloor *lava_floor;
Floor *floor_model;
Skybox skybox;

// input
Input input;

// game timer
VGTime* timer;

// lighting
XMVECTOR directional_light_shines_from = XMVectorSet(0.0f, 0.5f, 1.0f, 0.0f); // light direction front / down
XMVECTOR directional_light_colour = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f); // white color
XMVECTOR ambient_light_colour = XMVectorSet(0.1f, 0.1f, 0.1f, 1.0f); // dark grey

// the window title
char g_title[100] = "Swing to Win(g)";

// methods
HRESULT init_dx(HINSTANCE instance, HINSTANCE prev_instance, LPSTR lp_cmd_line, int n_cmd_show);
LRESULT CALLBACK wnd_proc(HWND, UINT, WPARAM, LPARAM);

// scene loading
void load_content();
void load_lava();
void load_enemies(GameObject* root);
void load_map(GameObject* root, float scale);

// updates
void update(VGTime *timer);

// draw method
void render_frame(Camera *camera);

// cleanup for when the game ends
void end_game();



//// simple debug method to test values
//void debug_util(int str);


// main function
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
			//render_frame(player->get_top_down_camera()); // second camera (top down view) for map overview
		}
	}

	end_game();
	return static_cast<int>(msg.wParam); // return exit message
}



void render_frame(Camera *camera)
{
	const auto view_projection = camera->calculate_view_projection(); // get camera view projection to project the 3d space to 2d 
	const auto sky_lock = XMMatrixTranslationFromVector(camera->get_transform()->get_local_position()) * view_projection; // move skybox transform with the camera

	dx_handle->clear_rtv(); // clear the render target view

	// draw start
	skybox.draw(sky_lock); // draw skybox

	// update constant buffer values for every gameobject in the scene
	scene_root.update_constant_buffer_time_scaled(view_projection, view_projection, directional_light_shines_from, directional_light_colour, ambient_light_colour, timer->total_time());
	scene_root.draw(view_projection); // draw scene

	// draw text 
	fps_info->RenderText(); // average fps text
	stat_info->RenderText(); // kills, deaths, score text
	//particles.draw(&view_projection); // does not work somehow, potential to improve, but time was short
	// draw end

	dx_handle->get_swap_chain()->Present(0, 0);	// swap back buffer with front buffer
}



void update(VGTime* timer)
{
	const auto kills = "kills " + std::to_string(player->get_stats().kills); // player kills text
	const auto deaths = "deaths " + std::to_string(player->get_stats().deaths); // player deaths text
	const auto score = "score " + std::to_string(player->get_stats().score); // player score text
	const auto life = "life " + std::to_string(player->get_life()); // player life text
	fps_info->AddText("FPS " + std::to_string(timer->get_fps()), -0.5, +1.0, 0.1); // add fps info text
	stat_info->AddText(kills, -1.0, +1.0, 0.05); // add kills text
	stat_info->AddText(deaths, -1.0, +0.95, 0.05); // add deaths text
	stat_info->AddText(score, -1.0, +0.90, 0.05); // add score text
	stat_info->AddText(life, -1.0, +0.85, 0.05); // add life text

	scene_root.update(timer); // update scene with all its children
}


// cleanup all pointers when application closes to prevent memory leaks
void end_game()
{
	timer->stop();
	scene_root.cleanup();
	dx_handle->cleanup();	
}


// was only used for debugging, excluded from build
//void debug_util(const int str)
//{
//	std::ostringstream ss;
//	ss << str;
//	string s = ss.str() + "\n";
//	OutputDebugString(s.c_str());
//}

void load_lava()
{

}

void load_enemies(GameObject* root)
{
	for (size_t i = 0; i < enemy_count; i++)
	{
		const auto pl = new Enemy("enemy", enemy, new Transform(XMVectorSplatOne(), XMQuaternionIdentity(), XMVectorSet((i + 1) * plat_distance_horizontal, plat_distance_vertical * 1.5, (i + 1) * plat_distance_horizontal, 0)));
		root->add_child(pl);
	}
}

void load_map(GameObject* root, const float scale)
{
	scene_root.add_child(lava); // add lava to scene

	// initialise lower platforms
	for (size_t i = 0; i < static_cast<int>(sqrt(platform_count)); i++)
	{
		for (size_t j = 0; j < static_cast<int>(sqrt(platform_count)); j++)
		{
			const auto pl = new GameObject("lower_platform", island_model, new Transform(scale_vector, rotation, XMVectorSet(i * plat_distance_horizontal, plat_distance_vertical, j * plat_distance_horizontal, 0)));
			root->add_child(pl); // add to scene
		}
	}

	// initialise middle platforms
	for (size_t i = 0; i < static_cast<int>(sqrt(platform_count)); i++)
	{
		for (size_t j = 0; j < static_cast<int>(sqrt(platform_count)); j++)
		{
			const auto pl = new GameObject("middle_platform", island_model, new Transform(scale_vector, rotation, XMVectorSet(i * plat_distance_horizontal, plat_distance_vertical * 2, j * plat_distance_horizontal, 0)));
			root->add_child(pl); // add to scene		
		}
	}

	// initialise upper platforms
	for (size_t i = 0; i < static_cast<int>(sqrt(platform_count)); i++)
	{
		for (size_t j = 0; j < static_cast<int>(sqrt(platform_count)); j++)
		{
			const auto pl = new GameObject("upper_platform", island_model, new Transform(scale_vector, rotation, XMVectorSet(i * plat_distance_horizontal, plat_distance_vertical * 3, j * plat_distance_horizontal, 0)));
			root->add_child(pl); // add to scene			
		}
	}
}


// loads all objects in the scene and the scene root
void load_content()
{
	timer = new VGTime(); // the game timer for steady movement
	skybox = Skybox("assets/purple_nebular.dds"); // skybox that loads a cube-map

	// initialize floating platform model
	island_model = new Model("assets/FloatingIsland_001.obj", CB_STATE_TIME_SCALED); // platform model i made by myself
	island_model->load_texture("assets/crate.jpg"); // texture of the platform model
	island_model->set_shader_file("lighted_shader.hlsl"); // set shader for the model

	// initialize enemy model
	enemy = new Model("assets/cube.obj", CB_STATE_TIME_SCALED); // cube model
	enemy->load_texture("assets/lava_selfmade_diffuse.png"); // texture of the enemy model
	enemy->set_shader_file("lighted_shader.hlsl"); // set shader for the model

	// initialize player gameobject
	player = new Player("player1"); // the player gameobject
	player->set_transform(new Transform(XMVectorSplatOne(), XMQuaternionIdentity(), XMVectorSet(0, 30, 0, 0))); // set initial player position
	player->update(timer); // call one update to set player to the correct position

	// initialize lava gameobject
	lava_floor = new LavaFloor(lava_tiles); // lava model
	lava = new GameObject("lava", lava_floor, new Transform(), LAVA); // lava gameobject
	lava->get_transform()->translate(lava_floor->get_transform()->get_local_position()); // translate the lave to be centered along the x and y axis

	// initialize scene root
	scene_root = GameObject("scene_root"); // root object
	scene_root.set_kinetic(false); // make root static not affected by gravity
	scene_root.add_child(player); // add player to root as first to ensure he gets updates before all other objects



	load_map(&scene_root, 1);
	load_enemies(&scene_root);
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

	fps_info = new Text2D("assets/font1.bmp", dx_handle->get_device(), dx_handle->get_immediate_context());
	stat_info = new Text2D("assets/font1.bmp", dx_handle->get_device(), dx_handle->get_immediate_context());

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

