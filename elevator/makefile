all:
	rm -f *.o start shutdown mng ctr op1 opx
	gcc -c signals.c	
	gcc -o mng liftMng.c signals.o `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
	gcc -o ctr liftCtrl.c signals.o	
	gcc -o op1 opePanel1.c signals.o `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
	gcc -o opx opePanelX.c signals.o `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
	gcc -o start start.c signals.o
	gcc -o shutdown shutdown.c signals.o	
start: start.c
	rm -f start
	gcc -o start start.c signals.o
shutdown: shutdown.c
	rm -f shutdown
	gcc -o shutdown shutdown.c signals.o	
mng: liftMng.c
	rm -f mng
	gcc -o mng liftMng.c signals.o `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
ctr: liftCtrl.c
	rm -f ctr
	gcc -o ctr liftCtrl.c signals.o
op1: opePanel1.c
	rm -f op1
	gcc -o op1 opePanel1.c signals.o `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
opx: opePanelX.c
	rm -f opx
	gcc -o opx opePanelX.c signals.o `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
clean:
	rm -f *.o start shutdown mng ctr op1 opx
