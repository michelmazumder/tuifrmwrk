#include "Label.hpp"

#include <algorithm>

namespace tui {

Label::Label(const std::string& labelText,
             int heightPercent,
             int widthPercent,
             Window* parentWindow)
	: Window("", heightPercent, widthPercent, parentWindow),
	  text(labelText) {
	center_on_parent();
	mark_need_repaint();
}

Label::~Label() {
}

const std::string& Label::get_text() const {
	return text;
}

void Label::set_text(const std::string& labelText) {
	text = labelText;
	mark_need_repaint();
}

void Label::draw_content() {
	WINDOW* inner = inner_window();
	if (!inner) return;

	int max_y = 0;
	int max_x = 0;
	getmaxyx(inner, max_y, max_x);

	if (max_y <= 0 || max_x <= 0) return;

	const int text_len = static_cast<int>(text.size());
	const int max_len = std::max(0, max_x);
	if (max_len == 0) return;

	int y = max_y / 2;
	int x = 0;
	if (text_len < max_x) {
		x = (max_x - text_len) / 2;
	}

	mvwaddnstr(inner, y, x, text.c_str(), max_len);
}

void Label::center_on_parent() {
	WINDOW* outer = outer_window();
	WINDOW* inner = inner_window();
	if (!outer) return;

	WINDOW* parent_inner = nullptr;
	Window* parent = parent_window();
	if (parent) {
		parent_inner = parent->inner_window();
	}

	int parent_h = 0;
	int parent_w = 0;
	int parent_y = 0;
	int parent_x = 0;

	if (parent_inner) {
		getmaxyx(parent_inner, parent_h, parent_w);
		getbegyx(parent_inner, parent_y, parent_x);
	} else {
		getmaxyx(stdscr, parent_h, parent_w);
		getbegyx(stdscr, parent_y, parent_x);
	}

	int out_h = 0;
	int out_w = 0;
	getmaxyx(outer, out_h, out_w);

	int new_y = parent_y + (parent_h - out_h) / 2;
	int new_x = parent_x + (parent_w - out_w) / 2;
	if (new_y < parent_y) new_y = parent_y;
	if (new_x < parent_x) new_x = parent_x;

	mvwin(outer, new_y, new_x);
	if (inner) {
		mvwin(inner, new_y + 1, new_x + 1);
	}
}

}
