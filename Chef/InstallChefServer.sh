sudo apt update -y && sudo apt upgrade -y
wget https://packages.chef.io/files/stable/chef-server/13.1.13/ubuntu/18.04/chef-server-core_13.1.13-1_amd64.deb
sudo dpkg -i chef-server-core_*.deb 
rm chef-server-core_*.deb
sudo chef-server-ctl reconfigure
mkdir .chef

