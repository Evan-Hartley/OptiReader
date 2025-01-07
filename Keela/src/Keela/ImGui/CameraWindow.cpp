#include "keepch.h"
#include "CameraWindow.h"
#include "imgui.h"

using Matrix = std::vector<std::vector<int>>;

void ShowGainSection(int& gain);
void ShowImageSection(const Matrix& img);
Matrix rotateMatrix90Clockwise(const Matrix& img, int data_height, int data_width);
Matrix rotateMatrix180Clockwise(const Matrix& img, int data_height, int data_width);
Matrix rotateMatrix270Clockwise(const Matrix& img, int data_height, int data_width);
void ShowOrientationSection(bool& rot90, bool& rot180, bool& rot270, bool& flipx, bool& flipy, int& orientation_current);


struct WindowState {
    bool rot90 = false;
    bool rot180 = false;
    bool rot270 = false;
    bool flipx = false;
    bool flipy = false;
    int gain = 1;
    int orientation_current = 0; // Added for rotation combo tracking
};


// Function to rotate matrix 90 degrees clockwise
Matrix rotateMatrix90Clockwise(const Matrix& img, int data_height, int data_width)
{
    Matrix rotatedMatrix(data_width, std::vector<int>(data_height));

    for (int i = 0; i < data_height; ++i) {
        for (int j = 0; j < data_width; ++j) {
            rotatedMatrix[j][data_height - i - 1] = img[i][j];
        }
    }
    return rotatedMatrix;
}

// Function to rotate matrix 180 degrees clockwise
Matrix rotateMatrix180Clockwise(const Matrix& img, int data_height, int data_width)
{
    Matrix rotatedMatrix(data_height, std::vector<int>(data_width));

    for (int i = 0; i < data_height; ++i) {
        for (int j = 0; j < data_width; ++j) {
            rotatedMatrix[data_height - i - 1][data_width - j - 1] = img[i][j];
        }
    }
    return rotatedMatrix;
}

// Function to rotate matrix 270 degrees clockwise
Matrix rotateMatrix270Clockwise(const Matrix& img, int data_height, int data_width)
{
    Matrix rotatedMatrix(data_width, std::vector<int>(data_height));

    for (int i = 0; i < data_height; ++i) {
        for (int j = 0; j < data_width; ++j) {
            rotatedMatrix[data_width - j - 1][i] = img[i][j];
        }
    }
    return rotatedMatrix;
}

Matrix ApplyFlipX(const Matrix& img, int data_width, bool flipx)
{
    if (!flipx) return img;
    Matrix flippedImg = img;
    for (auto& row : flippedImg) {
        std::reverse(row.begin(), row.end());
    }
    return flippedImg;
}

Matrix ApplyFlipY(const Matrix& img, int data_height, int data_width, bool flipy)
{
    if (!flipy) return img;
    Matrix flippedImg = img;
    std::reverse(flippedImg.begin(), flippedImg.end());
    return flippedImg;
}


// Main function to check rotation and apply based on selected option
Matrix ApplyRotation(const Matrix& img, int data_height, int data_width, bool rot90, bool rot180, bool rot270)
{
    if (rot90) {
        return rotateMatrix90Clockwise(img, data_height, data_width);
    }
    else if (rot180) {
        return rotateMatrix180Clockwise(img, data_height, data_width);
    }
    else if (rot270) {
        return rotateMatrix270Clockwise(img, data_height, data_width);
    }
    return img; // Return the original matrix if no rotation is selected
}

// Helper function to print the matrix
void printMatrix(const Matrix& matrix) {
    for (const auto& row : matrix) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
}

void ShowGainSection(int& gain)
{
    ImGui::InputInt("Gain", &gain);
}

void ShowImageSection(const Matrix& img)
{
    ImGui::SeparatorText("Processed Image");
    ImGui::Spacing();

    int height = img.size();
    int width = (height > 0) ? img[0].size() : 0;

    // Display the first element if it exists
    if (height > 0 && width > 0) {
        ImGui::Text("First Element: %d", img[0][0]);
    }
    else {
        ImGui::Text("First Element: (none)");
    }
}

// Use a map to store the state for each camIndex
std::unordered_map<int, WindowState> windowStates;

void ImGui::ShowImageCtrlWindow(bool* p_open, const Matrix& img, int data_height, int data_width, int camIndex)
{
    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context.");

    IMGUI_CHECKVERSION();

    WindowState& state = windowStates[camIndex];  // Access the state for the current window

    ImGuiWindowFlags window_flags = 0;
    // Your window flag setup here...

    std::stringstream ss;
    ss << "Image and Control for Camera " << camIndex + 1;

    if (!ImGui::Begin(ss.str().c_str(), p_open, window_flags))
    {
        ImGui::End();
        return;
    }

    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

    // Sections
    ShowGainSection(state.gain);  // Pass gain as reference
    ShowOrientationSection(state.rot90, state.rot180, state.rot270, state.flipx, state.flipy, state.orientation_current);

    Matrix rotatedImg = ApplyRotation(img, data_height, data_width, state.rot90, state.rot180, state.rot270);
    Matrix flipxImg = ApplyFlipX(rotatedImg, data_width, state.flipx);
    Matrix flipxyImg = ApplyFlipY(flipxImg, data_height, data_width, state.flipy);
    ShowImageSection(flipxyImg);

    ImGui::PopItemWidth();
    ImGui::End();
}

void ShowOrientationSection(bool& rot90, bool& rot180, bool& rot270, bool& flipx, bool& flipy, int& orientation_current)
{
    ImGui::SeparatorText("Select Orientation");
    ImGui::Spacing();
    const char* orientations[] = { " --- ", "Rotate 90 Degrees", "Rotate 180 Degrees", "Rotate 270 Degrees" };
    ImGui::Combo("Select Rotation", &orientation_current, orientations, IM_ARRAYSIZE(orientations));

    rot90 = rot180 = rot270 = false; // Reset
    if (orientation_current == 1) rot90 = true;
    if (orientation_current == 2) rot180 = true;
    if (orientation_current == 3) rot270 = true;

    ImGui::Checkbox("Flip Vertically", &flipx);
    ImGui::Checkbox("Flip Horizontally", &flipy);
}

