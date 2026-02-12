#include "App.hpp"
#include "Label.hpp"

#include <memory>
#include <thread>
#include <chrono>

class MyApp : public tui::App {
public:
    MyApp(const char *appName): tui::App(appName), label(nullptr)
    {
        label = std::make_shared<tui::Label>("10", 30, 60, main_window);
        main_window->add_child(label);
    }

    ~MyApp() override = default;

    std::shared_ptr<tui::Label> get_label() { return label; }

private:
    std::shared_ptr<tui::Label> label;
};

int main() {
    MyApp my_app("App di prova");
    
    // Timer countdown di 10 secondi
    my_app.push_task([&my_app]() {
        for (int i = 10; i >= 0; --i) {
            my_app.get_label()->set_text(std::to_string(i));
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });

    my_app.run();
    return 0;
}
