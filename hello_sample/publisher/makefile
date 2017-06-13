CC=gcc

pub: pub.o hello_sample.o ipc.o ipc_stream.o
	$(CC) -o pub pub.o hello_sample.o ipc.o ipc_stream.o -lzmq

pub.o: pub.c
	$(CC) -c -std=c99 -I../ipc -I../ pub.c 

hello_sample.o: ../hello_sample.c
	$(CC) -c -std=c99 -I../ipc ../hello_sample.c

ipc.o: ../ipc/ipc.c
	$(CC) -c -std=c99 -I../ipc ../ipc/ipc.c

ipc_stream.o: ../ipc/ipc_stream.c
	$(CC) -c -std=c99 -I../ipc ../ipc/ipc_stream.c


clean:
	rm -f pub *.o
