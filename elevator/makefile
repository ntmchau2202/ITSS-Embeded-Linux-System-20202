all: preprare signals.o eleMng eleCtrl gndPnl flrPnl elevator shutdown

preprare:
	rm -rf build
	mkdir build
signals.o: ./signals/signals.c 
	gcc -c -Wall ./signals/signals.c -o ./build/signals.o 																																																			
elevator: ./start.c
	gcc -o ./build/elevator start.c ./build/signals.o
shutdown: ./shutdown.c
	gcc -o ./build/shutdown shutdown.c ./build/signals.o	
eleMng: ./controller/eleMng.c
	gcc -o ./build/eleMng ./controller/eleMng.c ./build/signals.o `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
eleCtrl: ./controller/eleCtrl.c
	gcc -o ./build/eleCtrl ./controller/eleCtrl.c ./build/signals.o	
gndPnl: ./gui/gndPanel.c
	gcc -o ./build/gndPnl ./gui/gndPanel.c ./build/signals.o `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
flrPnl: ./gui/flrPanel.c
	gcc -o ./build/flrPnl ./gui/flrPanel.c ./build/signals.o `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
clean:
	rm -f *.o eleMng eleCtrl gndPnl flrPnl elevator shutdown
