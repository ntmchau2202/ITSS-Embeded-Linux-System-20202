#include "../signals/signals.h"

pid_t * sensor_process_id, body_process_id;
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
	switch(sigNo){
		case F1_UP:
		case F1_CALL: {
            send_liftdown_signals(1);
		    break;
        }
        
		case F2_UP:
		case F2_CALL: {
            send_liftdown_signals(2);
		    break;
        }

		case F3_UP:
		case F3_CALL: {
            send_liftup_signals(3);
            break;
        }
		
		break;
		case F4_UP:
		case F4_CALL:{
            send_liftup_signals(4);
            break;
        }
		case F5_UP:
		case F5_CALL:{
            send_liftup_signals(5);
            break;
        }
		default: 
        break;		
	}
}

void sensor_change(int sigNo){	
	int i;			
	switch(sigNo){
		case F1_ARRIVAL:
		send_signal(pid_list[LIFT_MNG],sigNo);		
		if(des==1){
			send_signal(body_process_id,LIFT_STOP);
			printf("Mission completed\n");			
		}	
		break;
		case F2_ARRIVAL:		
		case F3_ARRIVAL:		
		case F4_ARRIVAL:		
		case F5_ARRIVAL:
			i=sigNo-SIGRTMIN;			
			if(des==i){
				send_signal(body_process_id, LIFT_STOP);				
				send_signal(pid_list[LIFT_MNG], FINISHED);
				sleep(WAIT_TIME);				
				des=1;
				send_signal(body_process_id, IFT_DOWN);							
			}
			else{
				send_signal(pid_list[LIFT_MNG],sigNo);
			}			
			break;		
		default:			
		break;
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
	switch(sigNo){
		case LIFT_UP:	action=UP;		break;
		case LIFT_DOWN: action=DOWN;	break;
		case LIFT_STOP: action=STAND;	break;
		default:break;
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

	pid_list=update_pid(LIFT_CTR);// lưu pid của tiến trình liftCtrl vào share memory.
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


