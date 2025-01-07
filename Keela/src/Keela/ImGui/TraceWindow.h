#pragma once
#include "vector"

#ifndef IMGUI_TRA_WINDOW_H
#define IMGUI_TRA_WINDOW_H

using Matrix = std::vector<std::vector<int>>;

namespace ImGui
{
    void ShowTraceWindow(bool* traces);
}

#endif // IMGUI_TRA_WINDOW_H