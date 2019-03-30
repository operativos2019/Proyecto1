######################################################################
#This file is used to install docker on a machine
######################################################################
#Remove old versions of docker 
apt-get remove docker docker-engine docker.io containerd runc 

#Install the dependencies
apt-get update

apt-get install \
    apt-transport-https \
    ca-certificates \
    curl \
    gnupg-agent \
    software-properties-common

#Add the docker's official GPG key
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -

#Makes sure it is using the stable repo not the testing ones
add-apt-repository \
   "deb [arch=amd64] https://download.docker.com/linux/ubuntu \
   $(lsb_release -cs) \
   stable"

#Actually install docker
apt-get update
apt-get install docker-ce docker-ce-cli containerd.io

#Makes docker run on boot
systemctl enable docker

#Install Gcc
apt -y install gcc