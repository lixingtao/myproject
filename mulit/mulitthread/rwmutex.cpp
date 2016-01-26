#include <iostream>
#include <unistd.h>
#include <pthread.h>
using namespace std;

int loop = 100000000;

int cnt = 0;

pthread_mutex_t mutex;
pthread_rwlock_t rwlock;

void *thread(void *ptr)
{
    pthread_rwlock_rdlock(&rwlock);
    for(int i = 0;i < loop;i++) {
        //usleep(500000);
        //cout << "This is a pthread." << endl;
        ++cnt;
    }
    pthread_rwlock_unlock(&rwlock);
    return 0;
}


int main() {
    pthread_rwlock_init(&rwlock, NULL);

    pthread_t id;
    int ret = pthread_create(&id, NULL, thread, NULL);
    if(ret) {
        cout << "Create pthread error!" << endl;
        return 1;
    }

    pthread_rwlock_wrlock(&rwlock);
    for(int i = 0;i < loop;i++) {
        //cout <<  "This is the main process." << endl;
        //sleep(1);
        ++cnt;
    }
    pthread_rwlock_unlock(&rwlock);
    pthread_join(id, NULL);

    cout<<cnt<<endl;
    return 0;
}
