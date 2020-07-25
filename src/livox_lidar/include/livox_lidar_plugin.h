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

      /** Phase shift of the scanning pattern. */
      double _total_phase_shift;

      /** Parameters for scanning pattern (flower pattern). */
      /** Number of unique patterns (or flower petals) per 360 deg rotation. */
      static const unsigned int N = 5;
      /** A magic number for phase shift, such that the pattern will never overlap. */
      const double PHASE_SHIFT;
      static constexpr double ANGULAR_RESOLUTION = 0.001;
      /** Number of complete 360 deg scan per pointcloud message. (or number of flowers per message) */
      static const unsigned int NUM_SCAN_PER_MSG = 2;
  };
}

#endif