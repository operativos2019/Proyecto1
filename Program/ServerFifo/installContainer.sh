SERVERNAME="webserverfifo"
CONTAINERS="docker ps -a"
IMAGES="docker images"
#Stops and deletes previous instances of the container
docker stop $($CONTAINERS | awk '{ print $1,$2 }' | grep $SERVERNAME | awk '{print $1 }')  
docker rm $($CONTAINERS | awk '{ print $1,$2 }' | grep $SERVERNAME | awk '{print $1 }')  

#Deletes the previous image version
docker rmi $($IMAGES | awk '{ print $1,$3 }' | grep $SERVERNAME | awk '{print $2}' ) 

#Change to the executables folder
cd ServerFifo/

#Compiles daemon on host gcc environment
gcc -o src/daemon src/daemon.c

#Create the image
docker build -t=$SERVERNAME .

#Creates the running container
docker run -t -d -p 8001:8001 --privileged --security-opt seccomp:unconfined --cap-add=SYS_ADMIN -v /sys/fs/cgroup:/sys/fs/cgroup:ro -v /tmp/$(mktemp -d):/run $SERVERNAME

#executes the server 
docker exec $($CONTAINERS | awk '{ print $1,$2 }' | grep $SERVERNAME | awk '{print $1 }') systemctl enable WebServer.service
docker exec $($CONTAINERS | awk '{ print $1,$2 }' | grep $SERVERNAME | awk '{print $1 }') systemctl start WebServer.service
docker exec $($CONTAINERS | awk '{ print $1,$2 }' | grep $SERVERNAME | awk '{print $1 }') systemctl status WebServer.service

cd ../
