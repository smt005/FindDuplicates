// ◦ Xyz ◦
#pragma once

#include <string>

namespace FileUtils {
	std::string GetDirectory(const std::string& fileNamePath);
	std::string& ChangeTodSlash(std::string& namePath);
	std::string& ChangeToForwardSlash(std::string& namePath);
}
