#include "../signals/signals.h"
#define MAX_QUEUE 5

pid_t* pid_list;
int lift_is_moving=FALSE;
int fifoFd;
int current_request,current_floor=1;

void up_request(int sigNo){	
    FILE *f;
	if(lift_is_moving) {
		write(fifoFd, &sigNo, sizeof(int));
	}
	else{
		lift_is_moving = send_signal(pid_list[LIFT_CTR],sigNo);
		current_request = sigNo;
	}
}

int send_up_signals(int up_signal) {

    if (up_signal < F2_UP || up_signal > F5_UP) {
        return FALSE;
    }

    int requested_floor = up_signal - SIGRTMIN - 4;
    int ground_floor = OPE_PANE1;

    int arrival_signal = up_signal - 5;
    printf("    Floor %d reached from ground. Getting items...\n", requested_floor - 1);
    send_signal(pid_list[ground_floor], FINISHED);
    send_signal(pid_list[requested_floor], USING);
    
    int floor;
    for (floor = OPE_PANE1; floor <= OPE_PANE5; floor++){
        if(floor != requested_floor && floor != ground_floor) {
            send_signal(pid_list[floor], arrival_signal);
        }
    }

    return TRUE;
}

int send_call_signals(int call_signal){
    if (call_signal < F2_CALL || call_signal > F5_CALL) {
        return FALSE;
    }

    int current_floor = call_signal - SIGRTMIN - 9;

    int arrival_signal = call_signal - 10;
    printf("    Floor %d reached. Getting items...\n", current_floor - 1);
    send_signal(pid_list[current_floor], FINISHEDUSING);
    
    int floor;
    for (floor = OPE_PANE1; floor <= OPE_PANE5; floor++){
        if(floor != current_floor) {
            send_signal(pid_list[floor], arrival_signal);
        }
    }

    return TRUE;
}

void broadcast_signals(int signal){
    int floor;
    for(floor = OPE_PANE1; floor <= OPE_PANE5; floor++){
        send_signal(pid_list[floor], signal);
    }
}

void send_finish_notification(int sigNo){
    if(current_request >= F2_UP && current_request <= F5_UP) {
        send_up_signals(current_request);
    } else if (current_request >= F2_CALL && current_request <= F5_CALL) {
        send_call_signals(current_request);
    }
}

int send_arrival_signals(int arrive_signal){
     if (arrive_signal < F1_ARRIVAL || arrive_signal > F5_ARRIVAL) {
        return FALSE;
    }

    int ground_floor = OPE_PANE1;

    send_signal(pid_list[ground_floor], USING);
    int floor;
    for(floor = OPE_PANE2; floor <= OPE_PANE5; floor++) {
        send_signal(pid_list[floor], arrive_signal);
    }
    
    return TRUE;
}

void lift_arrival(int sigNo){
	int sigNumber;

    if(sigNo == FINISHED) {
        send_finish_notification(sigNo);
    } else if (sigNo == F1_ARRIVAL) {
        int success = send_arrival_signals(sigNo);
        if(success == TRUE) {
            if(read(fifoFd, &sigNumber, sizeof(int)) > 0){
            sleep(WAIT_TIME);		
            lift_is_moving = send_signal(pid_list[LIFT_CTR], sigNumber);
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
        broadcast_signals(sigNo);
    } else {
        lift_is_moving = TRUE;
    }
}

int main(int argc, char const *argv[])
{
	register_call_signals(up_request);
    register_up_signals(up_request);
    register_arrival_signals(lift_arrival);
    register_finished_signals(lift_arrival);

    
	pid_list=update_pid(LIFT_MNG);
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
