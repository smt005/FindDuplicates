// ◦ Xyz ◦

#include "../Help/WindowsWin.h"
#include <ShlObj_core.h>
#include <Windows.h>
#include <shobjidl.h> 
#include <algorithm>
#include "../Help/Help.h"
#include "../Help/Log.h"

namespace help {
    int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData) {
        switch (uMsg) {
        case BFFM_INITIALIZED:
            // Можно установить начальную папку, если lpData содержит путь
            if (lpData) {
                SendMessageW(hwnd, BFFM_SETSELECTIONW, TRUE, lpData);
            }
            break;
        case BFFM_SELCHANGED:
            // Можно выполнить действия при изменении выбранной папки
            break;
        default:
            break;
        }
        return 0;
    }

    std::wstring BrowseForFolder() {
        BROWSEINFOW bi = { 0 };
        wchar_t buffer[MAX_PATH];
        LPITEMIDLIST pidl;
        std::wstring selectedFolderPath;

        bi.hwndOwner = nullptr; // Дескриптор родительского окна (может быть nullptr)
        bi.pidlRoot = nullptr;  // PIDL корневой папки (может быть nullptr для "Рабочего стола")
        bi.pszDisplayName = buffer; // Буфер для отображаемого имени выбранной папки
        bi.lpszTitle = L"Выберите папку:"; // Заголовок диалогового окна
        bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI; // Флаги (только файловые папки, новый UI)
        bi.lpfn = BrowseCallbackProc; // Указатель на callback-функцию (необязательно)
        bi.lParam = 0;             // Данные, передаваемые в callback-функцию
        bi.iImage = 0;             // Индекс изображения (не используется)

        pidl = SHBrowseForFolderW(&bi);

        if (pidl != nullptr) {
            // Получаем путь к выбранной папке из PIDL
            if (SHGetPathFromIDListW(pidl, buffer)) {
                selectedFolderPath = buffer;
            }

            // Освобождаем PIDL
            CoTaskMemFree(pidl);
        }

        return selectedFolderPath;
    }
}

std::string help::ChooseDir() {
    // Инициализируем COM-библиотеку
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (FAILED(hr)) {
        LOG("[help::ChooseDir] Ошибка инициализации COM.");
        return {};
    }

    const std::wstring choosedPath = BrowseForFolder();

    // Освобождаем COM-библиотеку
    CoUninitialize();

    return help::WstrToStr(choosedPath);
}

std::string help::SelectFile() {
    HRESULT result = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(result))
        return std::string();

    //...
    IFileOpenDialog* fileSystem;
    result = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&fileSystem));
    if (FAILED(result)) {
        CoUninitialize();
        return std::string();
    }

    //...
    result = fileSystem->Show(NULL);
    if (FAILED(result)) {
        fileSystem->Release();
        CoUninitialize();
        return std::string();
    }

    //...
    IShellItem* files;
    result = fileSystem->GetResult(&files);
    if (FAILED(result)) {
        fileSystem->Release();
        CoUninitialize();
        return std::string();
    }

    //...
    PWSTR filePath;
    result = files->GetDisplayName(SIGDN_FILESYSPATH, &filePath);
    if (FAILED(result)) {
        files->Release();
        fileSystem->Release();
        CoUninitialize();
        return std::string();
    }

    //...
    std::wstring path(filePath);
    std::string sFilePath(path.begin(), path.end());

    //...
    CoTaskMemFree(filePath);
    files->Release();
    fileSystem->Release();
    CoUninitialize();

    return sFilePath;
}

bool help::OpenFile(const std::string fileNamePath)
{
    const std::wstring wfileNamePath = help::StrToWstr(fileNamePath);
    return help::OpenFile(wfileNamePath);
}

bool help::OpenFile(const std::wstring wfileNamePath)
{
    HINSTANCE result = ShellExecuteW(
        nullptr,
        L"open",
        wfileNamePath.c_str(),
        nullptr,
        nullptr,
        SW_SHOWNORMAL
    );

    if ((INT_PTR)result <= 32) {
        //errorText = TO_STRING("[help::OpenFile] file '{}' not open: ", wfileNamePath);
       // std::cerr << errorText << (INT_PTR)result << std::endl;
        return false;
    }

    return true;
}
