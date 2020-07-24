#include <livox_lidar_plugin.h>
#include <gazebo/physics/World.hh>
#include <gazebo/sensors/Sensor.hh>

#include <gazebo/sensors/RaySensor.hh>
#include <gazebo/common/Console.hh>
#include <gazebo/transport/TransportTypes.hh>
#include <gazebo/transport/Node.hh>

#include <sensor_msgs/PointCloud2.h>
#include <math.h>

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

    if (!_sdf->HasElement("frameName")) {
      ROS_INFO("Need to provide 'frameName' for the sensor plugin, default to 'livox_lidar'");
      _frame_name = "livox_lidar";
    }
    else {
      _frame_name = _sdf->GetElement("frameName")->Get<std::string>();
    }

    _parent_ray_sensor->SetActive(true);
    _sub = _gazebo_node->Subscribe(_parent_ray_sensor->Topic(), &LivoxLidar::OnScan, this);
    _pub = _nh->advertise<sensor_msgs::PointCloud2>(_topic_name, 5);
  }

  void LivoxLidar::OnScan(const ConstLaserScanStampedPtr &msg) {
    double angle_min = msg->scan().angle_min();
    double angle_max = msg->scan().angle_max();
    double angle_step = msg->scan().angle_step();
    double angle_range = fabs(angle_max - angle_min);

    double vertical_angle_min = msg->scan().vertical_angle_min();
    double vertical_angle_max = msg->scan().vertical_angle_max();
    double vertical_angle_step = msg->scan().vertical_angle_step();
    double vertical_angle_range = fabs(vertical_angle_max - vertical_angle_min);

    double range_min = msg->scan().range_min();
    double range_max = msg->scan().range_max();

    uint32_t count = msg->scan().count();
    uint32_t vertical_count = msg->scan().vertical_count();

    // gzdbg << "angle min: " << angle_min << " angle max: " << angle_max << " angle step" << angle_step << std::endl
    //   << "v angle min: " << vertical_angle_min << " v angle max: " << vertical_angle_max << " v angle step: " << vertical_angle_step << std::endl
    //   << "range min: " << range_min << " range max: " << range_max << " count: " << count << " vertical count: " << vertical_count << std::endl;

    // Pointcloud2 message to be published.
    const uint32_t POINT_STEP = 16;
    sensor_msgs::PointCloud2 pc_msg;
    pc_msg.header.frame_id = _frame_name;
    pc_msg.header.stamp = ros::Time(msg->time().sec(), msg->time().nsec());

    pc_msg.point_step = POINT_STEP;
    pc_msg.is_dense = false;

    const uint32_t NUM_FIELDS = 4;
    pc_msg.fields.resize(NUM_FIELDS);
    pc_msg.fields[0].name = "x";
    pc_msg.fields[0].offset = 0;
    pc_msg.fields[0].datatype = sensor_msgs::PointField::FLOAT32;
    pc_msg.fields[0].count = 1;
    pc_msg.fields[1].name = "y";
    pc_msg.fields[1].offset = 4;
    pc_msg.fields[1].datatype = sensor_msgs::PointField::FLOAT32;
    pc_msg.fields[1].count = 1;
    pc_msg.fields[2].name = "z";
    pc_msg.fields[2].offset = 8;
    pc_msg.fields[2].datatype = sensor_msgs::PointField::FLOAT32;
    pc_msg.fields[2].count = 1;
    pc_msg.fields[3].name = "intensity";
    pc_msg.fields[3].offset = 12;
    pc_msg.fields[3].datatype = sensor_msgs::PointField::FLOAT32;
    pc_msg.data.resize(count * vertical_count * POINT_STEP);

    float* data_ptr = (float*) pc_msg.data.data();
    uint32_t num_points = 0;
    for (int row = 0; row < vertical_count; row++) {
      for (int col = 0; col < count; col++) {
        int data_idx = row * count + col;
        
        float range = msg->scan().ranges(data_idx);
        float intensity = msg->scan().intensities(data_idx);

        if (isinf(range)) {
          continue;
        }

        float azimuth = col * angle_step - angle_range / 2.0;
        float elevation = row * vertical_angle_step - vertical_angle_range / 2.0;

        float range_z = range * sin(elevation);
        float range_xy = range * cos(elevation);
        float range_x = range_xy * cos(azimuth);
        float range_y = range_xy * sin(azimuth);

        data_ptr[NUM_FIELDS * num_points] = range_x;
        data_ptr[NUM_FIELDS * num_points + 1] = range_y;
        data_ptr[NUM_FIELDS * num_points + 2] = range_z;
        data_ptr[NUM_FIELDS * num_points + 3] = intensity;
  
        num_points++;
      }
    }
    pc_msg.width = num_points;
    pc_msg.row_step = num_points * POINT_STEP;
    pc_msg.height = 1;
    pc_msg.data.resize(num_points * POINT_STEP);
    
    _pub.publish(pc_msg);
  }
}