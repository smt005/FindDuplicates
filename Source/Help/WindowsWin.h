// ◦ Xyz ◦
#pragma once

#include <string>

namespace help {
	std::string ChooseDir();
	std::string SelectFile();
	bool OpenFile(const std::string fileNamePath);
	bool OpenFile(const std::wstring fileNamePath);
}
