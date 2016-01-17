#include "slide_window.h"

namespace SlideWindow{

    int Slide_window::get_window_size() {
        return window_size;
    }
    void Slide_window::set_window_size(int size) {
        if (size > MAX_WINDOW_SIZE) size = MAX_WINDOW_SIZE;
        window_size = size;
    }
    void Slide_window::push_window(Request req) {
        if (cur_size == window_size) {
            pop_window();
        }
        window.push_front(req);
        ++cur_size;
    }
    void Slide_window::pop_window() {
        if (cur_size > 0) {
            window.pop_back();
            --cur_size;
        }
    }
    void Slide_window::print_window() {
        printf("/*******************/\nbegin print window\n");
        iterator it = begin();
        for (; it != end(); ++it) {
            (*it).print();
        }
        printf("finish\n");
    }
}

