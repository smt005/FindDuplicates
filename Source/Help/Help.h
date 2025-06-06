﻿// ◦ Xyz ◦
#pragma once

#define WINDOWS_HIDE_CONSOLE false

#include <string>
#include <iostream>
#include "json/json.h"

namespace help {

	template <typename T>
	inline void CopyToArrayChar(T& arrayChar, const std::string& text) {
		strncpy(arrayChar.data(), text.data(), arrayChar.size());
	};

	//...
	unsigned int GetNextGuiId();

	template <size_t T>
	struct InputContainer {
		typedef std::shared_ptr<InputContainer> Ptr;

		InputContainer() : _guiId("##" + std::to_string(GetNextGuiId())) { }

		const char* GetId() {
			return _guiId.c_str();
		}
		void Clear() {
			textBuffer[0] = '\0';
			_guiId = "##" + std::to_string(GetNextGuiId());
		}
		void Set(const std::string& text) {
			_guiId = "##" + std::to_string(GetNextGuiId());
			help::CopyToArrayChar(textBuffer, text);
		}

		std::array<char, T> textBuffer;

	private:
		std::string _guiId;
	};

	//...
	bool LoadJson(const std::string& fileName, Json::Value& value);
	bool SaveJson(const std::string& fileName, const Json::Value& value, const std::string& indentation = "\t");
	std::string StringFromJson(const Json::Value& value, const std::string& indentation = "\t");

	std::wstring StrToWstr(const std::string& str);
	std::string WstrToStr(const std::wstring& wstr);
};
