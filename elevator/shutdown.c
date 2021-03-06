#include "./signals/signals.h"
pid_t* pid_list;
int fifoFd1,fifoFd2;

int main(int argc, char const *argv[])
{
	int shmid = shmget(SHARE_KEY,10*sizeof(pid_t),IPC_CREAT);
	pid_list=(pid_t*)shmat(shmid,0,0);
	if(pid_list==(int*)-1){perror("shmat");exit(0);}
		
	//Open for writing only
	if ( ( fifoFd1=open(FIFO_FILE_PATH,O_RDWR|O_NONBLOCK) ) == -1 ){
	    perror("fifofile open");	    
	}	
	close(fifoFd1);
	unlink(FIFO_FILE_PATH);
	remove(FIFO_FILE_PATH);
	if ( ( fifoFd2=open(GND_FIFO_FILE,O_RDWR|O_NONBLOCK) ) == -1 ){
	    perror("fifofile open");	    
	}	
	close(fifoFd2);
	unlink(GND_FIFO_FILE);
	remove(GND_FIFO_FILE);

	sendSignal(pid_list[LIFT_MNG],SIGINT);
    sendSignal(pid_list[LIFT_CTR],SIGINT);    
    for(int i=FLR_PNL1;i<=FLR_PNL5;i++){
        sendSignal(pid_list[i],SIGINT);
    }    	   
    shmdt(pid_list);
    releaseSharedMemory();    		
	return 0;
}