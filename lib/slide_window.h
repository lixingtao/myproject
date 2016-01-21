#ifndef slide_window

#define slide_window
#include <vector>
#include <map>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <queue>
#include <list>


namespace SlideWindow{

    /**请求数据结构**/
    struct Request {
        std::string reference;
        int pn;
        std::string ip;
        std::string uid;
        int time;
        std::string qt;
        std::string preurl;
        Request() : preurl(""), reference(""), qt(""), pn(0), ip(""), uid(""), time(0) {}
        void print() const{
            printf("ip: %s\t\t", ip.c_str());
            printf("uid: %s\t\t", uid.c_str());
            printf("reference: %s\t\t", reference.c_str());
            printf("pn: %d\t\t", pn);
            printf("qt: %s\t\t", qt.c_str());
            printf("time: %d\n", time);
        }
    };

    int strtoreq(char *buf, Request &req);
    const int INF = 2000000000;
    const int MAX_WINDOW_SIZE = 20000;
    class Slide_window{
        public:
            /**slide_window 遍历迭代器 **/
            class iterator {
                public:
                    iterator(std::list<Request>::const_iterator it) {
                        index = it;
                    }
                    iterator() {
                    }

                    iterator& operator=(iterator& rhs) {
                        index = rhs.get_real_index();
                        return *this;
                    }
                    iterator& operator=(std::list<Request>::const_iterator it) {
                        index = it;
                        return *this;
                    }
                    iterator& operator++() {
                        ++index;
                        return *this;
                    }
                    iterator& operator++(int) {
                        ++index;
                        return *this;
                    }
                    bool operator!=(iterator& rhs) {
                        return rhs.index != index;
                    }
                    bool operator!=(std::list<Request>::const_iterator it) {
                        return it != index;
                    }
                    const Request& operator*() {
                        return (*index);
                    }
                    std::list<Request>::const_iterator get_real_index() {
                        return index;
                    }
                private:
                    std::list<Request>::const_iterator index;
            };
        private:
            std::list<int> second_cnt;
            std::list<int> minute_cnt;
            std::list<int> hour_cnt;
            int max_second;
            int max_minute;
            int max_hour;
            int lastest_timestamp;

            void count_init();
            void push_cnt(int timestamp);
            void push_cnt_sec(int timestamp);
            void push_cnt_min(int timestamp);
            void push_cnt_hour(int timestamp);
            void push_cnt_core(std::list<int> &count, int maxlen, int lastest, int timestamp);
            int get_cnt_core(std::list<int> &count, int t); 

        private:
            int window_size;
            int cur_size;
            std::list<Request> window;
        public:
            Slide_window(int size) : window_size(size), cur_size(0) {
                if (size > MAX_WINDOW_SIZE) size = MAX_WINDOW_SIZE;
                window_size = size;
                count_init();
            }
            int get_window_size();
            void set_window_size(int size);
            void push_window(Request req);
            void pop_window();
            Request& get_latest() {return window.front();}
            Request& get_oldest() {return window.back();}
            std::list<Request>::const_iterator begin() {return window.begin();}
            std::list<Request>::const_iterator end() {return window.end();}
            void print_window();
            int get_cnt_sec(int t); //t < 60
            int get_cnt_min(int t); //t < 60
            int get_cnt_hour(int t); //t < 24
    };
}
#endif
