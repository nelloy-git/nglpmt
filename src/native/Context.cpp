#include "native/Context.hpp"

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

using namespace nglpmt::native;

std::atomic<unsigned int> Context::_glfw_windows = 0;

std::shared_ptr<Context> Context::make(const Parameters& params){
    auto ctx = std::shared_ptr<Context>(new Context);
    auto future = ctx->_gl_thread->submit([ctx, params](){
        std::cout << "GL: " << std::this_thread::get_id() << std::endl;
        ctx->_initGl(params);
    });
    ctx->_gl_thread->unpause();
    ctx->_gl_thread->wait_for_tasks();
    // ctx->_gl_thread->pause();
    return ctx;
}

Context::Context() :
    SharedObject(),
    _gl_thread(new BS::thread_pool(1)),
    _init_time(std::chrono::steady_clock::now()),
    _last_start_time(std::chrono::steady_clock::now()),
    _last_finish_time(std::chrono::steady_clock::now()),

    // onStart(decltype(onRun)::element_type::make()),
    onRun(decltype(onRun)::element_type::make(_gl_thread))
    // onFinish(decltype(onRun)::element_type::make())
    {

    // onRun->push([](std::weak_ptr<Context> weak_self){
    //     auto self = weak_self.lock();
    //     if (not self){
    //         return false;
    //     }

    //     glfwPollEvents();
    //     glfwSwapBuffers(self->_window.get());
    //     return true;
    // });
}

Context::~Context(){
    std::cout << __FUNCTION__ << std::endl;
    // auto future = run();
    // future.wait();
    _gl_thread->unpause();
    _gl_thread->wait_for_tasks();
    std::cout << __FUNCTION__ << std::endl;
}

std::future<void> Context::run(){
    auto now = std::chrono::steady_clock::now();
    auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(now - _last_start_time);
    _last_start_time = now;

    // onRun->addActionQueued([](std::weak_ptr<Context> weak_self){
    //     std::cout << "Here" << std::endl;
    //     // auto self = weak_self.lock();
    //     // if (self){self->_gl_thread->pause();}
    //     return false;
    // });
    auto future = onRun->emitQueued(this->weak_from_this(), dt);
    _gl_thread->unpause();
    return future;
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