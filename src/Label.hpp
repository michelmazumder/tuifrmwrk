#ifndef TUI_LABEL_HPP
#define TUI_LABEL_HPP

#include "Window.hpp"

#include <string>

namespace tui {
	class Label : public Window {
	public:
		Label(const std::string& labelText,
		      int heightPercent,
		      int widthPercent,
		      Window* parentWindow = nullptr);
		virtual ~Label();

		const std::string& get_text() const;
		void set_text(const std::string& labelText);

	protected:
		void draw_content() override;

	private:
		std::string text;

		void center_on_parent();
	};
}

#endif
