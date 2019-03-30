SERVERNAME="webserver"
CONTAINERS="docker ps -a"
IMAGES="docker images"

#Stops and deletes instances of the container
docker stop $($CONTAINERS | awk '{ print $1,$2 }' | grep $SERVERNAME | awk '{print $1 }')  
docker rm $($CONTAINERS | awk '{ print $1,$2 }' | grep $SERVERNAME | awk '{print $1 }')

#Deletes the previous image version
docker rmi $($IMAGES | awk '{ print $1,$3 }' | grep $SERVERNAME | awk '{print $2}' ) 
