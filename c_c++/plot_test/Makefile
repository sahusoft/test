CC=gcc
CFLAGS= -Wall -O2
LIB= -lm
OBJS=plot_test.o 
SRCS=${OBJS:%.o=%.c}

TARGETS=.depend plot_test

all:$(TARGETS)

.depend:
	@$(CC) $(CFLAGS) -MM $(SRCS)  > .depend  

-include .depend

plot_test: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIB) -o $@
	@echo $@ > .gitignore

clean:
	rm -rf $(OBJS) $(TARGETS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

