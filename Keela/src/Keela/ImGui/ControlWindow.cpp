#include "keepch.h"
#include "ControlWindow.h"
#include "CameraWindow.h"
#include "TraceWindow.h"
#include "imgui.h"

using Matrix = std::vector<std::vector<int>>;

void ShowDataCollectionSection();
void ShowFrameRateSection();
std::pair<int, int> ShowDataMatrixDimensionsSection();
void ShowCameraNumberSection();
void ShowDataVisualizationSection();
void ShowTroubleShootingSection();

void ImGui::ShowControlWindow(bool* p_open)
{
    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context.");

    IMGUI_CHECKVERSION();

    static bool no_titlebar = false, no_scrollbar = false, no_menu = true, no_move = false;
    static bool no_resize = false, no_collapse = false, no_close = false, no_nav = false;
    static bool no_background = false, no_bring_to_front = false, no_docking = false, unsaved_document = false;

    ImGuiWindowFlags window_flags = 0;
    if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
    if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
    if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
    if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
    if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
    if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
    if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    if (no_docking)         window_flags |= ImGuiWindowFlags_NoDocking;
    if (unsaved_document)   window_flags |= ImGuiWindowFlags_UnsavedDocument;
    if (no_close)           p_open = NULL;

    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Main Control Window", p_open, window_flags))
    {
        ImGui::End();
        return;
    }

    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

    // Sections
    ShowDataCollectionSection();
    ShowFrameRateSection();
    ShowDataMatrixDimensionsSection();
    ShowCameraNumberSection();
    ShowDataVisualizationSection();
    ShowTroubleShootingSection();

    ImGui::PopItemWidth();
    ImGui::End();
}

void ShowDataCollectionSection()
{
    ImGui::Text("Data Collection");
    ImGui::Spacing();
    ImGui::SeparatorText("Recording");
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7 / 7.0f, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7 / 7.0f, 0.8f, 0.8f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7 / 7.0f, 0.9f, 0.9f));
    static int rec = false;
    if (ImGui::Button("Record"))
        rec++;
    if (rec & 1)
    {
        ImGui::SameLine();
        ImGui::Text("Recording Data");
    }
    ImGui::PopStyleColor(3);
}

void ShowFrameRateSection()
{
    ImGui::SeparatorText("Frame Rate");
    static float frame_rate = 0.5;
    ImGui::InputFloat("Frame Rate", &frame_rate, 0.1f, 1.0f, "%.1f");
}

std::pair<int, int> ShowDataMatrixDimensionsSection()
{
    ImGui::Text("Data Matrix Dimensions");
    static int data_width = 3;
    ImGui::InputInt("Data Width", &data_width);
    static int data_height = 3;
    ImGui::InputInt("Data Height", &data_height);

    static bool cal_volt = false;
    ImGui::Text("Recording Type");
    ImGui::Checkbox("Calcium-Voltage Recording Setting", &cal_volt);

    // Return both data_width and data_height as a pair
    return { data_width, data_height };
}


void ShowCameraNumberSection() {
    static int cam_num = 1; // Camera number
    static bool isLocked = false; // Lock state

    Matrix img = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
    };

    int data_height = img.size();
    int data_width = img[0].size();

    // Checkbox to lock/unlock the camera number input
    ImGui::Checkbox("Lock Camera Number", &isLocked);

    for (int camIndex = 0; camIndex < cam_num; ++camIndex) {
        bool open = true;
        ImGui::ShowImageCtrlWindow(&open, img, data_height, data_width, camIndex);
    }

    if (isLocked) {
        // Display the camera number as text if locked
        ImGui::Text("Number of Cameras: %d", cam_num);
    }
    else {
        // Allow the user to input the number if unlocked
        ImGui::InputInt("Number of Cameras", &cam_num);
    }
}

void ShowDataVisualizationSection()
{
    ImGui::Spacing();
    ImGui::Text("Data Visualization");
    ImGui::Spacing();
    static bool traces = false;
    ImGui::Checkbox("Show Traces", &traces);

    if (traces) {
        ImGui::ShowTraceWindow(&traces);
    }
}

void ShowTroubleShootingSection()
{
    ImGui::Spacing();
    ImGui::Text("Trouble Shooting");
    ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(3 / 7.0f, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(3 / 7.0f, 0.8f, 0.8f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(3 / 7.0f, 0.9f, 0.9f));
    static bool restart = false;
    if (ImGui::Button("Restart Camera(s)"))
        restart = true;
    if (restart)
    {
        ImGui::SameLine();
        ImGui::Text("Camera(s) Restarted");
    }
    ImGui::PopStyleColor(3);
}
