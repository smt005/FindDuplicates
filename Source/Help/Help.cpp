﻿// ◦ Xyz ◦

#include "Help.h"
#include <codecvt>

#include "FileManager.h"
#include "../Help/Log.h"

using namespace help;

unsigned int help::GetNextGuiId() {
	static unsigned int guiId = 0;
	++guiId;
	return guiId;
}

//...
bool help::LoadJson(const std::string& fileName, Json::Value& value) {
	std::string stringData = FileManager::ReadTextFile(fileName);

	Json::CharReaderBuilder readerBuilder;
	Json::CharReader *reader = readerBuilder.newCharReader();
	std::string err;
	if (reader->parse(stringData.c_str(), stringData.c_str() + stringData.length(), &value, &err)) {
		return true;
	}

	LOG("Fail parse: {}", fileName);
	return false;
}

bool help::SaveJson(const std::string& fileName, const Json::Value& value, const std::string& indentation) {
	const std::string stringData = StringFromJson(value, indentation);
	return FileManager::WriteTextFile(fileName, stringData);
}

std::string help::StringFromJson(const Json::Value& value, const std::string& indentation) {
	Json::StreamWriterBuilder writerBuilder;
	writerBuilder["indentation"] = indentation;
	return Json::writeString(writerBuilder, value);
}

std::wstring help::StrToWstr(const std::string& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.from_bytes(str);
}

std::string help::WstrToStr(const std::wstring& wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.to_bytes(wstr);
}
