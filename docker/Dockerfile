## 设置继承自ubuntu官方镜像
FROM qsq/ubuntu:ssh

MAINTAINER qsq

ENV DEBIAN_FRONTEND noninteractive
#RUN sudo rm -rf /var/lib/apt/lists/*
#RUN sudo apt-get update

RUN sudo apt-get install -y vim wget curl openssh-server sudo && \

    sudo mkdir -p /var/run/sshd && \
    sed -i 's/UsePAM yes/UsePAM no/g' /etc/ssh/sshd_config && \
    useradd admin && \
    echo "admin:admin" | chpasswd && \
    echo "admin   ALL=(ALL)       ALL" >> /etc/sudoers && \
    usermod -s /bin/bash admin && \
    sudo apt-get install -y supervisor && \
    sudo mkdir -p /var/log/supervisor
    
COPY supervisord.conf /etc/supervisor/conf.d/supervisord.conf

EXPOSE 22

CMD ["/usr/bin/supervisord"]
