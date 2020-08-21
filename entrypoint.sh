#!/bin/bash

sh -c "echo 1000 > /sys/module/usbcore/parameters/usbfs_memory_mb"
source /opt/ros/melodic/setup.bash
source /usr/app/devel/setup.bash
mkdir /usr/app/log
echo "Staring ROS core..."
roscore &
sleep 3
echo "Launching Precision Landing..."
roslaunch volys_precision_landing precision_landing.launch log:=true disable_display:=true sim:=false &
sleep 3
echo "Launching Sensor Fusion..."
roslaunch volys_precision_landing sensor_fusion.launch sim:=false 