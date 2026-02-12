#pragma once

#include <ncurses.h>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace tui {
	class Window {
	public:
		Window(const std::string& winTitle,
		       int heightPercent,
		       int widthPercent);
		Window(const std::string& winTitle,
		       int heightPercent,
		       int widthPercent,
		       Window& parentWindow);
		virtual ~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		void set_title(const std::string& winTitle);
		void mark_need_repaint();
		bool needs_repaint() const;
		void repaint();

		std::shared_ptr<WINDOW> outer_window() const;
		std::shared_ptr<WINDOW> inner_window() const;

		void add_child(Window& child);
		void remove_child(Window& child);

	protected:
		virtual void draw_content();
		std::optional<std::reference_wrapper<Window>> parent_window() const;

	private:
		struct WindowDeleter {
			void operator()(WINDOW* window) const;
		};

		std::string title;
		std::unique_ptr<WINDOW, WindowDeleter> outer;
		std::unique_ptr<WINDOW, WindowDeleter> inner;
		std::optional<std::reference_wrapper<Window>> parent;
		std::vector<std::reference_wrapper<Window>> children;
		bool need_repaint;

		void create_windows(int heightPercent, int widthPercent);
		void draw_frame();
		void repaint_internal(bool force);
	};
}
