#pragma once

// #include <atomic>
// #include <functional>
// #include <string>

#include "BS_thread_pool.hpp"

#include "native/glfw/Key.hpp"
#include "native/glfw/KeyAction.hpp"
#include "native/glfw/KeyMod.hpp"
#include "native/utils/Event.hpp"
#include "native/utils/SharedObject.hpp"

struct GLFWwindow;

namespace nglpmt::native {

class Context : public SharedObject<Context> {
    std::shared_ptr<BS::thread_pool> _gl_thread;
public:
    struct Parameters {
        std::string title;
        int width;
        int height;
        int fps;
    };

    static std::shared_ptr<Context> make(const Parameters& params);
    virtual ~Context();

    std::future<void> run();
    const std::thread::id& getThreadId() const;

    // const std::shared_ptr<Event<std::weak_ptr<Context>, const std::chrono::milliseconds&>> onStart;
    const std::shared_ptr<Event<std::weak_ptr<Context>, const std::chrono::milliseconds&>> onRun;     // gl thread
    // const std::shared_ptr<Event<std::weak_ptr<Context>, const std::chrono::milliseconds&>> onFinish;
    // Event<Context*> onDetsroy;

    // Non-gl thread
    // Event<Context*, int, int> onWindowMove;
    // Event<Context*, int, int> onWinResize;
    // Event<Context*> onWinClose;
    // Event<Context*> onWinRefresh;
    // Event<Context*, bool> onWinIconify;
    // Event<Context*, bool> onWinMaximize;
    // Event<Context*, float, float> onWinScale;
    // Event<Context*, int, int> onFramebufferResize;

    // Event<Context*, bool> onCursorFocus;
    // Event<Context*, bool> onCursorEnter;
    // Event<Context*, double, double> onCursorMove;
    // Event<Context*, Button, Action, ModFlags> onCursorButton;
    // Event<Context*, double, double> onCursorScroll;

    // Event<Context*, const Key&, const int&, const KeyAction&, const KeyMods&> onKey;
    // Event<Context*, unsigned int, ModFlags> onChar;
protected:
    Context();
    
private:
    static std::atomic<unsigned int> _glfw_windows;

    std::shared_ptr<GLFWwindow> _window;

    Parameters _params;
    std::thread::id _gl_thread_id;
    // uptr<glfw::Window> _glfw_window;
    // std::atomic<bool> _valid;
    std::chrono::steady_clock::time_point _init_time;
    std::chrono::steady_clock::time_point _last_start_time;
    std::chrono::steady_clock::time_point _last_finish_time;

    void _initGl(const Parameters& params);

    // template<auto setter, class ... Args>
    // void _bindGlfwCallback(Event<Context*, Args...> &event){
    //     std::function<void(glfw::Window*, Args...)> func = [this, &event](glfw::Window*, Args... args){
    //         event.emit(this, args...);
    //     };
    //     (this->_glfw_window.get()->*setter)(func);
    // };
};



} // namespace nglpmt