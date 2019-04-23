########################################################################################################
##This file is used to create the docker image from which the container is going to be instantiated from
########################################################################################################
#Uses the centos/systemd image than contains a functional installed version of systemd
FROM centos/systemd
#Sets the environment as a docker container
ENV container docker
#Removes the necessary files for systemd to run correctly
RUN (cd /lib/systemd/system/sysinit.target.wants/; for i in *; do \ 
    [ $i == systemd-tmpfiles-setup.service ] || rm -f $i; done); 
RUN rm -f /lib/systemd/system/multi-user.target.wants/*;
RUN rm -f /etc/systemd/system/*.wants/*;
RUN rm -f /lib/systemd/system/local-fs.target.wants/*; 
RUN rm -f /lib/systemd/system/sockets.target.wants/*udev*; 
RUN rm -f /lib/systemd/system/sockets.target.wants/*initctl*; 
RUN rm -f /lib/systemd/system/basic.target.wants/*;
RUN rm -f /lib/systemd/system/anaconda.target.wants/*;

#Set ups a folder from the host in the container
VOLUME [ "/sys/fs/cgroup" ]
#Copies the WebServer's files
RUN mkdir -p /var/www/
RUN mkdir -p /usr/Server
COPY . /usr/Server
RUN cp /usr/Server/src/daemon /usr/bin/daemon
RUN cp /usr/Server/WebServer.service /etc/systemd/system/
RUN mv /usr/Server/Files /var/www/
