FROM dorowu/ubuntu-desktop-lxde-vnc:bionic
LABEL maintainer="Kawata<70ayatakano@gmail.com>"

RUN apt-get update -q && \
    apt-get upgrade -yq && \
    apt-get install -yq wget curl git build-essential vim sudo lsb-release locales bash-completion tzdata gosu && \
    rm -rf /var/lib/apt/lists/*
RUN useradd --create-home --home-dir /home/ubuntu --shell /bin/bash --user-group --groups adm,sudo ubuntu && \
    echo ubuntu:ubuntu | chpasswd && \
    echo "ubuntu ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers
RUN git clone https://github.com/Tiryoh/ros_setup_scripts_ubuntu.git /tmp/ros_setup_scripts_ubuntu && \
    gosu ubuntu /tmp/ros_setup_scripts_ubuntu/ros-melodic-desktop.sh && \
    rm -rf /var/lib/apt/lists/*
RUN mkdir -p /home/ubuntu/ros_catkin_ws/src && \
	cd /home/ubuntu/ros_catkin_ws/src && \
	/bin/bash -c "source /opt/ros/melodic/setup.bash; catkin_init_workspace" && \
	cd /home/ubuntu/ros_catkin_ws && \
	/bin/bash -c "source /opt/ros/melodic/setup.bash; catkin_make" && \
	echo "source /opt/ros/melodic/setup.bash" >> ~/.bashrc && \
	echo "source /home/ubuntu/ros_catkin_ws/devel/setup.bash" >> ~/.bashrc && \
	echo "export ROS_PACKAGE_PATH=\${ROS_PACKAGE_PATH}:/home/ubuntu/ros_catkin_ws" >> ~/.bashrc && \
	echo "export ROS_WORKSPACE=/home/ubuntu/ros_catkin_ws" >> ~/.bashrc

RUN cd /home/ubuntu/ros_catkin_ws/src && \
    git clone https://github.com/dreamdrive/seto_scararobot.git
RUN cd /home/ubuntu/ros_catkin_ws && \
    /bin/bash -c "source /opt/ros/melodic/setup.bash; catkin_make" && \
    /bin/bash -c "source devel/setup.bash"
ENV USER ubuntu
