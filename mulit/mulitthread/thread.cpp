#include <iostream>
#include <unistd.h>
#include <pthread.h>
using namespace std;

int loop = 100000000;

int cnt = 0;

pthread_mutex_t mutex;

void *thread(void *ptr)
{
    pthread_mutex_lock(&mutex);
    for(int i = 0;i < loop;i++) {
        //usleep(500000);
        //cout << "This is a pthread." << endl;
        ++cnt;
    }
    pthread_mutex_unlock(&mutex);
    return 0;
}

int main() {
    pthread_mutex_init(&mutex, NULL);

    pthread_t id;
    int ret = pthread_create(&id, NULL, thread, NULL);
    if(ret) {
        cout << "Create pthread error!" << endl;
        return 1;
    }

    pthread_mutex_lock(&mutex);
    for(int i = 0;i < loop;i++) {
        //cout <<  "This is the main process." << endl;
        //sleep(1);
        ++cnt;
    }
    pthread_mutex_unlock(&mutex);
    pthread_join(id, NULL);

    cout<<cnt<<endl;
    return 0;
}
