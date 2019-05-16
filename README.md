# Volans-i ROS workspace

Contains the ROS pipeline that runs on Volans-i companion computers.

## Install

Requires:
1. [ROS Melodic](http://wiki.ros.org/melodic/Installation/Ubuntu) - install `ros-desktop-full` and `python-catkin-tools`;
2. [AprilTag library](https://github.com/AprilRobotics/apriltag) - already included on this repository as `apriltag_lib`;
3. [GeographicLib](https://geographiclib.sourceforge.io/):
    ```sh
    apt install libgeographic-dev`
    ```
4. [GeographicLib geoid datasets](https://geographiclib.sourceforge.io/):
    ```sh
    $ wget https://raw.githubusercontent.com/mavlink/mavros/master/mavros/scripts/install_geographiclib_datasets.sh
    $ ./install_geographiclib_datasets.sh
    ```
5. ROS `geographic_msgs`:
    ```sh
    apt install ros-$ROS_DISTRO-geographic-msgs
    ```

## Build

1. Make sure your ROS environment is sourced (`source /opt/ros/melodic/setup.bash` should be on your `.bashrc`);
2. To build:
    ```sh
    $ cd voly-ros_ws
    $ catkin build
    ```
3. To clean the build:
    ```sh
    $ catkin clean --all -y
    ```

## Run

*Note:* Make sure to source the workspace after building it using `source <path_to_voly_ros_ws>/devel/setup.bash`.

### Precision landing pipeline

#### Simulation environment

To start PX4 SITL and spawn a vehicle with a moving fiducial marker for tracking and precision landing:

```sh
roslaunch volys_precision_landing precision_landing.launch
```
