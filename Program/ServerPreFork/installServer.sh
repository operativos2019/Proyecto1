cp /usr/Server/src/daemon /usr/bin/daemon
cp /usr/Server/WebServer.service /etc/systemd/system/
systemctl enable WebServer.service
systemctl start WebServer.service
systemctl status WebServer.service