#include "signals.h"


pid_t* update_pid(int i){
	int shmid = shmget(SHARE_KEY,10*sizeof(pid_t),IPC_CREAT);
	pid_t *shmpointer = (pid_t*)shmat(shmid,0,0);
	
    if(shmpointer==(int*)-1){
        perror("shmat");
        exit(0);
    }

	shmpointer[i]=getpid();
	return shmpointer;
}
void release_shm(){
	int shmid = shmget(SHARE_KEY,10*sizeof(pid_t),IPC_CREAT);
	shmctl(shmid,IPC_RMID,NULL);
}

int send_signal(pid_t pid,int sigNo){
	if (pid>0) {
        return (kill(pid,sigNo)==0);
    } else return 0;
}

void register_up_signals(sig_t handler){
    signal(F2_UP, handler);	
    signal(F3_UP, handler);	
    signal(F4_UP, handler);	
    signal(F5_UP, handler);
}

void register_arrival_signals(sig_t handler){
    signal(F1_ARRIVAL, handler);
    signal(F2_ARRIVAL, handler);
    signal(F3_ARRIVAL, handler);
    signal(F4_ARRIVAL, handler);
    signal(F5_ARRIVAL, handler);
}

void register_finished_signals(sig_t handler){
    signal(FINISHED, handler);
}

void register_using_signals(sig_t handler){
    signal(USING, handler);
}

void register_finished_using_signals(sig_t handler){
    signal(FINISHEDUSING, handler);
}

void register_call_signals(sig_t handler){
    signal(F2_CALL, handler);	
    signal(F3_CALL, handler);	
    signal(F4_CALL, handler);	
    signal(F5_CALL, handler);
}

void register_lift_signals(sig_t handler){
    signal(LIFT_UP, handler);
    signal(LIFT_DOWN, handler);
    signal(LIFT_STOP, handler);
}