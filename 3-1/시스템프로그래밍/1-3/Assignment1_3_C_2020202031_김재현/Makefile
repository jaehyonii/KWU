all: cli srv

cli: cli.c
	gcc -o cli cli.c

srv: srv.c
	gcc -o srv srv.c
	
clean:
	rm -rf cli srv
