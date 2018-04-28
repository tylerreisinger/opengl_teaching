#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <memory>

#include "GameClock.h"

#include <X11/Xlib.h>

class GlWindow;

class Simulation {
public:
    Simulation();
    ~Simulation();

    Simulation(const Simulation& other) = delete;
    Simulation(Simulation&& other) noexcept = delete;
    Simulation& operator =(const Simulation& other) = delete;
    Simulation& operator =(Simulation&& other) noexcept = delete;

    void run();

private:    
    void initialize();

    void update();
    void draw();
    void event_loop();

    std::unique_ptr<GlWindow> m_window;
    GameClock m_game_clock;

    Atom m_wm_delete_msg;

    bool m_is_running = true;

};

#endif
