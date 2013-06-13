CC=gcc
CFLAGS=-Wall -O2

TARGET= depend server client

all:$(TARGET)

depend:
	@$(CC) $(CFLAGS) -MM *.c > .depend

-include .depend

server: server.o shm_sem_test.o
	$(CC) $(CFLAGS) $^ -o $@
	@echo $@ > .gitignore

client: client.o shm_sem_test.o
	$(CC) $(CFLAGS) $^ -o $@
	@echo $@ > .gitignore

clean:
	rm -rf *.o $(TARGET)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
