#include <ros/ros.h>
#include <ros/console.h>
// PCL specific includes
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


ros::Publisher pub;

tf::StampedTransform transform;

void cloud_callback (const sensor_msgs::PointCloud2ConstPtr& input)


{

    sensor_msgs::PointCloud2 in_ = *input;

    sensor_msgs::PointCloud2 out;

    sensor_msgs::PointCloud2 p;

    


    geometry_msgs::TransformStamped transformStamped;
    tf2_ros::Buffer buffer_;

    tf2_ros::TransformListener listener(buffer_);


    std::string parent_frame = "world";
    std::string child_frame = "camera_link";

    try{

        transformStamped = buffer_.lookupTransform( child_frame,parent_frame, ros::Time(0),ros::Duration(2));
        pcl_ros::transformPointCloud(tf2::transformToEigen(transformStamped.transform).matrix().cast<float>(),in_, p);

        ROS_INFO("%f", transformStamped.transform.translation.x);



      }
      catch (tf2::TransformException &ex) {
       ROS_WARN("%s",ex.what());
        ros::Duration(0.5).sleep();
      }

    



    
    pub.publish(p);

            

}

int main (int argc, char** argv){

    // Initialize ROS
    ROS_INFO("Node started");

    ros::init (argc, argv, "point_cloud_transform");
    ros::NodeHandle nh;


    

    // Create a ROS subscriber for the input point cloud
    ros::Subscriber sub = nh.subscribe ("/camera/depth/points", 1, cloud_callback);

    // Create a ROS publisher for the output point cloud
    pub = nh.advertise<sensor_msgs::PointCloud2> ("/point_cloud/cloud_transformed", 1);

    

    ros::Rate rate(10.0);
    while (nh.ok()){

       

        ros::spinOnce();
        rate.sleep();
    }

    ros::spin();
    return 0;

};
