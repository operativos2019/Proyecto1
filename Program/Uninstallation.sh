FORKSERVER="webserverfork"
THREADSERVER = "webserverthread"
CONTAINERS="docker ps -a"
IMAGES="docker images"
STOP="docker stop"
REMOVE="docker rm"
REMOVEI = "docker rmi"
##UNINSTALLING FORK SERVER ######################################
#Stops and deletes instances of the container
$STOP $($CONTAINERS | awk '{ print $1,$2 }' | grep $FORKSERVER | awk '{print $1 }')  
$REMOVE $($CONTAINERS | awk '{ print $1,$2 }' | grep $FORKSERVER | awk '{print $1 }')

#Deletes the previous image version
$REMOVEI $($IMAGES | awk '{ print $1,$3 }' | grep $FORKSERVER | awk '{print $2}' ) 

#UNINSTALLING THREAD SERVER#########################
#Stops and deletes instances of the container
$STOP $($CONTAINERS | awk '{ print $1,$2 }' | grep $THREADSERVER | awk '{print $1 }')  
$REMOVE $($CONTAINERS | awk '{ print $1,$2 }' | grep $THREADSERVER | awk '{print $1 }')

#Deletes the previous image version
$REMOVEI $($IMAGES | awk '{ print $1,$3 }' | grep $THREADSERVER | awk '{print $2}' ) 