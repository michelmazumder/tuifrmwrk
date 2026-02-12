#pragma once

#include "Window.hpp"

#include <memory>
#include <string>

namespace tui {
	class Label : public Window {
	public:
		Label(const std::string& labelText,
		      int heightPercent,
		      int widthPercent);
		Label(const std::string& labelText,
		      int heightPercent,
		      int widthPercent,
		      Window& parentWindow);
		virtual ~Label();

		Label(const Label&) = delete;
		Label& operator=(const Label&) = delete;

		const std::string& get_text() const;
		void set_text(const std::string& labelText);

	protected:
		void draw_content() override;

	private:
		std::string text;

		void center_on_parent();
	};
}
