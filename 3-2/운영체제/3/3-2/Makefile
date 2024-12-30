CC = gcc
PROGS = numgen fork thread

all: $(PROGS)

%: %.c
	$(CC) $< -o $@ -pthread

clean:
	rm -rf numgen fork thread

empty:
	rm -rf tmp*
	sync
	echo 3 | sudo tee /proc/sys/vm/drop_caches