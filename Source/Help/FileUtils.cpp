// ◦ Xyz ◦

#include "FileUtils.h"
#include "Log.h"

namespace FileUtils {
	std::string GetDirectory(const std::string& fileNamePath)
	{
		size_t pos = fileNamePath.rfind('\\');
		if (pos == fileNamePath.npos) {
			pos = fileNamePath.rfind('/');
		}

		if (pos == fileNamePath.npos) {
			LOG("[FileUtils::GetDirectory] Devider not founded in '{}'", fileNamePath);
			return {};
		}

		return fileNamePath.substr(0, pos);
	}
}
