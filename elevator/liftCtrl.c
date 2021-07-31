#include "signals.h"

pid_t *pid_list, sensor_process_id, body_process_id;
int des=1, current_floor=1;

int send_liftdown_signals(int destination) {
    if (destination < 1 || destination > 5) {
        return FALSE;
    }

    des = destination;
    printf("Going to floor %d...\n", des);
    send_signal(body_process_id, LIFT_DOWN);
    return TRUE;
}

int send_liftup_signals(int destination) {
    if (destination < 1 || destination > 5) {
        return FALSE;
    }

    des = destination;
    printf("Going to floor %d...\n", des);
    send_signal(body_process_id, LIFT_UP);
    return TRUE;
}

void up_request(int sigNo){	


	if(sigNo == F1_UP || sigNo == F1_CALL) {
		send_liftdown_signals(1);
	} else if (sigNo == F2_UP || sigNo == F2_CALL) {
		send_liftup_signals(2);
	} else if (sigNo == F3_UP || sigNo == F3_CALL) {
		send_liftup_signals(3);
	} else if (sigNo == F4_UP || sigNo == F4_CALL) {
		send_liftup_signals(4);
	} else if (sigNo == F5_UP || sigNo == F5_CALL) {
		send_liftup_signals(5);
	}
}

void sensor_change(int sigNo){	
	int i;			

	if(sigNo == F1_ARRIVAL) {
		send_signal(pid_list[LIFT_MNG],sigNo);		
		if(des==1){
			send_signal(body_process_id,LIFT_STOP);
			printf("Mission completed\n");			
		}	
	} else if (sigNo == F1_ARRIVAL ||
				sigNo == F2_ARRIVAL ||
				sigNo == F3_ARRIVAL ||
				sigNo == F4_ARRIVAL ||
				sigNo == F5_ARRIVAL) {
		i=sigNo-SIGRTMIN;	
		// printf("Value of i: %d\n", i);		
		if(des==i){
			send_signal(body_process_id, LIFT_STOP);				
			send_signal(pid_list[LIFT_MNG], FINISHED);
			sleep(WAIT_TIME);				
			des=1;
			send_signal(body_process_id, LIFT_DOWN);							
		}
		else{
			send_signal(pid_list[LIFT_MNG],sigNo);
		}			
	}
}

void sensor_process_run(){	
	int control_process_id=getppid();
	int previous_position=15;	
	
	while(1){
		usleep(CLOCK);
		
		if(previous_position ^ pid_list[LIFT_POSITION]){
			switch(pid_list[LIFT_POSITION]){			
				case 15:			 
				send_signal(control_process_id, F1_ARRIVAL);break;
				case 45:
				send_signal(control_process_id, F2_ARRIVAL);break;
				case 75: 
				
				send_signal(control_process_id, F3_ARRIVAL);break;
				case 105: 
				
				send_signal(control_process_id, F4_ARRIVAL);break;
				case 135: 
				
				send_signal(control_process_id, F5_ARRIVAL);break;			
				default:break;
			}
			previous_position=pid_list[LIFT_POSITION];
		}					
	}
}

#define VELOCITY 5 // velocity: 0.5m/s
int action=STAND;

void body(int sigNo){
	if(sigNo == LIFT_UP) {
		action=UP;
	} else if (sigNo == LIFT_DOWN) {
		action=DOWN;
	} else if (sigNo == LIFT_STOP) {
		action=STAND;
	}
}

void quit(){
	send_signal(body_process_id,SIGKILL);
	send_signal(sensor_process_id,SIGKILL);
	exit(1);
}

void body_process_run(){// đây là hàm thực hiện công việc chính của liftBody.
	int control_process_id=getppid();
    register_lift_signals(body);
	
	pid_list[LIFT_POSITION]=15;

	while(1){		
		usleep(CLOCK);
		switch(action){
			case DOWN:
				if(pid_list[LIFT_POSITION] > 15) {
					pid_list[LIFT_POSITION] -= VELOCITY;					
				}
				else action=STAND;
				break;			
			case UP:
				if(pid_list[LIFT_POSITION] < 135) {
					pid_list[LIFT_POSITION] += VELOCITY;
					// printf("body_process_run: %d\npid_list: %d\n", LIFT_POSITION, pid_list[LIFT_POSITION]);					
				}
				else action=STAND;
				break;
			default:
				break;
		}
					
	}
}

int main(int argc, char const *argv[]){
	
    register_call_signals(up_request);
    register_up_signals(up_request);
    register_arrival_signals(sensor_change);

	pid_list=update_pid(LIFT_CTR); // lưu pid của tiến trình liftCtrl vào share memory.
	printf("control_process_id: %d\n",getpid());	
	if((sensor_process_id=fork())==0){
		sensor_process_run();
	}
	else{
		printf("sensor_process_id: %d\n",sensor_process_id );
		if((body_process_id=fork())==0){
			body_process_run();
		}
		else{
			signal(SIGINT,quit);
			printf("body_process_id: %d\n",body_process_id );
			while(1){
				sleep(5);	
			}
		}			
	}		
	return 0;
}


