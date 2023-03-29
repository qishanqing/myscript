#!/bin/bash

apt update
apt install unzip openjdk-11-jdk lftp curlftpfs rsync mysql-client ntpdate stow libucl-dev libglew-dev glew-utils -y

mkdir -p ~/system ~/tmp/logs  /mnt/ftp

ssh-keygen -t rsa
cat ~/.ssh/id_rsa.pub
systemctl enable systemd-timesyncd

pushd /root/system/
curlftpfs -o rw,allow_other,nonempty ftp://guest:guest@192.168.50.191 /mnt/ftp/
cp /mnt/ftp/slam_lib/DLib/ . -avr
(
    cp /mnt/ftp/slam_ThirdParty/Pangolin.zip .
    cd Pangolin
    mkdir build && cd build
    cmake ..
    cmake --build .
    make -j4
    make install
    mkdir -p /usr/local/slam
    cd /usr/local/slam
    scp -r  root@192.168.50.142:/usr/local/slam/Thirdparty .
)

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
添加到vim /etc/rc.local 之前exit 0
sudo mkswap -f /swapfile && sudo swapon /swapfile


touch ~/.gitconfig
echo export PATH="$PATH:~/myscript/sh" >> ~/.bashrc
echo "192.168.50.191   master-node build" >> /etc/hosts
