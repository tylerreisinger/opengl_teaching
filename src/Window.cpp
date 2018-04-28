#include "Window.h"

#include <iostream>

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

void GlWindow::open(int width, int height, const std::string& title)
{
    Display* display = XOpenDisplay(nullptr);
    
    static int fb_attribs[] = {
        GLX_X_RENDERABLE    , True,
        GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
        GLX_RENDER_TYPE     , GLX_RGBA_BIT,
        GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
        GLX_RED_SIZE        , 8,
        GLX_GREEN_SIZE      , 8,
        GLX_BLUE_SIZE       , 8,
        GLX_ALPHA_SIZE      , 8,
        GLX_DEPTH_SIZE      , 24,
        GLX_STENCIL_SIZE    , 8,
        GLX_DOUBLEBUFFER    , True,
        None
    };

    int glx_major, glx_minor;

    if(!glXQueryVersion(display, &glx_major, &glx_minor)) {
        throw std::runtime_error("GLX query failed");
    }

    if(glx_major >= 1 && glx_minor < 3) {
        throw std::runtime_error("The GLX version is too old!");
    }

    int fb_count = 0;
    GLXFBConfig* fbc = glXChooseFBConfig(display, DefaultScreen(display), fb_attribs,
        &fb_count);

    if(!fbc) {
        throw std::runtime_error("Could not load framebuffer config.");
    }

    std::cout << "Found " << fb_count << " possible configs." << std::endl;

    int choice = 0;
    int max_samples = 0;
    for (int i = 0; i < fb_count; ++i)
    {
        XVisualInfo* vi = glXGetVisualFromFBConfig( display, fbc[i] );

        if(vi) {
            int sample_buffers = 0;
            int samples = 0;
            glXGetFBConfigAttrib( display, fbc[i], GLX_SAMPLE_BUFFERS, &sample_buffers );
            glXGetFBConfigAttrib( display, fbc[i], GLX_SAMPLES       , &samples  );
            std::cout << "Sample Buffers: " << sample_buffers << "; Samples: " << samples << std::endl;

            if(samples > max_samples) {
                max_samples = samples;
                choice = i;
            }
        }

        XFree(vi);
    }

    std::cout << "Choosing format " << choice << std::endl;

    GLXFBConfig chosen_fbc = fbc[2];
    XFree(fbc);

    XVisualInfo* vi = glXGetVisualFromFBConfig(display, chosen_fbc);

    Colormap cmap;
    cmap = XCreateColormap(display,
            RootWindow( display, vi->screen ), 
            vi->visual, AllocNone);

    m_color_map = cmap;

    XSetWindowAttributes window_attribs;

    window_attribs.colormap = cmap;
    window_attribs.background_pixmap = None;
    window_attribs.border_pixel = 0;
    window_attribs.event_mask = StructureNotifyMask;

    Window win = XCreateWindow(display, RootWindow(display, vi->screen), 
        500, 500, width, height, 0, vi->depth, InputOutput, vi->visual,
        CWBorderPixel | CWColormap | CWEventMask, &window_attribs);

    XSelectInput(display, win, 
            ExposureMask | KeyPressMask | KeyReleaseMask | PointerMotionMask |
            ButtonPressMask | ButtonReleaseMask  | StructureNotifyMask 
            );

    XFree(vi);
    XStoreName(display, win, title.c_str());
    XMapWindow(display, win);

    if(!win) {
        throw std::runtime_error("Failed to create XOrg window!");
    }

    m_window_handle = win;

    const char *glxExts = glXQueryExtensionsString(display, DefaultScreen(display));
    std::cout << "Available Extensions: " << glxExts << std::endl;

    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = nullptr;

    glXCreateContextAttribsARB = 
        (glXCreateContextAttribsARBProc) glXGetProcAddressARB((const GLubyte*) "glXCreateContextAttribsARB");

    if(!glXCreateContextAttribsARB) {
        throw std::runtime_error("glXCreateContextAttribsARB is not supported!");
    }

    int gl_context_attribs[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
        GLX_CONTEXT_MINOR_VERSION_ARB, 3,
        GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
        None
    };

    GLXContext gl_ctx = glXCreateContextAttribsARB(display, chosen_fbc, 0, True, gl_context_attribs);

    if(!gl_ctx) {
        throw std::runtime_error("Failed to construct OpenGL context");
    }

    XSync(display, False);
    glXMakeCurrent(display, win, gl_ctx);

    m_display = display;
    m_gl_ctx = gl_ctx;
}
 
GlWindow::~GlWindow() {
    glXDestroyContext(m_display, m_gl_ctx);
    XDestroyWindow(m_display, m_window_handle);
    XFreeColormap(m_display, m_color_map);
    XCloseDisplay(m_display);
}
 
GlWindow::GlWindow(int width, int height, const std::string& title) {
    open(width, height, title);
}
 
