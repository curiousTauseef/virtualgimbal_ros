#ifndef __VIRTUALGIMAL_ROS_VG_MANAGER_H__
#define __VIRTUALGIMAL_ROS_VG_MANAGER_H__

#include "ros/ros.h"
#include "sensor_msgs/Image.h"
#include "sensor_msgs/CameraInfo.h"
#include "sensor_msgs/Imu.h"
#include "std_msgs/Float64.h"
#include "rotation.h"
#include <cv.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <image_transport/image_transport.h>
#include <Eigen/Dense>
#include "param.h"
#include "camera_information.h"
#include "cl_manager.h"
#include "SO3Filters.h"



namespace virtualgimbal
{

using MatrixPtr = std::shared_ptr<std::vector<float>>;

using Rotation = StampedDeque<Eigen::Quaterniond>;
using Image = StampedDeque<cv::UMat>;
using UMatPtr = std::unique_ptr<cv::UMat>;

class manager
{
public:
    manager();
    ~manager();
    void callback(const sensor_msgs::ImageConstPtr& image, const sensor_msgs::CameraInfoConstPtr& ros_camera_info);
    void imu_callback(const sensor_msgs::Imu::ConstPtr &msg);
    void run();
private:
    ros::NodeHandle nh_;
    ros::NodeHandle pnh_;
    image_transport::ImageTransport image_transport_;
    image_transport::CameraSubscriber camera_subscriber_;
    image_transport::CameraPublisher camera_publisher_;
    ros::Subscriber imu_subscriber_;
    image_transport::Publisher pub_ ;
    Eigen::Quaterniond q,q_filtered;
    sensor_msgs::Imu::ConstPtr imu_previous = nullptr;
    sensor_msgs::ImageConstPtr image_previous = nullptr;
    ros::Publisher raw_quaternion_pub,filtered_quaternion_pub,estimated_angular_velocity_pub,measured_augular_velocity_pub,
    raw_quaternion_queue_size_pub,filtered_quaternion_queue_size_pub;  
    Eigen::Vector3d last_vector;
    MatrixPtr getR(double ratio=1.0);
    ros::Time get_begin_time(ros::Time time);
    ros::Time get_end_time(ros::Time time);

    Rotation raw_angle_quaternion;
    Rotation filtered_angle_quaternion;

    Image src_image;

    Parameters param; 
    CameraInformationPtr camera_info_;
    CameraInformationPtr dst_camera_info_;
    sensor_msgs::CameraInfoConstPtr ros_camera_info_;

    // Image processing parameters
    float zoom_;
    bool enable_black_space_removal_;
    float cutoff_frequency_;
    float a1_;
    bool enable_trimming_;
    double offset_time_;

    // Prepare OpenCL
    cv::ocl::Context context;
    cv::String build_opt;

    // Debug
    bool publish_statistics;
};

} // namespace virtualgimbal

#endif //__VIRTUALGIMAL_ROS_VG_MANAGER_H__