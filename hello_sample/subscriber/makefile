CC=gcc

sub: sub.o hello_sample.o ipc.o ipc_stream.o
	$(CC) -o sub sub.o ../threadpool/libthreadpool.a hello_sample.o  ipc.o ipc_stream.o -lzmq -lpthread

sub.o: sub.c
	$(CC) -c -std=c99 -I../ipc -I../ -I../threadpool/src sub.c 

hello_sample.o: ../hello_sample.c
	$(CC) -c -std=c99 -I../ipc ../hello_sample.c

ipc.o: ../ipc/ipc.c
	$(CC) -c -std=c99 -I../ipc ../ipc/ipc.c

ipc_stream.o: ../ipc/ipc_stream.c
	$(CC) -c -std=c99 -I../ipc ../ipc/ipc_stream.c


clean:
	rm -f sub *.o
