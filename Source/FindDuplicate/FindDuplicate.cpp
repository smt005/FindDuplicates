// ◦ Xyz ◦

#include "FindDuplicate.h"
#include "imgui.h"

#include "../Editor/PopupModal.h"
#include "../Help/FileManager.h"
#include "../Help/WindowsWin.h"
#include "../Help/Log.h"
#include "../Help/FileUtils.h"

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
        if (ImGui::Button(u8"Найти дубликаты", {200.f, 32.f})) {
            PairMap pairMap = CollectFileInfo(_inputFirst.GetText(), _inputSecond.GetText());
            FinDublicate(pairMap);
        }

        ImGui::SameLine();
        if (ImGui::Button(u8"Найти одиночные", { 200.f, 32.f })) {
            PairMap pairMap = CollectFileInfo(_inputFirst.GetText(), _inputSecond.GetText());
            FinUnique(pairMap);
        }

        ImGui::SameLine();
        if (ImGui::Button(u8"Дублировать", { 200.f, 32.f })) {
            if (_itSelectDublicate != _dublicates.end()) {
                CreateDublicate(**_itSelectDublicate);
            }
        }

        ImGui::SameLine();
        if (ImGui::Button(u8"Дублировать всё", { 200.f, 32.f })) {
            if (!_dublicates.empty()) {
                CreateDublicates(_dublicates);
            }
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

    PairMap FindDuplicate::CollectFileInfo(const std::string& firstDir, const std::string& secondDir) {
        PairMap pairMap;
        FindFiles(firstDir, pairMap.first);
        FindFiles(secondDir, pairMap.second);
        return pairMap;
    }
    
    void FindDuplicate::FindFiles(const std::filesystem::path& dir, std::unordered_map<std::string, std::string>& files) {
        if (!std::filesystem::exists(dir)) {
            LOG("[FindDuplicate::FindFiles] dir '{}' not founded.", dir);
            return;
        }

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
    
    void FindDuplicate::FinDublicate(PairMap& pairMap) {
        _dublicates.clear();

        std::unordered_map<std::string, std::string>& firstFiles = pairMap.first;
        std::unordered_map<std::string, std::string>& secondFiles = pairMap.second;

        for (auto& pair : firstFiles) {
            if (auto it = secondFiles.find(pair.first); it != secondFiles.end()) {
                _dublicates.emplace_back(new InfoFiles(pair.first, pair.second, it->second));
                _itSelectDublicate = _dublicates.end();
            }
        }

        _itSelectDublicate = _dublicates.end();
    }

    void FindDuplicate::FinUnique(PairMap& pairMap)
    {
        auto findUnique = [this](const auto& firstFiles, const auto& secondFiles, bool needSwap) {
            for (auto& pair : firstFiles) {
                if (auto it = secondFiles.find(pair.first); it == secondFiles.end()) {
                    if (needSwap) {
                        _dublicates.emplace_back(new InfoFiles(pair.first, {}, pair.second));
                    }
                    else {
                        _dublicates.emplace_back(new InfoFiles(pair.first, pair.second, {}));
                    }
                }
            }
        };

        _dublicates.clear();

        findUnique(pairMap.first, pairMap.second, false);
        findUnique(pairMap.second, pairMap.first, true);

        _itSelectDublicate = _dublicates.end();
    }

    void FindDuplicate::CreateDublicate(InfoFiles& infoFiles)
    {
        std::string fileNamePath = infoFiles.fileNameFirst;
        std::string copyFileNamePath = infoFiles.fileNameSecond;

        if (fileNamePath.empty()) {
            std::swap(fileNamePath, copyFileNamePath);
        }

        if (!std::filesystem::exists(fileNamePath)) {
            LOG("[FindDuplicate::CreateDublicate] path files '{}' not founded.", fileNamePath);
            return;
        }

        std::string firstDir = _inputFirst.GetText();
        std::string secondtDir = _inputSecond.GetText();

        size_t pos = fileNamePath.find(firstDir);
        if (pos == fileNamePath.npos) {
            pos = fileNamePath.find(secondtDir);

            if (pos != fileNamePath.npos) {
                std::swap(firstDir, secondtDir);
            }
            else {
                LOG("[FindDuplicate::CreateDublicate] relativePath not founded.");
                return;
            }
        }

        const size_t sizePredDir = firstDir.size();
        const std::string relativePath = fileNamePath.substr(sizePredDir, fileNamePath.size() - sizePredDir);
        copyFileNamePath = TO_STRING("{}{}", secondtDir, relativePath);

        if (std::filesystem::exists(copyFileNamePath)) {
            LOG("[FindDuplicate::CreateDublicate] file name path '{}' is busy!", copyFileNamePath);
            return;
        }

        const std::string copyFileNameDir = FileUtils::GetDirectory(copyFileNamePath);
        if (!std::filesystem::exists(copyFileNameDir)) {
            std::filesystem::create_directories(copyFileNameDir);
        }

        if (std::filesystem::copy_file(fileNamePath, copyFileNamePath)) {
            if (infoFiles.fileNameFirst.empty()) {
                std::swap(infoFiles.fileNameFirst, copyFileNamePath);
            }
            else {
                std::swap(infoFiles.fileNameSecond, copyFileNamePath);
            }
        }
        else {
            LOG("[FindDuplicate::CreateDublicate] error copy file '{}' to '{}'", fileNamePath, copyFileNamePath);
        }
    }

    void FindDuplicate::CreateDublicates(std::vector<std::unique_ptr<InfoFiles>>& dublicates)
    {
        for (const auto& dublicateInfo : dublicates) {
            CreateDublicate(*dublicateInfo);
        }
    }

    void FindDuplicate::OpenFile(const std::string& fileNamePath)
    {
        LOG("[FindDuplicate::OpenFile] open file '{}'", fileNamePath);
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
        if (_itSelectDublicate == _dublicates.end() || !*_itSelectDublicate) {
            ImGui::EndChild();
            return;
        }

        auto& infoFiles = *_itSelectDublicate;
        auto& texturePtr = infoFiles->texturePtr;

        const std::string& fileNameFirst = !infoFiles->fileNameFirst.empty() ? infoFiles->fileNameFirst : infoFiles->fileNameSecond;
        if (!texturePtr && help::FileManager::HasFile(fileNameFirst)) {            
            texturePtr = Texture::AddTexture(fileNameFirst, 0);
            texturePtr->Load();
        }

        if (texturePtr) {
            float verticalSpace = 0.f;
            float height = static_cast<float>(texturePtr->GetHeight());
            float width = static_cast<float>(texturePtr->GetWidth());

            constexpr float maxHeighthIcon = 400;
            constexpr float maxWidthIcon = 400;

            float yFactor = maxHeighthIcon / height;
            float newWidth = width * yFactor;
            if (newWidth > maxWidthIcon) {
                height *= maxWidthIcon / width;
                width = maxWidthIcon;
                verticalSpace = maxHeighthIcon - height;
            }
            else {
                height = maxHeighthIcon;
                width = newWidth;
            }

            ImGui::Dummy(ImVec2(0.f, 0.f));
            ImGui::Image(reinterpret_cast<ImTextureID>(**texturePtr),
                { width, height },
                { 0, 1 }, { 1, 0 }, { 1.f, 1.f, 1.f, 1.f }, { 1.f, 1.f, 1.f, 1.f });

            if (ImGui::IsItemHovered()) {
                if (ImGui::IsItemClicked()) {
                    help::OpenFile(fileNameFirst);
                }
            }

            if (verticalSpace > 0.f) {
                ImGui::Dummy(ImVec2(0.f, verticalSpace));
            }
        }
        else {
            ImGui::Dummy(ImVec2(0.f, 40.f));
            ImGui::TextColored(redColor, "%s", "Texture no founded.");
            ImGui::Dummy(ImVec2(0.f, 40.f));
        }

        ImGui::EndChild();
    }
}
