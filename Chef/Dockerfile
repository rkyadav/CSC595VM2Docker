FROM ubuntu:18.04
RUN apt-get update
RUN sudo apt update -y && sudo apt upgrade -y
RUN wget https://packages.chef.io/files/stable/chef-server/13.1.13/ubuntu/18.04/chef-server-core_13.1.13-1_amd64.deb
RUN sudo dpkg -i chef-server-core_*.deb 
RUN rm chef-server-core_*.deb
RUN sudo chef-server-ctl reconfigure
RUN mkdir .chef
RUN sudo chef-server-ctl user-create ryadav Rakesh Yadav 'ryadav@test.com' 'PASSWORD' --filename ~/.chef/ryadav.pem
RUN sudo chef-server-ctl org-create ryadavinc "Rakesh Yadav Inc" --association_user ryadav --filename ~/.chef/ryadavinc.pem