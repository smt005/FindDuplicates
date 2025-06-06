﻿// ◦ Xyz ◦

#include "FileManager.h"
#include <windows.h>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "FileUtils.h"
#include "Help.h"
#include "../Help/Log.h"

using namespace help;

std::filesystem::path FileManager::resourcesDir = std::filesystem::current_path();

std::string FileManager::GetFullFileNameDir()
{
	static std::string currentFileNamePath;
	if (currentFileNamePath.empty()) {
		char buffer[MAX_PATH];
		GetModuleFileNameA(nullptr, buffer, MAX_PATH);
		std::string bufferStr = buffer;
		std::replace(bufferStr.begin(), bufferStr.end(), '\\', '/');
		currentFileNamePath = FileUtils::GetDirectory(bufferStr);
	}

	return currentFileNamePath;
}

void FileManager::SetResourcesDir(const std::filesystem::path& argResourcesDir) {
	std::filesystem::path currentPath = std::filesystem::current_path();
	resourcesDir = std::filesystem::absolute(currentPath / argResourcesDir);
}

const std::filesystem::path& FileManager::GetResourcesDir() {
	return resourcesDir;
}

std::filesystem::path FileManager::FullPath(const std::filesystem::path& fileName) {
	return resourcesDir / fileName;
}

bool FileManager::HasFile(const std::filesystem::path& fileName) {
	std::filesystem::path fullFilePath = FullPath(fileName);
	return std::filesystem::exists(fullFilePath);
}

std::string FileManager::ReadTextFile(const std::filesystem::path& fileName) {
	std::filesystem::path fullFilePath = FullPath(fileName);

	std::ifstream fileStream(fullFilePath);
	if (!fileStream.is_open()) {
		LOG("Fail load: {}", fileName.string());
		return std::string();
	}

	fileStream.seekg(0, std::ios::end);

	std::string fileData;
	fileData.reserve(fileStream.tellg());
	fileStream.seekg(0, std::ios::beg);

	fileData.assign(std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>());
	fileStream.close();

	return fileData;
}

bool FileManager::WriteTextFile(const std::filesystem::path& fileName, const std::string& text) {
	std::filesystem::path fullFilePath = FullPath(fileName);
	std::filesystem::path fullPath = fullFilePath.parent_path();

	if (!std::filesystem::exists(fullPath)) {
		std::filesystem::create_directory(fullPath);
	}

	FILE* file;
	if (fopen_s(&file, fullFilePath.string().c_str(), "w") != NULL) {
		LOG("Fail save: {}", fileName.string());
		return false;
	}

	fputs(text.data(), file);
	fclose(file);
	return true;
}
