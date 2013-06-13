CC=gcc
CFLAGS= -Wall -O2
LIB= -lpthread
OBJS=gcc_sync_test.o 
SRCS=${OBJS:%.o=%.c}

TARGETS=.depend gcc_sync_test

all:$(TARGETS)

.depend:
	@$(CC) $(CFLAGS) -MM $(SRCS)  > .depend  

-include .depend

gcc_sync_test: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIB) -o $@
	@echo $@ > .gitignore

clean:
	rm -rf $(OBJS) $(TARGETS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

