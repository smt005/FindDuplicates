// ◦ Xyz ◦
#pragma once

#include <any>
#include <memory>
#include <filesystem>
#include <map>

#include  "../Help/Help.h"
#include "../Editor/Input.h"

namespace Window {
	class FindDuplicate final {
	public:
		struct InfoFiles {
			InfoFiles() = default;
			InfoFiles(const std::string& argName, const std::string& argFileNameFirst, const std::string& argFileNameSecond)
				: name(argName)
				, fileNameFirst(argFileNameFirst)
				, fileNameSecond(argFileNameSecond)
			{}

			std::string name;
			std::string fileNameFirst;
			std::string fileNameSecond;
		};

	// FindDuplicate

	public:
		FindDuplicate(const std::filesystem::path& resourcePathName, const std::string& resourceFileName);
		FindDuplicate() = delete;
		FindDuplicate(const FindDuplicate&) = delete;
		FindDuplicate(FindDuplicate&&) = delete;
		void operator = (const FindDuplicate&) = delete;
		void operator = (FindDuplicate&&) = delete;

	public:
		void InitDisplay(float width, float height);
		void Render();
		void Resize(float width, float height);

		void Load();
		void Save();

	private:
		inline std::shared_ptr<bool>& GetWndPtr() { return _flagWndPtr ? _flagWndPtr : _flagWndPtr = std::make_shared<bool>(true); }
		inline void ResetPtr() { _flagWndPtr.reset(); }

		void InitInterface();
		void FinDublicate(const std::string& firstPath, const std::string& secondPath);
		void FinDublicate(std::map<std::string, std::string>& firstFiles, std::map<std::string, std::string>& secondFiles);
		void FindFiles(const std::filesystem::path& dir, std::map<std::string, std::string>& files);

		void ListDisplay();
		void InfoDisplay();

	private:
		const char* _windowId;
		const int _windowFlags;
		std::shared_ptr<bool> _flagWndPtr;
		float _widthWindow;
		float _heightWindow;

		Input _inputFirst;
		Input _inputSecond;

		std::vector<std::unique_ptr<InfoFiles>> _dublicates;
		std::vector<std::unique_ptr<InfoFiles>>::iterator _itSelectDublicate = _dublicates.end();

		std::string _resourceFileName;
	};
}
