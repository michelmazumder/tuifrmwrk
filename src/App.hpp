#include <string>
#include <functional>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>

#include "Window.hpp"


namespace tui {
	class App {
		private:
			std::string app_name;
			// Gestione Concorrenza
			std::thread worker_thread;
    		std::queue<std::function<void()>> tasks;
    		std::mutex queue_mutex;
    		std::condition_variable condition;

			void worker_loop();
			void stop_worker();



		protected:
			std::atomic<bool> running;
			std::shared_ptr<Window> main_window;
			virtual void update_ui();
			virtual bool handle_input(int character);

		public:
			App(const std::string& appName);
			App(const char* appName);
			virtual ~App();

			virtual void on_drastic_close();
			virtual void on_controlled_close();
			void push_task(std::function<void()> task);

			void run();
	};
}
