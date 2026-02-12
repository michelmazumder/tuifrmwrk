#include "App.hpp"

class MyApp : public tui::App {
public:
    MyApp(const char *appName): tui::App(appName)
    {}
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
