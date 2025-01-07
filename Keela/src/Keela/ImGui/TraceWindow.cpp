#include "keepch.h"
#include "TraceWindow.h"
#include "imgui.h"

void ImGui::ShowTraceWindow(bool* traces)
{
    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context.");

    IMGUI_CHECKVERSION();

    ImGuiWindowFlags window_flags = 0;
    // Your window flag setup here...

    if (!ImGui::Begin("Traces", traces, window_flags))
    {
        ImGui::End();
        return;
    }
    ImGui::Text("This is where traces will go.");
    ImGui::End();
}