#ifndef LIVOX_LIDAR_H
#define LIVOX_LIDAR_H

#include <ros/ros.h>
#include <ros/callback_queue.h>
#include <ros/advertise_options.h>

#include <sdf/Param.hh>

#include <gazebo/sensors/SensorTypes.hh>
#include <gazebo/plugins/RayPlugin.hh>

/**
 * Note: Sensor coordinate is defined as such: Forward is X, Left is Y, Up is Z.
*/
namespace gazebo {
  class LivoxLidar : public RayPlugin {
    public:
      LivoxLidar();
      ~LivoxLidar();

      /** Load and parse the sdf file. */
      void Load(sensors::SensorPtr _parent, sdf::ElementPtr _sdf);

    private:
      /** Callback when new ray scan is available. */
      void OnScan(const ConstLaserScanStampedPtr &_msg);

      ros::NodeHandle* _nh;
      /** Lidar message publisher */
      ros::Publisher _pub;
      /** Publish lidar topic name. */
      std::string _topic_name;
      std::string _frame_name;
      double _circular_fov;
      /** Pointer to parent. */
      sensors::RaySensorPtr _parent_ray_sensor;

      // Gazebo messaging.
      private: gazebo::transport::NodePtr _gazebo_node;
      gazebo::transport::SubscriberPtr _sub;
  };
}

#endif