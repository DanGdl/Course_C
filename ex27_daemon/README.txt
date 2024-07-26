sudo cp ./ex27_daemon /usr/bin/
sudo cp ./ex27_daemon.service /etc/systemd/system/
sudo cp ./ex27_daemon.service /lib/systemd/system/
sudo cp ./ex27_daemon.conf /etc/ots_daemon.conf

sudo systemctl start ex27_daemon
sudo systemctl enable ex27_daemon  (same as ln -s /etc/systemd/system/tde_server.service /etc/systemd/system/multi-user.target.wants/tde_server.service)
sudo systemctl status ex27_daemon

test:
sudo ./ex27_client

Example: https://github.com/jirihnidek/daemon
