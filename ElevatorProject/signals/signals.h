#ifndef _SIGS_H_
#define _SIGS_H_
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

#include <sys/stat.h>
#include <sys/errno.h>
#include <sys/fcntl.h>


enum {
    LIFT_MNG, // idx = 0
    LIFT_CTR, // 1
    OPE_PANE1, // 2
    OPE_PANE2, // 3
    OPE_PANE3, // 4
    OPE_PANE4, // 5
    OPE_PANE5, // 6
    LIFT_POSITION // 7
};

/***** BASIC PARAMETERS *****/

#define CLOCK 300000
#define SHARE_KEY 3579
#define WAIT_TIME 3

#define TRUE 1
#define FALSE 0
#define SUCCESS 1
#define FAILED 0

#define FIFO_FILE_PATH "requestsqueue"
#define OPE1_FIFO_FILE "openfifofile"
#define BUFF_SIZE 256


/***** BEHAVIOR OF ELEVATOR *****/

#define UP 1
#define STAND 0
#define DOWN -1

#define F1_ARRIVAL SIGRTMIN + 1
#define F2_ARRIVAL SIGRTMIN + 2
#define F3_ARRIVAL SIGRTMIN + 3
#define F4_ARRIVAL SIGRTMIN + 4
#define F5_ARRIVAL SIGRTMIN + 5

#define F1_UP SIGRTMIN + 6 // SIGRTMIN + enum + 4 
#define F2_UP SIGRTMIN + 7
#define F3_UP SIGRTMIN + 8
#define F4_UP SIGRTMIN + 9
#define F5_UP SIGRTMIN + 10

#define F1_CALL SIGRTMIN + 11 // SIGRTMIN + enum + 9
#define F2_CALL SIGRTMIN + 12
#define F3_CALL SIGRTMIN + 13
#define F4_CALL SIGRTMIN + 14
#define F5_CALL SIGRTMIN + 15

#define LIFT_UP    SIGRTMIN + 16
#define LIFT_DOWN  SIGRTMIN + 17
#define LIFT_STOP  SIGRTMIN + 18

#define USING           SIGRTMIN + 19
#define FINISHED        SIGRTMIN + 20
#define FINISHEDUSING   SIGRTMIN + 21

/***** SIGNAL REGISTER FUNCTIONS *****/

void register_up_signals(sighandler_t handler);
void register_arrival_signals(sighandler_t handler);
void register_finished_signals(sighandler_t handler);
void register_call_signals(sighandler_t handler);
void register_using_signals(sighandler_t handler);
void register_lift_signals(sighandler_t handler);

pid_t* update_pid(int i);
void release_shm();
int send_signal(pid_t pid, int sigNo);

#endif
