# Volans-i ROS workspace

Contains the ROS pipeline that runs on Volans-i companion computers.

## Install

Requires:

1.  [ROS Melodic](http://wiki.ros.org/melodic/Installation/Ubuntu) - install `ros-desktop-full` and `python-catkin-tools`;
1.  [AprilTag library](https://github.com/AprilRobotics/apriltag) - already included on this repository as `apriltag_lib`;
1.  [GeographicLib](https://geographiclib.sourceforge.io/):
```
#!bash
$ apt install libgeographic-dev
```
1.  [GeographicLib geoid datasets](https://geographiclib.sourceforge.io/):

```
#!bash
$ wget https://raw.githubusercontent.com/mavlink/mavros/master/mavros/scripts/install_geographiclib_datasets.sh
$ ./install_geographiclib_datasets.sh
```
1.  ROS `geographic_msgs`:
```
#!bash
$ apt install ros-$ROS_DISTRO-geographic-msgs
```

## Build

1.  Make sure your ROS environment is sourced (`source /opt/ros/melodic/setup.bash` should be on your `.bashrc`);
1.  To build:
```
#!bash
$ cd voly-ros_ws
$ catkin build
```
1.  To clean the build:
```
#!bash
$ catkin clean --all -y
```

## Run

_Note: Make sure to source the workspace after building it using `source <path_to_voly_ros_ws>/devel/setup.bash`._

### Precision landing pipeline

#### Simulation environment

To start PX4 SITL and spawn a vehicle with a moving fiducial marker for tracking and precision landing:

```
#!bash
$ roslaunch volys_precision_landing precision_landing.launch
```
