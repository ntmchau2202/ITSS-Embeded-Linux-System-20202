#include "../signals/signals.h"
#define MAX_QUEUE 5

pid_t* pid_list;
int lift_is_moving=FALSE;
int fifoFd;
int current_request,current_floor=1;

void upRequest(int sigNo){	
    FILE *f;
	if(lift_is_moving) {
		write(fifoFd, &sigNo, sizeof(int));
	}
	else{
		lift_is_moving = sendSignal(pid_list[LIFT_CTR],sigNo);
		current_request = sigNo;
	}
}

int sendUpSignals(int up_signal) {

    if (up_signal < F2_UP || up_signal > F5_UP) {
        return FALSE;
    }

    int requested_floor = up_signal - SIGRTMIN - 4;
    int ground_floor = FLR_PNL1;

    int arrival_signal = up_signal - 5;
    printf("    Floor %d reached from ground. Getting items...\n", requested_floor - 1);
    sendSignal(pid_list[ground_floor], FINISHED);
    sendSignal(pid_list[requested_floor], USING);
    
    int floor;
    for (floor = FLR_PNL1; floor <= FLR_PNL5; floor++){
        if(floor != requested_floor && floor != ground_floor) {
            sendSignal(pid_list[floor], arrival_signal);
        }
    }

    return TRUE;
}

int sendCallSignals(int call_signal){
    if (call_signal < F2_CALL || call_signal > F5_CALL) {
        return FALSE;
    }

    int current_floor = call_signal - SIGRTMIN - 9;

    int arrival_signal = call_signal - 10;
    printf("    Floor %d reached. Getting items...\n", current_floor - 1);
    sendSignal(pid_list[current_floor], FINISHEDUSING);
    
    int floor;
    for (floor = FLR_PNL1; floor <= FLR_PNL5; floor++){
        if(floor != current_floor) {
            sendSignal(pid_list[floor], arrival_signal);
        }
    }

    return TRUE;
}

void broadcastSignals(int signal){
    int floor;
    for(floor = FLR_PNL1; floor <= FLR_PNL5; floor++){
        sendSignal(pid_list[floor], signal);
    }
}

void sendFinishNotifications(int sigNo){
    if(current_request >= F2_UP && current_request <= F5_UP) {
        sendUpSignals(current_request);
    } else if (current_request >= F2_CALL && current_request <= F5_CALL) {
        sendCallSignals(current_request);
    }
}

int sendArrivalSignals(int arrive_signal){
     if (arrive_signal < F1_ARRIVAL || arrive_signal > F5_ARRIVAL) {
        return FALSE;
    }

    int ground_floor = FLR_PNL1;

    sendSignal(pid_list[ground_floor], USING);
    int floor;
    for(floor = FLR_PNL2; floor <= FLR_PNL5; floor++) {
        sendSignal(pid_list[floor], arrive_signal);
    }
    
    return TRUE;
}

void liftArrival(int sigNo){
	int sigNumber;

    if(sigNo == FINISHED) {
        sendFinishNotifications(sigNo);
    } else if (sigNo == F1_ARRIVAL) {
        int success = sendArrivalSignals(sigNo);
        if(success == TRUE) {
            if(read(fifoFd, &sigNumber, sizeof(int)) > 0){
            sleep(WAIT_TIME);		
            lift_is_moving = sendSignal(pid_list[LIFT_CTR], sigNumber);
            current_request = sigNumber;
            }
            else{
                lift_is_moving = FALSE;
                printf("Lift stopped!\n");
            }
        } else {
            printf("An error occurred when elevating...\n");
        }
    } else if (sigNo == F2_ARRIVAL ||
                sigNo == F3_ARRIVAL ||
                sigNo == F4_ARRIVAL ||
                sigNo == F5_ARRIVAL) {
        broadcastSignals(sigNo);
    } else {
        lift_is_moving = TRUE;
    }
}

int main(int argc, char const *argv[])
{
	registerCallSignals(upRequest);
    registerUpSignals(upRequest);
    registerArrivalSignals(liftArrival);
    registerFinishedSignals(liftArrival);

    
	pid_list=updatePID(LIFT_MNG);
	setpgid(pid_list[LIFT_MNG],0);
	printf("mng_process_id: %d\n",getpid());

	//Make FIFO file
	remove(FIFO_FILE_PATH);
	if ( mkfifo(FIFO_FILE_PATH,0666) == -1 ){
	    perror("mkfifo");
	    exit(1);
	}
	//Open for writing only
	if ( ( fifoFd=open(FIFO_FILE_PATH,O_RDWR|O_NONBLOCK) ) == -1 ){
	    perror("fifofile open");
	    exit(1);
	}

	while(1){
		sleep(1);
	}
	close(fifoFd);
	unlink(FIFO_FILE_PATH);
	return 0;
}
