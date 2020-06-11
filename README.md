# CSC595VM2Docker

Instructions
Install docker by running DockerInstall.sh
Install all necessary installations for you test vm(chef server, large files, etc)
Navigate to vm2docker repository
Call ‘time docker build . -t ubuntu\firstrun’ to copy current vm into image
Call ‘docker run ubuntu\firstrun’ to load docker container
Plain Ubuntu Docker
Navigate to Basic Ububtu
For Ubuntu 14.04
Go to 14.04 folder and call ‘time docker build . -t ubuntu\14’
Call ‘docker run ubuntu\14’ to load docker container
For Ubuntu 18.04
Go to 18.04 folder and call ‘time docker build . -t ubuntu\18’
Call ‘docker run ubuntu\18’ to load docker container

Chef
Install Chef Server with InstallChefServer.sh (accept licensing when prompted)
Navigate to vm2docker repository
Call ‘time docker build . -t ubuntu\chef’ to copy chef server vm into image
Call ‘docker run ubuntu\chef’ to load docker container

Chef - Docker Hub
docker pull cbuisson/chef-server




