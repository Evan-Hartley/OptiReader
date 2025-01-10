#include "keepch.h"

#include "CameraWindow.h"
#include "imgui.h"
#include "implot.h"
#include "implot_internal.h"

using Matrix = std::vector<std::vector<int>>;

std::vector<double> flattenMatrix(const Matrix& img);
float* convertMatrixToArray(const Matrix& img);
void ShowGainSection(int& gain);
void ShowImageSection(const Matrix& img, float& rmin, float& rmax);
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

// Function to flatten a 2D matrix into a 1D vector
std::vector<double> flattenMatrix(const Matrix& img) {
    std::vector<double> flattened;

    for (const auto& row : img) {
        for (int value : row) {
            flattened.push_back(static_cast<double>(value)); // Convert to double if necessary
        }
    }

    return flattened;
}

float* convertMatrixToArray(const Matrix& img) {
    size_t rows = img.size();
    size_t cols = img[0].size();

    // Allocate a 1D array with size rows * cols
    float* values = new float[rows * cols];

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            // Flatten into a row-major 1D array
            values[i * cols + j] = static_cast<float>(img[i][j]);
        }
    }

    return values;
}

// Function to map matrix values in range 1-255 to {0.0f, 0.25f, 0.5f, 0.75f, 1.0f}
std::vector<float> mapToPredefinedRange(const std::vector<int>& values) {
    std::vector<float> mappedValues;

    for (int value : values) {
        if (value >= 1 && value <= 51) {
            mappedValues.push_back(0.0f);
        }
        else if (value >= 52 && value <= 102) {
            mappedValues.push_back(0.25f);
        }
        else if (value >= 103 && value <= 153) {
            mappedValues.push_back(0.5f);
        }
        else if (value >= 154 && value <= 204) {
            mappedValues.push_back(0.75f);
        }
        else if (value >= 205 && value <= 255) {
            mappedValues.push_back(1.0f);
        }
    }

    return mappedValues;
}

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

void ShowImageSection(const Matrix& img, float& rmin, float& rmax)
{
    ImGui::SeparatorText("Processed Image");
    ImGui::Spacing();

    int height = img.size();
    int width = (height > 0) ? img[0].size() : 0;

    float* pic = convertMatrixToArray(img);

    static ImPlotAxisFlags axes_flags = ImPlotAxisFlags_Lock | ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickMarks;
    static ImPlotHeatmapFlags hm_flags = 0;

    if (ImPlot::BeginPlot("Camera Image", ImVec2(500, 500), ImPlotFlags_NoLegend | ImPlotFlags_NoMouseText)) {
        ImPlot::SetupAxes(nullptr, nullptr, axes_flags, axes_flags);
        ImPlot::SetupAxisTicks(ImAxis_X1, 1, 3, 3, nullptr);
        ImPlot::SetupAxisTicks(ImAxis_Y1, 1, 3, 3, nullptr);

        // Make sure only to plot if data has changed
        ImPlot::PlotHeatmap("Image", pic, 3, 3, rmin, rmax, nullptr, ImPlotPoint(0, 0), ImPlotPoint(1, 1), hm_flags);
        ImPlot::EndPlot();
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

    std::vector<double> flatData = flattenMatrix(img);
    

    static bool range = true;
    ImGui::Checkbox("Range", &range);
    static float rmin = 0;
    static float rmax = 255;
    static float bins = 256;
    if (range) {
        ImGui::InputFloat("Minimum", &rmin, 1.0f, 254.0f, "%1.0f");
        ImGui::InputFloat("Maximum", &rmax, 2.0f, 255.0f, "%1.0f");
    }

    std::vector<double> line_height = { 0.0, 1.0 };
    std::vector<double> rmin_line = { rmin, rmin };
    std::vector<double> rmax_line = { rmax, rmax };

    if (ImPlot::BeginPlot("Normalization Histogram",ImVec2(-1,0), ImPlotFlags_NoLegend)) {
        ImPlot::SetNextFillStyle(ImVec4(0.00f, 0.00f, 0.00f, 1.00f), 0.8f);
        ImPlot::SetupAxes("", "", ImPlotAxisFlags_NoTickLabels, ImPlotAxisFlags_NoTickLabels);
        ImPlot::PlotHistogram("Intensities", flatData.data(), sizeof(flatData.data()), bins, 1.0, range ? ImPlotRange(0, 255) : ImPlotRange());
        ImPlot::SetNextLineStyle(ImVec4(1.00f, 0.01f, 0.01f, 1.00f), 5.0f);
        ImPlot::PlotLine("Minimum", rmin_line.data(), line_height.data(), 2);
        ImPlot::SetNextLineStyle(ImVec4(0.01f, 0.01f, 1.00f, 1.00f), 5.0f);
        ImPlot::PlotLine("Maximum", rmax_line.data(), line_height.data(), 2);
        ImPlot::EndPlot();
    }


    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

    // Sections
    ShowGainSection(state.gain);  // Pass gain as reference
    ShowOrientationSection(state.rot90, state.rot180, state.rot270, state.flipx, state.flipy, state.orientation_current);

    Matrix rotatedImg = ApplyRotation(img, data_height, data_width, state.rot90, state.rot180, state.rot270);
    Matrix flipxImg = ApplyFlipX(rotatedImg, data_width, state.flipx);
    Matrix flipxyImg = ApplyFlipY(flipxImg, data_height, data_width, state.flipy);
    ShowImageSection(flipxyImg, rmin, rmax);

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

