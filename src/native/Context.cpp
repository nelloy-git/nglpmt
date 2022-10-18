#include "native/Context.hpp"

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

using namespace nglpmt::native;

std::atomic<unsigned int> Context::_glfw_windows = 0;

std::shared_ptr<Context> Context::make(const Parameters& params){
    return std::shared_ptr<Context>(new Context(params));
}

Context::Context(const Parameters& params) :
    SharedObject(),
    _gl_thread(new BS::thread_pool(1)),
    _init_time(std::chrono::steady_clock::now()),
    _last_start_time(std::chrono::steady_clock::now()),
    _last_finish_time(std::chrono::steady_clock::now()),

    onStart(decltype(onStart)::element_type::make(_gl_thread)),
    onRun(decltype(onRun)::element_type::make(_gl_thread)),
    onFinish(decltype(onFinish)::element_type::make(_gl_thread)),
    onKey(decltype(onKey)::element_type::make(_gl_thread)){

    _gl_thread->submit([this, &params](){
        std::cout << "GL: " << std::this_thread::get_id() << std::endl;
        try {
            _initGl(params);
        } catch (const std::exception& err){
            std::cout << err.what() << std::endl;
        }
        
    }).wait();

    onStart->addActionQueued([window = _window](){
        glfwPollEvents();
        glfwSwapBuffers(window.get());
        return true;
    });
    
}

Context::~Context(){
}

std::future<void> Context::run(){
    auto now = std::chrono::steady_clock::now();
    auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(now - _last_start_time);
    _last_start_time = now;

    onStart->emitQueued(this->shared_from_this(), dt);
    onRun->emitQueued(this->shared_from_this(), dt);
    return onFinish->emitQueued(this->shared_from_this(), dt);
}

const std::thread::id& Context::getThreadId() const {
    return _gl_thread_id;
}

void Context::_initGl(const Parameters& params){
    _gl_thread_id = std::this_thread::get_id();

    if (_glfw_windows == 0 && !glfwInit()){
        throw std::runtime_error("Failed to initialize GLFW.");
    }
    ++_glfw_windows;

    std::vector<std::pair<int, int>> hints = {
        // {GLFW_VERSION_MAJOR, 4},
        // {GLFW_VERSION_MINOR, 6},
        {GLFW_CONTEXT_VERSION_MAJOR, 4},
        {GLFW_CONTEXT_VERSION_MINOR, 6},
        {GLFW_REFRESH_RATE, params.fps},
    };

    for (auto& hint : hints){
        glfwWindowHint(hint.first, hint.second);
    }

    _window = std::shared_ptr<GLFWwindow>(
        glfwCreateWindow(params.width, params.height, params.title.c_str(), NULL, NULL),
        [](GLFWwindow* window){
            glfwDestroyWindow(window);
            --_glfw_windows;
            if (_glfw_windows == 0){
                glfwTerminate();
            }
        }
    );
    glfwSetWindowUserPointer(_window.get(), this);
    glfwMakeContextCurrent(_window.get());
    
    auto ver = gladLoadGL(glfwGetProcAddress);
    if (ver == 0){
        throw std::runtime_error("Failed to initialize OpenGL context.");
    }
    std::cout << "Version: " << ver << std::endl;
    
    glfwSetWindowPosCallback(_window.get(), [](GLFWwindow* window, int x, int y){
        auto ctx = static_cast<Context*>(glfwGetWindowUserPointer(window));
        // ctx->onWindowMove.emit(ctx, x, y);
    });

    glfwSetKeyCallback(_window.get(), [](GLFWwindow* window, int key, int scancode, int action, int mods){
        auto ctx = static_cast<Context*>(glfwGetWindowUserPointer(window));
        ctx->onKey->emitQueued(ctx->shared_from_this(), key, scancode, action, mods);
    });


    // _bindGlfwCallback<&glfw::Window::setMoveCallback>(onWinMove);
    // _bindGlfwCallback<&glfw::Window::setResizeCallback>(onWinResize);
    // _bindGlfwCallback<&glfw::Window::setCloseCallback>(onWinClose);
    // _bindGlfwCallback<&glfw::Window::setRefreshCallback>(onWinRefresh);
    // _bindGlfwCallback<&glfw::Window::setIconifyCallback>(onWinIconify);
    // _bindGlfwCallback<&glfw::Window::setMaximizeCallback>(onWinMaximize);
    // _bindGlfwCallback<&glfw::Window::setScaleCallback>(onWinScale);
    // _bindGlfwCallback<&glfw::Window::setFramebufferResizeCallback>(onFramebufferResize);

    // _bindGlfwCallback<&glfw::Window::setCursorFocusCallback>(onCursorFocus);
    // _bindGlfwCallback<&glfw::Window::setCursorMoveCallback>(onCursorMove);
    // _bindGlfwCallback<&glfw::Window::setCursorEnterCallback>(onCursorEnter);
    // _bindGlfwCallback<&glfw::Window::setCursorButtonCallback>(onCursorButton);
    // _bindGlfwCallback<&glfw::Window::setCursorScrollCallback>(onCursorScroll);
    // _bindGlfwCallback<&glfw::Window::setKeyCallback>(onKey);
    // _bindGlfwCallback<&glfw::Window::setCharCallback>(onChar);
}