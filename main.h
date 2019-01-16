#pragma once
#include <Windows.h>

// constants
const int enemy_count = 10;
const int platform_count = 100; // number of platforms
const auto scale_vector = XMVectorSet(1, 1, 1, 0); // platform scale
const auto rotation = XMQuaternionIdentity(); // identity rotation
const auto plat_distance_horizontal = 20;
const auto plat_distance_vertical = 20;
const auto lava_tiles = 800;

HINSTANCE g_hInst = nullptr;
HWND g_hWnd = nullptr;