FORKSERVER="webserverfork"
THREADSERVER="webserverthread"
FIFOSERVER="webserverfifo"
PREFORKSERVER="webserverprefork"
PRETHREADSERVER="webserverprethread"
CONTAINERS="docker ps -a"
IMAGES="docker images"
SILENCE="2>/dev/null"

#######REMOVING PREVIOUS CONTAINERS################
#Stops and deletes previous instances of the fifo container
docker stop $FIFOSERVER 2>/dev/null
docker rm $FIFOSERVER 2>/dev/null

#Stops and deletes previous instances of the fork container
docker stop $FORKSERVER 2>/dev/null
docker rm $FORKSERVER 2>/dev/null

#Stops and deletes previous instances of the thread container
docker stop $THREADSERVER 2>/dev/null
docker rm $THREADSERVER  2>/dev/null

#Stops and deletes previous instances of the prefork container
docker stop $PREFORKSERVER 2>/dev/null
docker rm $PREFORKSERVER 2>/dev/null
#Stops and deletes previous instances of the prethreaded container
docker stop $PRETHREADSERVER 2>/dev/null
docker rm $PRETHREADSERVER 2>/dev/null

#Deletes the previous fifo image version
docker rmi $FIFOSERVER 2>/dev/null

#Deletes the previous fork image version
docker rmi $FORKSERVER 2>/dev/null

#Deletes the previous thread image version
docker rmi $THREADSERVER 2>/dev/null

#Deletes the previous prefork image version
docker rmi $PREFORKSERVER 2>/dev/null

#Deletes the previous prethread image version
docker rmi $PRETHREADSERVER 2>/dev/null


######INSTALLING FIFO SERVER#######################################
#Change to the executables folder
cd ServerFifo/

#Compiles daemon on host gcc environment
gcc src/daemon.c -o src/daemon

#Create the image
docker build -t=$FIFOSERVER .

#Creates the running container
docker run --name $FIFOSERVER -t -d -p 8001:8001 --privileged --security-opt seccomp:unconfined --cap-add=SYS_ADMIN -v /sys/fs/cgroup:/sys/fs/cgroup:ro -v /tmp/$(mktemp -d):/run $FIFOSERVER

#executes the server 
docker exec $FIFOSERVER systemctl enable WebServer.service
docker exec $FIFOSERVER systemctl start WebServer.service
docker exec $FIFOSERVER systemctl status WebServer.service

cd ../
######INSTALLING FORK SERVER#######################################
#Change to the executables folder
cd ServerFork/

#Compiles daemon on host gcc environment
gcc src/daemon.c -o src/daemon

#Create the image
docker build -t=$FORKSERVER .

#Creates the running container
docker run --name $FORKSERVER -t -d -p 8003:8003 --privileged --security-opt seccomp:unconfined --cap-add=SYS_ADMIN -v /sys/fs/cgroup:/sys/fs/cgroup:ro -v /tmp/$(mktemp -d):/run $FORKSERVER

#executes the server 
docker exec $FORKSERVER systemctl enable WebServer.service
docker exec $FORKSERVER systemctl start WebServer.service
docker exec $FORKSERVER systemctl status WebServer.service
cd ../

###INSTALLING THREAD SERVER################################
#Change to the executables folder
cd ServerThread/

#Compiles daemon on host gcc environment
gcc -o mypthread.o -c ../myPthreads/mypthread.c
gcc src/daemon.c -o src/daemon mypthread.o

#Create the image
docker build -t=$THREADSERVER .

#Creates the running container
docker run --name $THREADSERVER -t -d -p 8005:8005 --privileged --security-opt seccomp:unconfined --cap-add=SYS_ADMIN -v /sys/fs/cgroup:/sys/fs/cgroup:ro -v /tmp/$(mktemp -d):/run $THREADSERVER

#executes the server 
docker exec $THREADSERVER systemctl enable WebServer.service
docker exec $THREADSERVER systemctl start WebServer.service
docker exec $THREADSERVER systemctl status WebServer.service
cd ../

# ######INSTALLING PRETHREAD SERVER#######################################
# #Change to the executables folder
# cd ServerPreThread/

# #Compiles daemon on host gcc environment
# gcc src/daemon.c -o src/daemon

# #Create the image
# docker build -t=$PRETHREADSERVER .

# #Creates the running container
# docker run --name $PRETHREADSERVER -t -d -p 8003:8003 --privileged --security-opt seccomp:unconfined --cap-add=SYS_ADMIN -v /sys/fs/cgroup:/sys/fs/cgroup:ro -v /tmp/$(mktemp -d):/run $PRETHREADSERVER

# #executes the server 
# docker exec $PRETHREADSERVER systemctl enable WebServer.service
# docker exec $PRETHREADSERVER systemctl start WebServer.service
# docker exec $PRETHREADSERVER systemctl status WebServer.service
# cd ../

# ######INSTALLING PREFORK SERVER#######################################
# #Change to the executables folder
# cd ServerPreFork/

# #Compiles daemon on host gcc environment
# gcc src/daemon.c -o src/daemon

# #Create the image
# docker build -t=$PREFORKSERVER .

# #Creates the running container
# docker run --name $PREFORKSERVER -t -d -p 8003:8003 --privileged --security-opt seccomp:unconfined --cap-add=SYS_ADMIN -v /sys/fs/cgroup:/sys/fs/cgroup:ro -v /tmp/$(mktemp -d):/run $PREFORKSERVER

# #executes the server 
# docker exec $PREFORKSERVER systemctl enable WebServer.service
# docker exec $PREFORKSERVER systemctl start WebServer.service
# docker exec $PREFORKSERVER systemctl status WebServer.service
# cd ../


