#!/bin/bash

apt update
apt install openjdk-11-jdk curlftpfs mysql-client ntpdate stow -y

mkdir -p ~/system ~/tmp/logs  /mnt/ftp

ssh-keygen -t rsa
cat ~/.ssh/id_rsa.pub

pushd /root/system/
curlftpfs -o rw,allow_other,nonempty ftp://guest:guest@192.168.50.191 /mnt/ftp/
cp /mnt/ftp/slam_lib/DLib/ . -avr
git clone ssh://git@192.168.50.191:222/sunhao/cppreview.git
git clone ssh://git@192.168.50.191:222/AroundI18RProject/I18RPublicBaseTypes.git
git clone ssh://git@192.168.50.191:222/AroundI18RProject/i18rconfig.git -b dev
git clone ssh://git@192.168.50.191:222/AroundI18RProject/i18rutilitysubmodule.git -b develop
popd

pushd ~/
git clone ssh://shiquan@192.168.50.90/home/shiquan/myscript -b indemind
popd

dd if=/dev/zero of=/swapfile bs=1M count=8888
mkswap /swapfile
swapon /swapfile

touch ~/.gitconfig
echo "export PATH="$PATH:~/myscript/sh" >> ~/.bashrc
echo "192.168.50.26   master-node build" >> /etc/hosts
