miditoserial : main.o serialout.o midircv.o
	gcc -o miditoserial main.o serialout.o midircv.o -lasound

main.o : main.c serialout.h midircv.h
	gcc -c -o main.o main.c

serialout.o : serialout.c serialout.h midircv.h
	gcc -c -o serialout.o serialout.c

midircv.o : midircv.c serialout.h midircv.h
	gcc -c -o midircv.o midircv.c
