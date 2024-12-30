OBJS = srv cli
CC = gcc

all: $(OBJS)

srv: srv.c
	$(CC) -o $@ $^

cli: cli.c
	$(CC) -o $@ $^

clean:
	rm -rf $(OBJS)