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

Currently, by default, the horizontal velocity arguments for the moving platform (`helipad_x_vel` and `helipad_y_vel`) are set to 0, meaning that the platform is static.

#### Launch the robot_localization to do sensor fusion

After running the simulation pipeline, launch the sensor fusion using:

```
#!bash
$ roslaunch volys_precision_landing sensor_fusion.launch
```

#### Launch a simulation world with a map of a place loaded

The `google_static_map.world` world file located in the *precision_landing* package launches the Gazebo *StaticMapPlugin*, which loads a ground plane model with satellite images using the Google's Static Map API. For more info on the plugin, check http://gazebosim.org/tutorials?tut=static_map_plugin&cat=build_world.

To launch the world, on the `precision_landing.launch` file, replace the default world arg with `google_static_map.world`.

_Note: To use this plugin, Gazebo 9.10 or greater is required. To install it, please follow the instructions in http://gazebosim.org/tutorials?tut=install_ubuntu&cat=install documentation._

_Note: if one gets an error like `StaticMapPlugin: boost::filesystem::rename: Invalid cross-device link: "/tmp/<model_name>", "/home/.../.gazebo/models/<model_name>"` after launching the world, means that your `/temp` dir is in a different partition than `~/.gazebo`. A fix was done in https://bitbucket.org/osrf/gazebo/pull-requests/3123/fix-moving-model-files-in-staticmapplugin/diff. While it doesn't get merged and released, one can manually copy the contents of the created /temp/<model_name> to ~/.gazebo/models/<model_name> and set <use_cache>true</use_cache>. Note also that there's already a `buchanan_field_airport.world` than can be used as an example._

### Visual odometry pipeline

The chosen visual odometry method is using a monocular camera and [ORB-SLAM2](https://arxiv.org/abs/1610.06475). The node is launched with the following:

```
#!bash
$ roslaunch volys_precision_landing visual_odometry.launch
```

Some remarks:

1.  The visual odometry system requires some visual map points to be retrieved for initialization (one will be able to see `Map point vector is empty!` message after launching the node). That's because the monocular SLAM requires some initialization. Part of the initialization method is to move the camera around, until the system creates a map (a message like `New Map created with ... points` will appear);
1.  Only after the the initial map is created that the ORB dictionary is able to be applied on the SLAM system;
1.  In simulation, a warehouse world was loaded so it is able to retrieve enough features of the environment to initialize SLAM faster;
1.  Point 4 should be replace with a map texture and the camera then will be pointing downwards.

To use the visual odometry data inside the Flight Controller, the `EKF2_AID_MASK` PX4 parameter should be set to, at least, 24 (25 will mean that the GPS will also be used). The data is propagated from the ORB-SLAM2 topic to the `vision_pose_estimate` plugin in MAVROS and from it to the FCU thtough the `VISION_POSITION_ESTIMATE` Mavlink message.

_Note: Should be considered a stereo system for stereo visual odometry._
