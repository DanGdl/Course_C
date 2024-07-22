sudo cp ./ots_daemon /usr/bin/
sudo cp ./ots_daemon.service /etc/systemd/system/
sudo cp ./ots_daemon.service /lib/systemd/system/
sudo cp ./ots_daemon.conf /etc/ots_daemon.conf

sudo systemctl start ots_daemon
sudo systemctl enable ots_daemon  (same as ln -s /etc/systemd/system/tde_server.service /etc/systemd/system/multi-user.target.wants/tde_server.service)
sudo systemctl status ots_daemon

test:
sudo ./client

Example: https://github.com/jirihnidek/daemon
