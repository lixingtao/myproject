#include<sys/types.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#define SEM_KEY 5555
#define MY_SHM_ID 67483
int main()
{
    int shmid,semid;
    int *addr;
    int h,w;
    int result,sum;
    struct sembuf sem1_opt_wait[1]={0,-1,SEM_UNDO};
    struct sembuf sem2_opt_wakeup[1]={1,1,SEM_UNDO};
    while(1)
    {
        //shmid=shmget(ftok("/etc/passwd",1001),getpagesize(),S_IRUSR| S_IWUSR| S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
        //shmid=shmget(ftok("in.dat",1001),getpagesize(),S_IRUSR| S_IWUSR| S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
        sleep(1);
        shmid=shmget(MY_SHM_ID,getpagesize(),IPC_CREAT);
        if((shmid=shmget(MY_SHM_ID,getpagesize(),IPC_CREAT))==-1)
        {
            perror("shmget error:");
            exit(EXIT_FAILURE);
        }
        //semid=semget(SEM_KEY,2,0600);
        semid=semget(SEM_KEY,2,0);
        if(semid==-1)
        {
            perror("semget error:");
            exit(EXIT_FAILURE);
        }
        addr=(int*)shmat(shmid,0,0);
        semop(semid,sem1_opt_wait,1);
        w=*(addr+1);
        h=*addr;
        //sum=h*h/10000;
        //result=w/sum;
        result = w + h;
        //printf("%s\n", (char*)addr);


        printf("%d\n", result);

        /*if(result>25)
        {
            printf("胖拉！\n");
            *(addr+2)=0;
        }
        else if(result<20)
        {
            printf("瘦啦！\n");
            *(addr+2)=0;
        }
        else
        {
            if(result>5&&result<20)
            {
                printf("正常！\n");
                *(addr+2)=0;
            }
        }*/
        shmdt(addr);
        semop(semid,sem2_opt_wakeup,1);
        sleep(2);
    }
}
