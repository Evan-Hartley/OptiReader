#pragma once
#include "vector"

#ifndef IMGUI_CAM_WINDOW_H
#define IMGUI_CAM_WINDOW_H

using Matrix = std::vector<std::vector<int>>;

namespace ImGui
{
    void ShowImageCtrlWindow(bool* p_open, const Matrix& img, int data_height, int data_width, int camIndex);
}

#endif // IMGUI_CAM_WINDOW_H