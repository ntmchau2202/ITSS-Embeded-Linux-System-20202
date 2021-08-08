#include "../signals/signals.h"

pid_t *pid_list, sensor_process_id, body_process_id;
int des=1, current_floor=1;

int sendLiftdownSignals(int destination) {
    if (destination < 1 || destination > 5) {
        return FALSE;
    }

    des = destination;
    printf("Going to floor %d...\n", des);
    sendSignal(body_process_id, LIFT_DOWN);
    return TRUE;
}

int sendLiftupSignals(int destination) {
    if (destination < 1 || destination > 5) {
        return FALSE;
    }

    des = destination;
    printf("Going to floor %d...\n", des);
    sendSignal(body_process_id, LIFT_UP);
    return TRUE;
}

void upRequest(int sigNo){	


	if(sigNo == F1_UP || sigNo == F1_CALL) {
		sendLiftdownSignals(1);
	} else if (sigNo == F2_UP || sigNo == F2_CALL) {
		sendLiftupSignals(2);
	} else if (sigNo == F3_UP || sigNo == F3_CALL) {
		sendLiftupSignals(3);
	} else if (sigNo == F4_UP || sigNo == F4_CALL) {
		sendLiftupSignals(4);
	} else if (sigNo == F5_UP || sigNo == F5_CALL) {
		sendLiftupSignals(5);
	}
}

void sensorChange(int sigNo){	
	int i;			

	if(sigNo == F1_ARRIVAL) {
		sendSignal(pid_list[LIFT_MNG],sigNo);		
		if(des==1){
			sendSignal(body_process_id,LIFT_STOP);
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
			sendSignal(body_process_id, LIFT_STOP);				
			sendSignal(pid_list[LIFT_MNG], FINISHED);
			sleep(WAIT_TIME);				
			des=1;
			sendSignal(body_process_id, LIFT_DOWN);							
		}
		else{
			sendSignal(pid_list[LIFT_MNG],sigNo);
		}			
	}
}

void sensorRun(){	
	int control_process_id=getppid();
	int previous_position=15;	
	
	while(1){
		usleep(CLOCK);
		
		if(previous_position ^ pid_list[LIFT_POSITION]){
			switch(pid_list[LIFT_POSITION]){			
				case 15:			 
				sendSignal(control_process_id, F1_ARRIVAL);break;
				case 45:
				sendSignal(control_process_id, F2_ARRIVAL);break;
				case 75: 
				
				sendSignal(control_process_id, F3_ARRIVAL);break;
				case 105: 
				
				sendSignal(control_process_id, F4_ARRIVAL);break;
				case 135: 
				
				sendSignal(control_process_id, F5_ARRIVAL);break;			
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
	sendSignal(body_process_id,SIGKILL);
	sendSignal(sensor_process_id,SIGKILL);
	exit(1);
}

void bodyRun(){// đây là hàm thực hiện công việc chính của liftBody.
	int control_process_id=getppid();
    registerLiftSignals(body);
	
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
					// printf("bodyRun: %d\npid_list: %d\n", LIFT_POSITION, pid_list[LIFT_POSITION]);					
				}
				else action=STAND;
				break;
			default:
				break;
		}
					
	}
}

int main(int argc, char const *argv[]){
	
    registerCallSignals(upRequest);
    registerUpSignals(upRequest);
    registerArrivalSignals(sensorChange);

	pid_list=updatePID(LIFT_CTR); // lưu pid của tiến trình liftCtrl vào share memory.
	printf("control_process_id: %d\n",getpid());	
	if((sensor_process_id=fork())==0){
		sensorRun();
	}
	else{
		printf("sensor_process_id: %d\n",sensor_process_id );
		if((body_process_id=fork())==0){
			bodyRun();
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


