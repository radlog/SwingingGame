#pragma once
#include <Windows.h>

// constants
const int enemy_count = 10;
const int platform_count = 100; // number of platforms
const auto scale_vector = XMVectorSet(1, 1, 1, 0); // platform scale
const auto rotation = XMQuaternionIdentity(); // identity rotation
const auto plat_distance_horizontal = 20; // horizontal distance between platforms
const auto plat_distance_vertical = 20; // vertical distance between platforms
const auto lava_tiles = 800; // number of tiles for the lava

HINSTANCE g_h_inst = nullptr; // program instance handle
HWND g_h_wnd = nullptr; // window handle