#include "Input.h"
#include "imgui.h"

using namespace Window;

namespace {
    ImVec4 redColor = { 0.9f, 0.1f, 0.1f, 1.f };
    ImVec4 whiteColor = { 1.f, 1.f, 1.f, 1.f };
}

unsigned int Input::guiIndex = 0;

bool Input::Display() {
    ImGui::PushItemWidth(_width);
    ImGui::PushStyleColor(ImGuiCol_WindowBg | ImGuiCol_Border, _error ? redColor : whiteColor);
    ImGui::PushID(_guiIndex);
    
    //const std::string guiId = std::to_string(_guiIndex) + "##" + std::to_string(_guiIndex);
    //if (ImGui::InputText(guiId.c_str(), _textBuffer.data(), _textBuffer.size())) {
    if (ImGui::InputText("", _textBuffer.data(), _textBuffer.size())) {
        if (_callback) {
            _error = !_callback(_textBuffer.data());
        }
    }

    ImGui::PopID();
    ImGui::PopStyleColor();
    ImGui::PopItemWidth();

    return _error;
}
