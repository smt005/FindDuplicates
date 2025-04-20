// ◦ Xyz ◦
#pragma once

#include <array>
#include <string>
#include <functional>

#include "../Help/Help.h"

namespace Window {
	class Input final {
		typedef std::function<bool(const char*)> Callback;

	public:
		Input(float width, Callback callback)
			: _width(width)
			, _callback(callback)
		{
			_textBuffer[0] = '\0';
		}
		Input() {
			_textBuffer[0] = '\0';
		};

		~Input() = default;

		Input(const Input&) = delete;
		Input(Input&&) = delete;
		void operator = (const Input&) = delete;
		void operator = (Input&&) = delete;

		void SetText(const std::string& text) {
			help::CopyToArrayChar(_textBuffer, text.data());

		}
		void SetWidth(float width) {
			_width = width;
		}
		void SetCallback(Callback callback) {
			_callback = callback;
		}

		std::string GetText() {
			return _textBuffer.data();
		}

		bool Error() {
			return _error;
		}

		bool Display();

	private:
		std::array<char, 256> _textBuffer;
		Callback _callback = nullptr;
		float _width = 200.f;
		bool _error = false;
		unsigned int _guiIndex = ++guiIndex;

	private:
		static unsigned int guiIndex;
	};

}
