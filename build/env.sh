#!/bin/bash


apt update
apt install unzip expect ftp openjdk-11-jdk lftp curl  curlftpfs rsync mysql-client ntpdate stow libucl-dev libglew-dev glew-utils qtbase5-private-dev -y

mkdir -p ~/system ~/tmp/logs  /mnt/ftp

ssh-keygen -t rsa
cat ~/.ssh/id_rsa.pub
systemctl enable systemd-timesyncd

pushd /root/system/
curlftpfs -o rw,allow_other,nonempty ftp://guest:guest@192.168.50.191 /mnt/ftp/
git clone ssh://git@192.168.50.191:222/AroundI18RProject/i18rutilitysubmodule.git -b develop
cp -r /mnt/ftp/tools/oss  .
cp -r /mnt/ftp/tools/mqtt  .
pushd oss
bash -ex oss_install.sh
popd

pushd mqtt
bash -ex mqtt_install.sh
popd
   
cp /mnt/ftp/slam_lib/DLib/ . -avr
(
    cp /mnt/ftp/slam_ThirdParty/Pangolin.zip .
    unzip  Pangolin.zip
    cd Pangolin
    mkdir build && cd build
    cmake ..
    cmake --build .
    make -j4
    make install
    mkdir -p /usr/local/slam
    cd /usr/local/slam
    scp -r  root@192.168.50.196:/usr/local/slam/Thirdparty .
)
popd

pushd ~/
git clone ssh://shiquan@192.168.50.90/home/shiquan/myscript -b indemind
popd

dd if=/dev/zero of=/swapfile bs=1M count=8888
mkswap /swapfile
swapon /swapfile
添加到vim /etc/rc.local 之前exit 0
mkswap -f /swapfile && swapon /swapfile


touch ~/.gitconfig
[user]
	name = JENKINS
	email = jenkins@indemind.cn
[credential]
	helper = store

echo export PATH="$PATH:~/myscript/sh" >> ~/.bashrc
echo "192.168.50.191   master-node build" >> /etc/hosts

手动下载一个仓库，记录用户名密码，新环境替换libm.so.6
