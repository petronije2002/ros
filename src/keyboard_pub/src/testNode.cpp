#include "ros/ros.h"
#include "geometry_msgs/PointStamped.h"

#include "tf2_ros/transform_listener.h"
#include "tf2_ros/message_filter.h"
#include "message_filters/subscriber.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"

#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/ModelCoefficients.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl_ros/transforms.h>
#include <tf2_eigen/tf2_eigen.h>


class PoseDrawer
{
public:
  PoseDrawer() :
    tf2_(buffer_),  target_frame_("world"),
    tf2_filter_(point_cloud_sub_, buffer_, target_frame_, 10, 0)
  {
    point_cloud_sub_.subscribe(n_, "/camera/depth/point", 10);

    tf2_filter_.registerCallback( boost::bind(&PoseDrawer::msgCallback, this, _1) );
  }

  //  Callback to register with tf2_ros::MessageFilter to be called when transforms are available
  void msgCallback(const sensor_msgs::PointCloud2ConstPtr& input) 
  {
    sensor_msgs::PointCloud2 point_out;
    try 
    {
      point_out = buffer_.transform(*input, point_out, target_frame_,ros::Duration(1.2));

    //   pcl_ros::transformPointCloud(tf2::transformToEigen(point_out.data..transform).matrix().cast<float>(),in_, p);

         publ_.publish(point_out);
      
    }
    catch (tf2::TransformException &ex) 
    {
      ROS_WARN("Failure %s\n", ex.what()); //Print exception which was caught
    }
  }

private:
  std::string target_frame_;
  tf2_ros::Buffer buffer_;
  tf2_ros::TransformListener tf2_;
  ros::NodeHandle n_;


  message_filters::Subscriber<sensor_msgs::PointCloud2> point_cloud_sub_;
  tf2_ros::MessageFilter<sensor_msgs::PointCloud2> tf2_filter_;


  ros::Publisher publ_ = n_.advertise<sensor_msgs::PointCloud2> ("/cloud_transformed", 1); 


};


int main(int argc, char ** argv)
{
  ros::init(argc, argv, "pose_drawer"); //Init ROS
  PoseDrawer pd; //Construct class
  ros::spin(); // Run until interupted 
  return 0;
};