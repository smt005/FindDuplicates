// ◦ Xyz ◦

#include "FindDuplicate.h"
#include "imgui.h"

#include "../Editor/PopupModal.h"
#include "../Help/FileManager.h"
#include "../Help/WindowsWin.h"
#include "../Help/Log.h"

namespace Window {
    namespace {
        float border = 20.f;

        ImVec4 redColor = {0.9f, 0.1f, 0.1f, 1.f};
        ImVec4 blackColor = { 0.0f, 0.0f, 0.0f, 1.f };
        ImVec4 whiteColor = { 1.f, 1.f, 1.f, 1.f };
        ImVec4 grayColor = { 0.9f, 0.9f, 0.9f, 1.f };
        ImVec4 greenColor = { 0.9f, 0.9f, 0.9f, 1.f };
    }

    FindDuplicate::FindDuplicate(const std::filesystem::path& resourcePath, const std::string& resourceFileName)
        : _windowId(typeid(FindDuplicate).name())
        , _windowFlags(ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove) 
        , _resourceFileName(resourceFileName)
	{
        std::filesystem::path path = std::filesystem::current_path();
        path += resourcePath;
        help::FileManager::SetResourcesDir(path);
	}

	void FindDuplicate::Resize(float width, float height) {        
        ImGui::SetWindowPos(_windowId, { 0.f, 0.f });
		ImGui::SetWindowSize(_windowId, { width, height });

        _widthWindow = width;
        _heightWindow = height;

        _inputFirst.SetWidth(_widthWindow - border);
        _inputSecond.SetWidth(_widthWindow - border);
	}

    void FindDuplicate::InitDisplay(float width, float height) {
        ImGui::Begin(_windowId, nullptr, _windowFlags);
        Resize(width, height);
        ImGui::End();

        InitInterface();
    }

	void FindDuplicate::Render() {
		ImGui::Begin(_windowId, nullptr, _windowFlags);

        _inputFirst.Display();
        _inputSecond.Display();

        ImGui::Dummy(ImVec2(0.f, 0.f));
        if (ImGui::Button("Find", {200.f, 32.f})) {
            FinDublicate(_inputFirst.GetText(), _inputSecond.GetText());
        }

        if (!_dublicates.empty()) {
            ListDisplay();
        }

        InfoDisplay();

        //PopupModal::Draw();
		ImGui::End();
	}

    void FindDuplicate::InitInterface() {
        _inputFirst.SetWidth(_widthWindow - border);
        _inputSecond.SetWidth(_widthWindow - border);

        _inputFirst.SetText(u8"C:/Папка Y");
        _inputSecond.SetText(u8"C:/Папка Z");
    }

    void FindDuplicate::FinDublicate(const std::string& firstPath, const std::string& secondPath) {
        std::map<std::string, std::string> firstFiles;
        std::map<std::string, std::string> secondFiles;

        const std::string firstDir = _inputFirst.GetText();
        LOG("firstDir: {}", firstDir);
        FindFiles(_inputFirst.GetText(), firstFiles);

        LOG(std::string("Second: ...   ...   ..."));
        FindFiles(_inputSecond.GetText(), secondFiles);

        if (firstFiles.size() < secondFiles.size()) {
            FinDublicate(firstFiles, secondFiles);
        }
        else {
            FinDublicate(secondFiles, firstFiles);
        }
    }

    void FindDuplicate::FinDublicate(std::map<std::string, std::string>& firstFiles, std::map<std::string, std::string>& secondFiles) {
        for (auto& pair : firstFiles) {
            if (auto it = secondFiles.find(pair.first); it != secondFiles.end()) {
                _dublicates.emplace_back(new InfoFiles(pair.first, pair.second, it->second));
                _itSelectDublicate = _dublicates.end();
            }
        }

        _itSelectDublicate = _dublicates.end();
    }

    void FindDuplicate::FindFiles(const std::filesystem::path& dir, std::map<std::string, std::string>& files) {
        for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(dir)) {
            if (entry.is_directory()) {
                FindFiles(entry, files);
            }
            else {
                std::string fullFilePath = entry.path().string();
                size_t size = fullFilePath.size();

                size_t slashPos = fullFilePath.rfind('/');
                size_t slash2Pos = fullFilePath.rfind('\\');
                slashPos = slashPos > slash2Pos ? slashPos : slash2Pos;

                if (slashPos == fullFilePath.npos) {
                    continue;
                }

                ++slashPos;
                if (slashPos >= size) {
                    continue;
                }

                std::string name = fullFilePath.substr(slashPos, (size - slashPos));
                LOG("[{}] {}", name, fullFilePath);

                files.emplace(name, fullFilePath);
            }
        }
    }


    void FindDuplicate::Load() {
    }

    void FindDuplicate::Save() {
    }

    // Private

    void FindDuplicate::ListDisplay() {
        ImGui::BeginChild("list", { (_widthWindow - border), (_heightWindow / 2.f - 55.f)}, true);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, _widthWindow);
        ImGui::SetColumnWidth(1, 1.f);

        for (auto it = _dublicates.begin(); it != _dublicates.end(); ++it) {
            bool select = it == _itSelectDublicate ? true : false;
            InfoFiles& info = **it;

            ImGui::TextColored(select ? redColor : blackColor, "%s", info.name.c_str());
            if (ImGui::IsItemHovered()) {
                if (ImGui::IsItemClicked()) {
                    _itSelectDublicate = it;
                }
            }

            ImGui::Dummy(ImVec2(0.f, 0.f));
            ImGui::TextColored(select ? redColor : blackColor, "%s", info.fileNameFirst.c_str());

            ImGui::Dummy(ImVec2(0.f, 0.f));
            ImGui::TextColored(select ? redColor : blackColor, "%s", info.fileNameSecond.c_str());

            ImGui::Separator();

            ImGui::NextColumn();
            ImGui::NextColumn();
        }

        ImGui::EndChild();
    }

    void FindDuplicate::InfoDisplay() {
        ImGui::BeginChild("info", { (_widthWindow - border), (_heightWindow / 2.f - 55.f) }, true);
        if (_itSelectDublicate == _dublicates.end()) {
            ImGui::EndChild();
            return;
        }
        
        ImGui::EndChild();
    }
}
