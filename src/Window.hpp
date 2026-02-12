#ifndef TUI_WINDOW_HPP
#define TUI_WINDOW_HPP

#include <ncurses.h>

#include <memory>
#include <string>
#include <vector>

namespace tui {
	class Window {
	public:
		Window(const std::string& winTitle,
		       int heightPercent,
		       int widthPercent,
		       std::shared_ptr<Window> parentWindow = nullptr);
		virtual ~Window();

		void set_title(const std::string& winTitle);
		void mark_need_repaint();
		bool needs_repaint() const;
		void repaint();

		std::shared_ptr<WINDOW> outer_window() const;
		std::shared_ptr<WINDOW> inner_window() const;

		void add_child(const std::shared_ptr<Window>& child);
		void remove_child(const std::shared_ptr<Window>& child);

	protected:
		virtual void draw_content();
		std::shared_ptr<Window> parent_window() const;

	private:
		struct WindowDeleter {
			void operator()(WINDOW* window) const;
		};

		std::string title;
		std::unique_ptr<WINDOW, WindowDeleter> outer;
		std::unique_ptr<WINDOW, WindowDeleter> inner;
		std::weak_ptr<Window> parent;
		std::vector<std::weak_ptr<Window>> children;
		bool need_repaint;

		void create_windows(int heightPercent, int widthPercent);
		void draw_frame();
		void repaint_internal(bool force);
	};
}

#endif
