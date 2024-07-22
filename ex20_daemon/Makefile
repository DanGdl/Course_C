all: ex20_client ex20_daemon

ex20_daemon: ex20_daemon.c
	gcc -g -Wall -Wextra -Wpedantic -std=c11 -D_POSIX_C_SOURCE=200809L ex20_daemon.c -o ex20_daemon

ex20_client: client.c
	gcc -Wall -Wextra -Wpedantic -std=c11 -D_POSIX_C_SOURC=200809L client.c -o ex20_client

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions ots_daemon client

.PHONY: clean
