#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/stat.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<fcntl.h>
#include<stdlib.h>
#include<time.h>
#include<stdio.h>
#include<string.h>
#include <unistd.h>
#define SEM_KEY1 5555
#define MY_SHM_ID 67483
union semun
{
    int setval;
    struct semid_ds *buf;
    unsigned short *array;
};
int main()
{
    int shmid,semid;
    int *addr;
    int h=0,w=0;
    int ret;
    union semun sem_args;
    unsigned short array[2]={0,1};
    sem_args.array=array;
    //shmid=shmget(MY_SHM_ID,getpagesize(),IPC_CREAT);
    //shmid=shmget(ftok("in.dat",1001),getpagesize(),0);
    if((shmid=shmget(MY_SHM_ID,getpagesize(),IPC_CREAT))==-1)
    {
        perror("shmget error:");
        exit(EXIT_FAILURE);
    }
    semid=semget(SEM_KEY1,2,IPC_CREAT|0666); 
    printf("%d\n", semid);
    if(semid==-1)
    {
        perror("semget error:");
        exit(EXIT_FAILURE);
    }
    ret=semctl(semid,1,SETALL,sem_args);
    if(ret==-1)
    {
        perror("shmctl error:");
        exit(EXIT_FAILURE);
    }
    struct sembuf sem1_opt_wakeup[1]={0,1,SEM_UNDO};
    struct sembuf sem2_opt_wait[1]={1,-1,SEM_UNDO};
    while(1)
    {
        int i=0;
        semop(semid,sem2_opt_wait,1);
        addr=(int *)shmat(shmid,NULL,0);
        printf("please input your infornation:\n");
        scanf("%d%d",addr,addr+1);
        shmdt(addr);
        semop(semid,sem1_opt_wakeup,1);
        //printf("退出清输入1\n");
        //scanf("%d",&i);
        if(i==1)
        {
            shmctl(shmid,IPC_RMID,NULL);
            semctl(semid,1,IPC_RMID,NULL);
            semctl(semid,0,IPC_RMID,NULL);
            exit(1);
        }
    }
    return 0;
}
