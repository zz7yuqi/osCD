//#include  头文件pthread.h、sys/types.h、linux/sem.h等
#include <sys/types.h>
#include <sys/sem.h>
#include <stdio.h>
#include <sys/shm.h>
//#include <sys/ipc.h>
#include <stdlib.h>

#define KEY1 7778
#define KEY2 3331
//P V 操作定义
void P(int semid, int index)
{
    struct sembuf sem;
    sem.sem_num = index;
    sem.sem_op = -1;
    sem.sem_flg = 0;       //操作标记：0或IPC_NOWAIT等
    semop(semid, &sem, 1); //1:表示执行命令的个数
    return;
}

void V(int semid, int index)
{
    struct sembuf sem;
    sem.sem_num = index;
    sem.sem_op = 1;
    sem.sem_flg = 0;
    semop(semid, &sem, 1);
    return;
}

//信号灯、线程句柄定义：
int semid, shmid;	
//子进程
int p1,p2,p3;

int  main(){
    // 创建信号灯；
    semid = semget(KEY1, 4, IPC_CREAT | 0666);
    if (semid == -1){
        printf("信号灯创建失败！\n");
    }

        //信号灯赋初值；
       semctl(semid,0,SETVAL,1);     //s1
       semctl(semid,1,SETVAL,0);     //t1
       semctl(semid,2,SETVAL,1);    //s2
       semctl(semid,3,SETVAL,0);    //t2
       //创建共享内存
    shmid = shmget(KEY2, 200, IPC_CREAT | 0666);
    if ((shmid) == -1){
        printf("!创建共享内存失败！\n");
    }
    //创建第一个子进程
    if ((p1 = fork()) == -1)
    { 
        printf("fork error!\n");
    }

    if (p1 == 0)
    {
        execv("/home/zz7/build-get-unknown-Debug/get", NULL);
    }
    else
    {
            printf("!!!dd");
        if ((p2 = fork()) == -1)
        { //创制第二个子进程
            printf("fork error!\n");
        }
        if (p2 == 0)
        {
            execv("/home/zz7/build-copy-unknown-Debug/copy", NULL);
        }
        else{
            if ((p3= fork()) == -1){
                //创制第三个子进程
                printf("fork error!\n");
            }
            if (p3 == 0){
                 execv("/home/zz7/build-put-unknown-Debug/put", NULL);
            }
            wait();
            wait();
            wait();
            // 	删除信号灯；
            semctl(semid, 0, IPC_RMID);
            //删除共享内存
            shmctl(shmid, 0, IPC_RMID);
        }
    }

    return 0;
    
}