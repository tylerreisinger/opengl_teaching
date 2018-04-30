#include "Simulation.h"

#include "BufferObject.h"
#include "VertexArrayObject.h"

#include "GameClock.h"
#include "GameTime.h"

#include "Window.h"

#include "GlHeaders.h"
#include <glm/glm.hpp>


#include <thread>
#include <iostream>

struct Vertex {
    float x;
    float y;
    float z;
    float r;
    float g;
    float b;
    float a;
};

std::string severity_to_string(GLenum severity) {
    switch(severity) {
        case GL_DEBUG_SEVERITY_HIGH: return "High";
        case GL_DEBUG_SEVERITY_MEDIUM: return "Medium";
        case GL_DEBUG_SEVERITY_LOW: return "Low";
        case GL_DEBUG_SEVERITY_NOTIFICATION: return "Notification";
        default: return "???";
    }
}

void debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, 
        GLsizei length, const GLchar* message, const void* userParam) 
{
    std::cout << "Source: " << source << " -- Type " << type << " -- ID " << id 
        << " Severity: " << severity_to_string(severity) << " Msg: " << message << std::endl;
}



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
    m_window = std::make_unique<GlWindow>(800, 600, "Starlight Glimmer");

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        throw std::runtime_error("GLEW failed to initialize!");
    }

    Atom wm_delete_msg = XInternAtom(m_window->display(), "WM_DELETE_WINDOW", False);
    XSetWMProtocols(m_window->display(), m_window->window(), &wm_delete_msg, 1);

    m_wm_delete_msg = wm_delete_msg;

    glDebugMessageCallback(debug_callback, nullptr);
 
    glClearColor(0.0, 0.0, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glXSwapBuffers(m_window->display(), m_window->window());

    auto vertex_shader = std::make_shared<Shader>(ShaderKind::Vertex);
    vertex_shader->set_shader_source(Shader::read_glsl_source("glsl/vert.glsl"));
    vertex_shader->compile();
    auto fragment_shader = std::make_shared<Shader>(ShaderKind::Fragment);
    fragment_shader->set_shader_source(Shader::read_glsl_source("glsl/frag.glsl"));
    fragment_shader->compile();

    m_shader = std::make_unique<ShaderProgram>();
    m_shader->attach_shader(std::move(vertex_shader));
    m_shader->attach_shader(std::move(fragment_shader));
    m_shader->link();

    m_shader->use();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    std::array<Vertex, 3> vertices{{
        Vertex{0.0, 1.0, -1.0, 1.0, 0.0, 0.0, 1.0},
        Vertex{1.0, 1.0, -1.0, 0.0, 1.0, 0.0, 1.0},
        Vertex{1.0, 0.0, -1.0, 0.0, 0.0, 1.0, 1.0},
    }};

    auto buffer = std::make_shared<BufferObject>(BufferKind::VertexBuffer);
    buffer->bind();
    buffer->set_data(vertices, BufferUsage::StaticDraw);

    m_vao = std::make_unique<VertexArrayObject>();
    m_vao->bind();
    m_vao->attach_vertex_buffer(buffer, 0, 3, GL_FLOAT, sizeof(Vertex), 0);
    m_vao->attach_vertex_buffer(buffer, 1, 4, GL_FLOAT, sizeof(Vertex), 3*sizeof(float));
    VertexArrayObject::unbind();
}
 
void Simulation::update() {
 
}
 
void Simulation::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_vao->bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    m_vao->unbind();

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
 
