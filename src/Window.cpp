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
               int widthPercent)
	: title(winTitle),
	  parent(std::nullopt),
	  need_repaint(true) {
	create_windows(heightPercent, widthPercent);
}

Window::Window(const std::string& winTitle,
               int heightPercent,
               int widthPercent,
               Window& parentWindow)
	: title(winTitle),
	  parent(parentWindow),
	  need_repaint(true) {
	create_windows(heightPercent, widthPercent);
}

Window::~Window() {
}

void Window::WindowDeleter::operator()(WINDOW* window) const {
	if (window) {
		delwin(window);
	}
}

void Window::set_title(const std::string& winTitle) {
	title = winTitle;
	mark_need_repaint();
}

void Window::set_position(int y, int x) {
	auto outer_win = outer_window();
	auto inner_win = inner_window();
	if (outer_win) {
		mvwin(outer_win.get(), y, x);
	}
	if (inner_win) {
		mvwin(inner_win.get(), y + 1, x + 1);
	}
	mark_need_repaint();
}

void Window::mark_need_repaint() {
	need_repaint = true;
}

bool Window::needs_repaint() const {
	return need_repaint;
}

std::shared_ptr<WINDOW> Window::outer_window() const {
	return std::shared_ptr<WINDOW>(outer.get(), [](WINDOW*) {});
}

std::shared_ptr<WINDOW> Window::inner_window() const {
	return std::shared_ptr<WINDOW>(inner.get(), [](WINDOW*) {});
}

void Window::add_child(Window& child) {
	for (const auto& existing : children) {
		if (&existing.get() == &child) {
			return;
		}
	}
	children.emplace_back(child);
}

void Window::remove_child(Window& child) {
	children.erase(
		std::remove_if(children.begin(), children.end(),
			[&child](const std::reference_wrapper<Window>& item) {
				return &item.get() == &child;
			}),
		children.end());
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

	auto parent_inner = parent ? parent->get().inner_window() : nullptr;
	if (parent_inner) {
		getmaxyx(parent_inner.get(), base_h, base_w);
		getbegyx(parent_inner.get(), base_y, base_x);
	} else {
		getmaxyx(stdscr, base_h, base_w);
		getbegyx(stdscr, base_y, base_x);
	}

	int out_h = (base_h * h_pct) / 100;
	int out_w = (base_w * w_pct) / 100;
	out_h = clamp_min(out_h, 3);
	out_w = clamp_min(out_w, 3);

	outer.reset(newwin(out_h, out_w, base_y, base_x));
	inner.reset(newwin(out_h - 2, out_w - 2, base_y + 1, base_x + 1));
}

void Window::draw_frame() {
	auto outer_win = outer_window();
	auto inner_win = inner_window();
	if (!outer_win) return;

	werase(outer_win.get());
	box(outer_win.get(), 0, 0);

	if (!title.empty()) {
		int max_y = 0;
		int max_x = 0;
		getmaxyx(outer_win.get(), max_y, max_x);
		(void)max_y;
		int max_len = max_x > 2 ? max_x - 2 : 0;
		if (max_len > 0) {
			mvwaddnstr(outer_win.get(), 0, 1, title.c_str(), max_len);
		}
	}

	wrefresh(outer_win.get());

	if (inner_win) {
		werase(inner_win.get());
		draw_content();
		wrefresh(inner_win.get());
	}
}

void Window::repaint_internal(bool force) {
	const bool should_draw = force || need_repaint;
	if (should_draw) {
		draw_frame();
		need_repaint = false;
	}

	for (auto& child : children) {
		child.get().repaint_internal(should_draw);
	}
}

void Window::draw_content() {
}

std::optional<std::reference_wrapper<Window>> Window::parent_window() const {
	return parent;
}

}
