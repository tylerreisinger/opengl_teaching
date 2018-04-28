#include "Simulation.h"

#include "GameClock.h"
#include "GameTime.h"

#include "Window.h"

#include "GlHeaders.h"
#include <glm/glm.hpp>


#include <thread>
#include <iostream>


Simulation::Simulation() {
 
}
 
Simulation::~Simulation() {
 
}
 
void Simulation::run() {
    using namespace std::chrono_literals;
    initialize();

    auto time_step = 33.3ms;

    while(m_is_running) {
        GameTime frame_time = m_game_clock.tick(time_step);

        event_loop();

        update();
        draw();
        
        auto frame_length = std::chrono::high_resolution_clock::now()
            - frame_time.get_frame_start_time();
        auto sleep_duration = time_step - frame_length;

        if(sleep_duration > decltype(frame_length)::zero()) {
            std::this_thread::sleep_for(sleep_duration);
        }
    }
}
 
void Simulation::initialize() {
    m_window = std::make_unique<GlWindow>(799, 600, "Starlight Glimmer");

    GLenum err = glewInit();
    if(err != GLEW_OK) {
        throw std::runtime_error("GLEW failed to initialize!");
    }

    Atom wm_delete_msg = XInternAtom(m_window->display(), "WM_DELETE_WINDOW", False);
    XSetWMProtocols(m_window->display(), m_window->window(), &wm_delete_msg, 1);

    m_wm_delete_msg = wm_delete_msg;
 
    glClearColor(0.0, 0.0, 0.5, 1.0);
}
 
void Simulation::update() {
 
}
 
void Simulation::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glXSwapBuffers(m_window->display(), m_window->window());
}
 
void Simulation::event_loop() {
    auto fd = ConnectionNumber(m_window->display());
    fd_set descriptor_set;
    FD_ZERO(&descriptor_set);
    FD_SET(fd, &descriptor_set);

    timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 100;


    int num_ready = select(fd+1, &descriptor_set, nullptr, nullptr, &timeout);

    while(num_ready > 0 && XPending(m_window->display()) > 0) {
        XEvent evt;
        XNextEvent(m_window->display(), &evt);

        switch(evt.type) {
            case ClientMessage: {
                if(evt.xclient.data.l[0] == m_wm_delete_msg) {
                    m_is_running = false;
                }
                break;
            }
        }
    }
}
 
