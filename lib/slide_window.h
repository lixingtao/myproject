#include <vector>
#include <map>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <queue>
#include <list>


namespace SlideWindow{

    struct Request {
        std::string reference;
        int pn;
        std::string ip;
        std::string uid;
        int time;
        Request() : reference(""), pn(0), ip(""), uid(""), time(0) {}
        void print() const{
            printf("ip: %s\t\t", ip.c_str());
            printf("uid: %s\t\t", uid.c_str());
            printf("reference: %s\t\t", reference.c_str());
            printf("pn: %d\t\t", pn);
            printf("time: %d\n", time);
        }
    };

    const int MAX_WINDOW_SIZE = 10000;
    class Slide_window{
        public:
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
            int window_size;
            int cur_size;
            std::list<Request> window;
        public:
            Slide_window(int size) : window_size(size), cur_size(0) {
                if (size > MAX_WINDOW_SIZE) size = MAX_WINDOW_SIZE;
                window_size = size;
            }
            int get_window_size();
            void set_window_size(int size);
            void push_window(Request req);
            void pop_window();
            Request& get_latest() {return window.front();}
            Request& get_oldest() {return window.back();}
            std::list<Request>::const_iterator begin() {return window.begin();}
            std::list<Request>::const_iterator end() {return window.end();}
            //const std::list<Request> get_all();
            void print_window();
    };
}

