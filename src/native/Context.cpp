#include "native/Context.hpp"

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

using namespace nglpmt::native;

std::atomic<unsigned int> Context::_glfw_windows = 0;

std::shared_ptr<Context> Context::make(const Parameters& params){
    auto ctx = std::shared_ptr<Context>(new Context);
    auto future = ctx->_gl_thread->submit([ctx, params](){
        ctx->_initGl(params);
    });
    ctx->_gl_thread->unpause();
    ctx->_gl_thread->wait_for_tasks();
    ctx->_gl_thread->pause();


    return ctx;
}

Context::Context() :
    SharedObject(),
    _gl_thread(new BS::thread_pool(1)),
    _last_start(std::chrono::steady_clock::now()),
    onRun(Event<Context*, const std::chrono::milliseconds&>::make(_gl_thread))
    // onDetsroy(_gl_thread)
    {
}

Context::~Context(){
    auto future = _gl_thread->submit([this](){
        // onDetsroy.emit(this);
    });

    _gl_thread->unpause();
    _gl_thread->wait_for_tasks();
    _gl_thread->pause();
}

bool Context::start(){
    onRun->push([this](){
        glfwPollEvents();
        glfwSwapBuffers(_window.get());
        _gl_thread->pause();
    });

    auto now = std::chrono::steady_clock::now();
    auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(now - _last_start);
    _last_start = now;

    onRun->emit(this, dt);
    _gl_thread->unpause();
    return true;
}

void Context::wait(){
    _gl_thread->wait_for_tasks();
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
        {GLFW_VERSION_MAJOR, 4},
        {GLFW_VERSION_MINOR, 2},
        {GLFW_CONTEXT_VERSION_MAJOR, 4},
        {GLFW_CONTEXT_VERSION_MINOR, 2},
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
    
    glfwSetWindowPosCallback(_window.get(), [](GLFWwindow* window, int x, int y){
        auto ctx = static_cast<Context*>(glfwGetWindowUserPointer(window));
        // ctx->onWindowMove.emit(ctx, x, y);
    });

    glfwSetKeyCallback(_window.get(), [](GLFWwindow* window, int key, int scancode, int action, int mods){
        auto ctx = static_cast<Context*>(glfwGetWindowUserPointer(window));
        // ctx->onKey.emit(ctx, glfwToKey(key), scancode, glfwToKeyAction(action), mods);
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