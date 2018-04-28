#ifndef WINDOW_H_
#define WINDOW_H_

#include <string>

#include <unistd.h>

#include "GlHeaders.h"
#include <GL/glx.h> 
#include <X11/X.h>
#include <X11/Xlib.h>


class GlWindow {
public:
    GlWindow(int width, int height, const std::string& title);
    ~GlWindow();

    GlWindow(const GlWindow& other) = delete;
    GlWindow(GlWindow&& other) noexcept = delete;
    GlWindow& operator =(const GlWindow& other) = delete;
    GlWindow& operator =(GlWindow&& other) noexcept = delete;

    Display* display() { return m_display; }
    GLXContext gl_context() { return m_gl_ctx; }
    Window window() { return m_window_handle; }


private:
    void open(int width, int height, const std::string& title);

    Window m_window_handle = 0;
    GLXContext m_gl_ctx = 0;
    Display* m_display = nullptr;
    Colormap m_color_map = 0;
};

#endif
