CFLAGS = -g -Wall

LIBS = -pthread 

target: multi_threaded_server.o server.o handler.o helper.o queue.o 
	gcc -o target $(CFLAGS) multi_threaded_server.o server.o handler.o helper.o queue.o  $(LIBS)

clean:
	rm -f *.o target

# DONOT USE THESE RULES DIRECTLY
multi_threaded_server.o: multi_threaded_server.c multi_threaded_server.h
	gcc $(CFLAGS) -c multi_threaded_server.c

server.o: server.c server.h
	gcc $(CFLAGS) -c server.c

client.o: client.c client.h
	gcc $(CFLAGS) -c client.c

handler.o: handler.c handler.h
	gcc $(CFLAGS) -c handler.c

helper.o: helper.c helper.h
	gcc $(CFLAGS) -c helper.c

queue.o: queue.c queue.h
	gcc $(CFLAGS) -c queue.c