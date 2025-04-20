// ◦ Xyz ◦
#pragma once

#include <string>

namespace FileUtils {
	std::string GetDirectory(const std::string& fileNamePath);


	template <typename Str>
	Str& ChangeToSlash(Str& namePath)
	{
		for (auto& ch : namePath) {
			if (ch == '/') {
				ch = '\\';
			}
		}
		return namePath;
	}

	template <typename Str>
	Str& ChangeToForwardSlash(Str& namePath)
	{
		for (auto& ch : namePath) {
			if (ch == '\\') {
				ch = '/';
			}
		}
		return namePath;
	}
}
