all: ex27_client ex27_daemon

ex27_daemon: daemon.c
	gcc -g -Wall -Wextra -Wpedantic -std=c11 -D_POSIX_C_SOURCE=200809L daemon.c -o ex27_daemon

ex27_client: client.c
	gcc -Wall -Wextra -Wpedantic -std=c11 -D_POSIX_C_SOURC=200809L client.c -o ex27_client

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions ex27_daemon ex27_client

.PHONY: clean
