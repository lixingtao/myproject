#include "slide_window.h"
#include "stropt.h"

namespace SlideWindow{

    int strtoreq(char *buf, Request &req) {
        trim(buf);
        std::vector<std::string> ret;
        explode(buf, ret);
        if (ret.size() < 6) return -1;
        req.time = atoi(ret[0].c_str()); req.ip = ret[1]; req.uid = ret[2];
        req.preurl = ret[3]; req.qt = ret[4]; req.pn = atoi(ret[5].c_str());
        return 0;
    }

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
        push_cnt(req.time);
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

    void Slide_window::count_init() {
        lastest_timestamp = 0;
        max_second = 60;
        max_minute = 60;
        max_hour = 24;
    }

    void Slide_window::push_cnt(int timestamp) {
        push_cnt_sec(timestamp);
        push_cnt_min(timestamp);
        push_cnt_hour(timestamp);
        lastest_timestamp = timestamp;
    }
    void Slide_window::push_cnt_core(std::list<int> &count, int maxlen, int lastest, int time) {
        if (time - lastest >= maxlen) {
            count.clear();
            for (int i = 0; i < maxlen; ++i) {
                count.push_front(0);
            }
        }
        else if (time > lastest) {
            for (int i = lastest; i < time; i++) {
                count.push_front(0);
                if (count.size() > maxlen) count.pop_back();
            }
        }
        if (count.empty()) printf("push_cnt_core error, in:%s", __FILE__); ////
        ++(*(count.begin()));
    }
    void Slide_window::push_cnt_sec(int timestamp) {
        push_cnt_core(second_cnt, max_second, lastest_timestamp, timestamp);
    }
    void Slide_window::push_cnt_min(int timestamp) {
        push_cnt_core(minute_cnt, max_minute, lastest_timestamp/60, timestamp/60);
    }
    void Slide_window::push_cnt_hour(int timestamp) {
        push_cnt_core(hour_cnt, max_hour, lastest_timestamp/3600, timestamp/3600);
    }
    int Slide_window::get_cnt_core(std::list<int> &count, int t) {
        int ret = 0;
        std::list<int>::iterator it = count.begin();
        for (int i = 0; i < t && it != count.end(); ++it , ++i) {
            ret += *it;
        }
        return ret;
    }
    int Slide_window::get_cnt_sec(int t) {
        if (t > max_second) return -1;
        return get_cnt_core(second_cnt, t);
    }
    int Slide_window::get_cnt_min(int t) {
        if (t > max_minute) return -1;
        return get_cnt_core(minute_cnt, t);
    }
    int Slide_window::get_cnt_hour(int t) {
        if (t > max_hour) return -1;
        return get_cnt_core(hour_cnt, t);
    }
}

