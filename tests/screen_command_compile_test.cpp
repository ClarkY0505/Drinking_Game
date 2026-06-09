#include <memory>

#include "engine/screen.h"

class TestScreen : public Screen {
   public:
    void on_enter() override {}
    void on_exit() override {}
    ScreenCommand on_event(const SDL_Event&) override {
        return ScreenCommand::none();
    }
    void on_update(float) override {}
    void on_render(SDL_Renderer*) override {}
    bool init(SDL_Renderer*) override {
        return true;
    }
};

int main() {
    ScreenCommand command = ScreenCommand::quit();
    if (command.type != ScreenCommandType::Quit) {
        return 1;
    }

    TestScreen screen;
    SDL_Event event{};
    command = screen.on_event(event);
    return command.type == ScreenCommandType::None ? 0 : 1;
}
