#include <livox_lidar_plugin.h>
#include <gazebo/physics/World.hh>
#include <gazebo/sensors/Sensor.hh>

#include <gazebo/sensors/RaySensor.hh>
#include <gazebo/common/Console.hh>
#include <gazebo/transport/TransportTypes.hh>
#include <gazebo/transport/Node.hh>

#include <sensor_msgs/PointCloud2.h>

namespace gazebo {
  GZ_REGISTER_SENSOR_PLUGIN(LivoxLidar)

  LivoxLidar::LivoxLidar() {
    int argc;
    ros::init(argc, NULL, "livox_lidar_plugin_node");
    _nh = new ros::NodeHandle();
  }

  LivoxLidar::~LivoxLidar() {
    if (_nh) {
      _nh->shutdown();
      delete _nh;
    }
  }

  void LivoxLidar::Load(sensors::SensorPtr _parent, sdf::ElementPtr _sdf) {
    if (!ros::isInitialized()) {
      gzthrow("Ros need to be initialized.");
      return;
    }
    // Invoke parent load function.
    RayPlugin::Load(_parent, _sdf);

    // Initialize gazebo node.
    _gazebo_node = gazebo::transport::NodePtr(new gazebo::transport::Node());
    _gazebo_node->Init();

    _parent_ray_sensor = std::dynamic_pointer_cast<sensors::RaySensor>(_parent);
    if (!_parent_ray_sensor) {
      gzthrow("Livox lidar plugin requires a RaySensor as it's parent.");
    }

    // Parsing SDF file.
    if (!_sdf->HasElement("topicName")) {
      ROS_INFO("Need to provide 'topicName' for the sensor plugin, default to /livox_lidar/points");
      _topic_name = "/livox_lidar/points";
    }
    else {
      _topic_name = _sdf->GetElement("topicName")->Get<std::string>();
    }

    _parent_ray_sensor->SetActive(true);
    _sub = _gazebo_node->Subscribe(_parent_ray_sensor->Topic(), &LivoxLidar::OnScan, this);
    _pub = _nh->advertise<sensor_msgs::PointCloud2>(_topic_name, 5);
  }

  void LivoxLidar::OnScan(const ConstLaserScanStampedPtr &msg) {
    double angle_min = msg->scan().angle_min();
    double angle_max = msg->scan().angle_max();
    double angle_step = msg->scan().angle_step();

    double vertical_angle_min = msg->scan().vertical_angle_min();
    double vertical_angle_max = msg->scan().vertical_angle_max();
    double vertical_angle_step = msg->scan().vertical_angle_step();

    double range_min = msg->scan().range_min();
    double range_max = msg->scan().range_max();

    int count = msg->scan().count();
    int vertical_count = msg->scan().vertical_count();

    gzdbg << "angle min: " << angle_min << " angle max: " << angle_max << " angle step" << angle_step << std::endl
      << "v angle min: " << vertical_angle_min << " v angle max: " << vertical_angle_max << " v angle step: " << vertical_angle_step << std::endl
      << "range min: " << range_min << " range max: " << range_max << " count: " << count << " vertical count: " << vertical_count << std::endl;
  }
}