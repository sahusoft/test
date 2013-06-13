CC=gcc
CFLAGS= -Wall -O2
LIB= -lpthread
OBJS=atomic_mutex_test.o 
SRCS=${OBJS:%.o=%.c}

TARGETS=.depend atomic_mutex_test

all:$(TARGETS)

.depend:
	@$(CC) $(CFLAGS) -MM $(SRCS)  > .depend  

-include .depend

atomic_mutex_test: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIB) -o $@
	@echo $@ > .gitignore

clean:
	rm -rf $(OBJS) $(TARGETS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

