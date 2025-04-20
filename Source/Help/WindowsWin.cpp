// ◦ Xyz ◦

#include "../Help/WindowsWin.h"

#include <ShlObj_core.h>
#include <Windows.h>
#include <shobjidl.h> 
#include <algorithm>
#include "../Help/Help.h"
#include "../Help/Log.h"

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
