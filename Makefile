FP: main.o building.o hunter.o ghost.o room.o evidence.o
	gcc -o FP main.o building.o hunter.o ghost.o room.o evidence.o -lpthread

main.o: main.c defs.h
	gcc -c main.c

building.o: building.c defs.h
	gcc -c building.c

hunter.o: hunter.c defs.h
	gcc -c hunter.c

ghost.o: ghost.c defs.h
	gcc -c ghost.c

room.o: room.c defs.h
	gcc -c room.c

evidence.o: evidence.c defs.h
	gcc -c evidence.c
