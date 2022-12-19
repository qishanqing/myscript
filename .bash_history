docker ps
docker images
exit
ls
pwd
cd /var/lib/jenkins/
ls -al
cd jobs/
ls -al
history
exit
pwd
exit
ssh indemind@192.168.50.158
ssh indemind@192.168.50.158 ls
ssh root@192.168.50.158 ls
cat ~/.ssh/known_hosts 
ll ~/.ssh/
cd .ssh/
ls
ls -al
exit
ll
ls
cd .ssh/
ll
ls
cat known_hosts 
cd /var/lib/jenkins/
ls
hostname
ssh-keygen -t rsa 
ll
ls
cd .ssh/
ls
cat id_rsa.pub 
ssh root@192.168.50.158 ls
ls
ll
ls -al
chmod chmod 700 id_rsa
chmod 700 id_rsa
ll
ls
ls -al
chmod 755 id_rsa
ls -al
cat known_hosts 
ls
ls -al
ls
vim authorized_keys
ls
ls -al
ssh root@192.168.50.158 ls
rm -f authorized_keys 
ssh root@192.168.50.158 ls
ll
ls -al
cat known_hosts 
ll
ssh root@192.168.50.158 ls
cat /var/lib/jenkins/.ssh/id_rsa
cat /var/lib/jenkins/.ssh/id_rsa.pub 
chmod 600  /var/lib/jenkins/.ssh/id_rsa
ssh root@192.168.50.158 ls
ssh root@192.168.50.158 ls
ssh root@192.168.50.158 ls /
ssh root@192.168.50.158 ls /backup/
ssh root@192.168.50.158 ls /backup/gitlab
ssh root@192.168.50.158 ls /backup/gitlab/backup
ssh root@192.168.50.158 ls /backup/gitlab/
ssh root@192.168.50.158 ls /backup/gitlab/git-data
ssh root@192.168.50.158 ls /backup/gitlab/backups
ssh indemind@192.168.50.158 ls /backup/gitlab/backups
ssh indemind@192.168.50.158 ls /backup/gitlab/backups
ssh root@192.168.50.158 ls /backup/gitlab/backups
exit
localhost
hostname
exit
ll
cd /home/indemind/devops/gitlab/data/backups/
ll /home/indemind/devops/gitlab/data/backups/
ls /home/indemind/devops/gitlab/data/backups/
ll
ls
exit
ll
ls -al
ls
cd
ls
pwd
ll
ls -al
exit
docker
ll
ll /home/
ls -al /home/
ls -al /home/gerrit/
exit
ls
ll /backup/jira/
ls /backup/jira/
df -h
cd /backup/
ll
ls -al
ll jira/
ls -al jira/
exit
pwd
ls
ll /var/lib/jenkins/
ll /var/lib/jenkins
ls  /var/lib/jenkins
touch /var/lib/jenkins/rules
vim  rules 
ls /backup/gitlab/
ssh -p 222 root@192.168.50.158 ls
cat ~/.ssh/id_rsa.pub 
ssh -p 222 root@192.168.50.158 ls
exit
ls
date
cat /etc/issue
hwclock/clock 
tzselect
ls
date
ls -al  /etc/localtime 
rm /etc/localtime 
sudo rm /etc/localtime 
exit
ls
date
vim /etc/timezone 
sudo su -
sudo su
cd
ls
exit
su
exit
exit
ls
cd
ls
ls -al
df -h
exit
ls
df -hT
date
date
ls
ls home/
ls
cd
ls
pwd
ls
pwd
whoami
pwd
ls /
mkdir /backup 
ll
ls -al /
pwd
ls
pwd
mkdir backup
cd backup/
ll
ls -al
pwd
ll
ls -al
ls -al
ls -al
du -sh FULL-2021-02-01_17-11/
du -sh FULL-2021-02-01_17-11/
du -sh FULL-2021-02-01_17-11/
du -sh FULL-2021-02-01_17-11/
du -sh FULL-2021-02-01_17-11/
ls -al  FULL-2021-02-01_17-11/
exit
ls
cd
hostname 
cd var
pwd
cd backup/
ll
ls
ls -al
du -sh *
exit
ls
cd /var/jenkins_home/jobs/cr/workspace
ll
ls
docker 
docker ps
ls
ls
exit
ls /usr/bin/
ls /var/run/
exit
ls
su -
su -
docker 
exit
docker ps
docker images
exit
docker run  --name=tt --user=root -v      args '-v /home/indemind/workspace:/code -v /home/jenkins/jenkins_home/jobs:/jobs  -it 
docker run  --name=tt --user=root -v /home/indemind/workspace:/code -v /home/jenkins/jenkins_home/jobs:/jobs  -it 
docker run -it  --name=tt --user=root -v /home/indemind/workspace:/code -v /home/jenkins/jenkins_home/jobs:/jobs compile_x64:18.04 
docker run -it  --name=tt --user=root -v /home/indemind/workspace:/code -v /home/jenkins/jenkins_home/jobs:/jobs compile_x64:18.04 bash
docker run   --name=tt --user=root -v /home/indemind/workspace:/code -v /home/jenkins/jenkins_home/jobs:/jobs compile_x64:18.04 bash
docker run -t  --name=tt --user=root -v /home/indemind/workspace:/code -v /home/jenkins/jenkins_home/jobs:/jobs compile_x64:18.04 
docker run -t  --name=tt --user=root -v /home/indemind/workspace:/code -v /home/jenkins/jenkins_home/jobs:/jobs --detach=true compile_x64:18.04 
docker run -t  --name=tt --user=root -v /home/indemind/workspace:/code -v /home/jenkins/jenkins_home/jobs:/jobs --detach=true compile_x64:18.04 bash
docker run -it  --name=tt --user=root -v /home/indemind/workspace:/code -v /home/jenkins/jenkins_home/jobs:/jobs --detach=true compile_x64:18.04 bash
docker ps
docker ps -a
docker rm -f tt
docker images
ifconfig
docker images
docker inspect -f . compile_x64:18.04
docker pull compile_x64:18.04
docker inspect -f . compile_x64:18.04
exit
docker ps
docker ps
exit
ls
date
date
pwd
cd /var/jenkins_home/
ll
ls
pwd
ls
ll ~/
ls ~/
pwd
cd ~/
pwd
exit
cd /var/jenkins_home/
ls
ll
ls -la
cat copy_reference_file.log 
rm -f copy_reference_file.log 
which lftp
lftp
ls /mnt/ftp/
exit
cp .dockerenv  1
ls
ll
ls -;a
ls -la
cp  ~/thinBackup.xml 1
cd ~/
pwd
ll
ls
cd /home/indemind/myscript/
ls -la
cp sh/s .
ls
mv s /mnt/ftp/release/code_style_report/20210729/
git status
git branch -av
git remote -av
git remote -v
git pull --rebase github indemind
git pull --rebase github indemind
cat ~/.ssh/id_rsa.pub 
git pull --rebase github indemind
vi build/build.sh 
vim build/build.sh 
exit
ll
ls
cd ~/tmp/logs/
ll
ls
ls -la
cat cmdb.err.txt 
sql
cat /etc/os-release 
sudo apt install mysql-client5
apt install mysql-client5
exit
ssh-keygen -t rsa
cd /var/jenkins_home/.ssh/
ll
ls -la
cat id_rsa.pub 
git config -l
git clone ssh://git@192.168.50.191:222/abby/source/sdk_plubins -b master --depth=1
ll
ls -la
rm -rf sdk_plubins/
exit
docker
docker exec -i c3566 /bin/bash
sudo docker exec -i c3566 /bin/bash
docker exec -i c3566 /bin/bash
docker ps
docker 
docker ps
ls -la /var/run
ls -la /var/run/docker.sock 
exit
docker ps
exit
cd ~/.gitconfig 
vim ~/.gitconfig 
cd ~/jobs/rubby-sdk/
ll
ls
cd workspace
ls
cd sdk_plubins/
ll
ls
cd /var/jenkins_home/jobs/rubby-sdk/workspace/sdk_plubins/
ll
ls
ll
ls
ll -a
ls la
ls -la
ls -la
cd -
cd -
ls
ls -la
git submodule update --init --recursive
mkdir -p /home/jenkins/jenkins_home/
sudo mkdir -p /home/jenkins/jenkins_home/
mkdir -p /home/jenkins/jenkins_home/
exit
cat ~/.gitconfig 
cd /var/jenkins_home/jobs/rubby405-embeded/workspace/rubby_sensor_p
ls
ls
cd /var/jenkins_home/jobs/rubby405-embeded/workspace/rubby_sensor_p/
ls
ls -a
ls -al
exit
ll
ls
date
exit
