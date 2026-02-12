#include <iostream>
#include <ncurses.h>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#include <memory>
#include <csignal>
#include "App.hpp"

namespace tui {

App::App(const char *appName): App(std::string(appName))
{}

App::App(const std::string& appName): 
	app_name(appName),
	running(true),
	main_window(nullptr)
{
	// Inizializzazione ncurses
	initscr();
	raw();                  // Disattiva il buffering della riga
	keypad(stdscr, TRUE);   // Abilita i tasti speciali (F1, frecce)
	noecho();               // Non stampare i tasti premuti
	curs_set(0);            // Nascondi il cursore
	timeout(100);           // Getch non bloccante (100ms)

	main_window = std::make_shared<Window>(app_name, 100, 100, nullptr);

	// Avvio del thread per i task in background
	worker_thread = std::thread(&App::worker_loop, this);
}

App::~App() {
	stop_worker();
	main_window.reset();
	endwin(); // Chiude ncurses
}

void App::on_drastic_close() {
	running = false;
	// Logica per Ctrl+C (es. kill immediato)
}

void App::on_controlled_close() {
	running = false;
	// Logica per Ctrl+Q (es. salvataggio dati)
}

// Aggiunge un task alla coda
void App::push_task(std::function<void()> task) {
	{
		std::lock_guard<std::mutex> lock(queue_mutex);
		tasks.push(task);
	}
	condition.notify_one();
}

void App::run() {
	while (running) {
		int ch = getch();
		handle_input(ch);
		update_ui();
	}
}

void App::update_ui() {
	if (!main_window) return;

	main_window->repaint();
}

bool App::handle_input(int ch) {
	if (ch == 3) { // Ctrl+C (in modalità raw)
		on_drastic_close();
		return true;
	} else if (ch == 17) { // Ctrl+Q
		on_controlled_close();
		return true;
	}
	return false;
}

void App::worker_loop() {
	while (running) {
		std::function<void()> task;
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			condition.wait_for(lock, std::chrono::milliseconds(100), [this] {
					return !tasks.empty() || !running;
			});

			if (!running && tasks.empty()) return;
			if (tasks.empty()) continue;

			task = std::move(tasks.front());
			tasks.pop();
		}
		task(); // Esecuzione della lambda
	}
}

void App::stop_worker() {
	running = false;
	condition.notify_all();
	if (worker_thread.joinable()) worker_thread.join();
}

}
