#include "./signals/signals.h"
int main(int argc, char const *argv[])
{	
	pid_t pid;	
	if((pid=fork())==0)
		execl("./eleMng","./eleMng",NULL);
	else if((pid=fork())==0)
		execl("./eleCtrl","./eleCtrl",NULL);
	else if((pid=fork())==0)
		execl("./flrPnl","./flrPnl","5",NULL);		
	else if((pid=fork())==0)
		execl("./flrPnl","./flrPnl","4",NULL);
	else if((pid=fork())==0)
		execl("./flrPnl","./flrPnl","3",NULL);
	else if((pid=fork())==0)
		execl("./flrPnl","./flrPnl","2",NULL);
	else	
		execl("./gndPnl","./gndPnl",NULL);
	return 0;
}