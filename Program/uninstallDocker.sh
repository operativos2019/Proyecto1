#Uninstall package
apt-get purge docker-ce

#Delete images, containers and volumes
rm -rf /var/lib/docker
