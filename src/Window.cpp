#include "Window.hpp"

#include <algorithm>

namespace tui {

namespace {
int clamp_percent(int value) {
	if (value < 1) return 1;
	if (value > 100) return 100;
	return value;
}

int clamp_min(int value, int min_value) {
	return value < min_value ? min_value : value;
}
}

Window::Window(const std::string& winTitle,
               int heightPercent,
               int widthPercent,
               Window* parentWindow)
	: title(winTitle),
	  outer(nullptr),
	  inner(nullptr),
	  parent(parentWindow),
	  need_repaint(true) {
	create_windows(heightPercent, widthPercent);
	if (parent) {
		parent->add_child(this);
	}
}

Window::~Window() {
	if (inner) {
		delwin(inner);
		inner = nullptr;
	}
	if (outer) {
		delwin(outer);
		outer = nullptr;
	}
}

void Window::set_title(const std::string& winTitle) {
	title = winTitle;
	mark_need_repaint();
}

void Window::mark_need_repaint() {
	need_repaint = true;
}

bool Window::needs_repaint() const {
	return need_repaint;
}

WINDOW* Window::outer_window() const {
	return outer;
}

WINDOW* Window::inner_window() const {
	return inner;
}

void Window::add_child(Window* child) {
	if (!child) return;
	if (std::find(children.begin(), children.end(), child) == children.end()) {
		children.push_back(child);
	}
}

void Window::remove_child(Window* child) {
	auto it = std::remove(children.begin(), children.end(), child);
	children.erase(it, children.end());
}

void Window::repaint() {
	repaint_internal(false);
}

void Window::create_windows(int heightPercent, int widthPercent) {
	const int h_pct = clamp_percent(heightPercent);
	const int w_pct = clamp_percent(widthPercent);

	int base_h = 0;
	int base_w = 0;
	int base_y = 0;
	int base_x = 0;

	if (parent && parent->inner_window()) {
		getmaxyx(parent->inner_window(), base_h, base_w);
		getbegyx(parent->inner_window(), base_y, base_x);
	} else {
		getmaxyx(stdscr, base_h, base_w);
		getbegyx(stdscr, base_y, base_x);
	}

	int out_h = (base_h * h_pct) / 100;
	int out_w = (base_w * w_pct) / 100;
	out_h = clamp_min(out_h, 3);
	out_w = clamp_min(out_w, 3);

	outer = newwin(out_h, out_w, base_y, base_x);
	inner = newwin(out_h - 2, out_w - 2, base_y + 1, base_x + 1);
}

void Window::draw_frame() {
	if (!outer) return;

	werase(outer);
	box(outer, 0, 0);

	if (!title.empty()) {
		int max_y = 0;
		int max_x = 0;
		getmaxyx(outer, max_y, max_x);
		int max_len = max_x > 2 ? max_x - 2 : 0;
		if (max_len > 0) {
			mvwaddnstr(outer, 0, 1, title.c_str(), max_len);
		}
	}

	wrefresh(outer);

	if (inner) {
		werase(inner);
		wrefresh(inner);
	}
}

void Window::repaint_internal(bool force) {
	const bool should_draw = force || need_repaint;
	if (should_draw) {
		draw_frame();
		need_repaint = false;
	}

	for (Window* child : children) {
		if (child) {
			child->repaint_internal(should_draw);
		}
	}
}

}
