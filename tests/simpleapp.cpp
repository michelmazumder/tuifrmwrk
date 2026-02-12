#include "App.hpp"
#include "Label.hpp"

class MyApp : public tui::App {
public:
    MyApp(const char *appName): tui::App(appName), label(nullptr)
    {
        label = new tui::Label("Ciao dal framework", 30, 60, main_window);
    }

    ~MyApp() override {
        delete label;
        label = nullptr;
    }

private:
    tui::Label* label;
};

int main() {
    MyApp my_app("App di prova");
    
    // Esempio di task in background aggiunto all'avvio
    my_app.push_task([]() {
        // Operazione pesante...
    });

    my_app.run();
    return 0;
}
