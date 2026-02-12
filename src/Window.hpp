#ifndef TUI_WINDOW_HPP
#define TUI_WINDOW_HPP

#include <ncurses.h>

#include <string>
#include <vector>

namespace tui {
	class Window {
	public:
		Window(const std::string& winTitle,
		       int heightPercent,
		       int widthPercent,
		       Window* parentWindow = nullptr);
		virtual ~Window();

		void set_title(const std::string& winTitle);
		void mark_need_repaint();
		bool needs_repaint() const;
		void repaint();

		WINDOW* outer_window() const;
		WINDOW* inner_window() const;

		void add_child(Window* child);
		void remove_child(Window* child);

	private:
		std::string title;
		WINDOW* outer;
		WINDOW* inner;
		Window* parent;
		std::vector<Window*> children;
		bool need_repaint;

		void create_windows(int heightPercent, int widthPercent);
		void draw_frame();
		void repaint_internal(bool force);
	};
}

#endif
