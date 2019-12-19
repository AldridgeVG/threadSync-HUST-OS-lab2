//#include <linux/sem.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <iostream>
#include <thread>

using namespace std;

int count = 0;
int semID;

void P(int semid, int index) {
    struct sembuf sem;
    sem.sem_num = index;
    sem.sem_op = -1;
    sem.sem_flg = 0;
    semop(semid, &sem, 1);
    return;
}

void V(int semid, int index) {
    struct sembuf sem;
    sem.sem_num = index;
    sem.sem_op = 1;
    sem.sem_flg = 0;
    semop(semid, &sem, 1);
    return;
}


void subp1() {
    int i = 1;
    for (i = 1; i <= 100; i++) {
        P(semID, 0);
        cout<<"thread 1 adding count, now adding "<<i<<endl;
        count += i;
        V(semID, 1);
    }
    return;
}

void subp2(){
  int i = 1;
    for (i = 1; i <= 100; i++) {
        P(semID, 1);
        cout<<"thread 2 printing, now count "<<count<<endl;
        V(semID, 0);
    }
    return;
}
//???
union senum {int val;} arg1,arg2;

int main() {
    //initiate 2 sem
    key_t key =1;
    semID = semget(key,2,IPC_CREAT|0666);

    //init value of sems
    arg1.val=1;
    arg2.val=0;

    semctl(semID,0,SETVAL,arg1);
    semctl(semID,1,SETVAL,arg2);

    //create thread1,2
    std::thread p1(subp1);
    std::thread p2(subp2);
    p1.join();
    p2.join();

    cout<<"over"<<endl;
    return 0;
}