#include <ros/ros.h>
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
// #include <pcl/point_cloud>
#include <pcl/filters/passthrough.h>
#include <sensor_msgs/LaserScan.h>
#include <pcl/common/common.h>
#include <boost/foreach.hpp>



typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;


class SubscribeAndPublish
{
public:
  SubscribeAndPublish()
  {
    //Topic you want to publish
    pub_ = n_.advertise<PointCloud>("/published_filtered", 1);

    //Topic you want to subscribe
    sub_ = n_.subscribe("/camera/depth/points", 1, &SubscribeAndPublish::callback, this);
  }

  void callback(const PointCloud::ConstPtr &msg)
  {
    PointCloud output;



    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZ>);

    pcl::PassThrough<pcl::PointXYZ> pass;
    pass.setInputCloud(msg);

    pass.setFilterFieldName("y");
    pass.setFilterLimits(-0.5, -0.2);



    pass.filter(*cloud_filtered);



    
    // float minY = cloud_filtered->points[1].y;
    // float maxY = cloud_filtered->points[1].y;

    // pcl::PointXYZ minPt, maxPt;
    // pcl::getMinMax3D (*cloud_filtered, minPt, maxPt);


    // std::cout << "Max y: " << maxPt.y << std::endl;
    // for (auto it = cloud_filtered->points.begin(); it != cloud_filtered->points.end(); ++it) { 
    //     if ((*it).y > boundaries.maxX) { 
    //             boundaries.maxX = (*it).x; 
    //     } 
    //     if ((*it).x < boundaries.minX) { 
    //             boundaries.minX = (*it).x; 
    //     } 
    // }




    // std::cerr << "Cloud after filtering: " << std::endl;

    
    // for (const auto &point : *cloud_filtered){



    //     if (point.y<=minY){

    //         minY = point.y;

    //     }

    //     if( point.y >= maxY){

    //         maxY = point.y;
    //     }


    // }




        // std::cerr << "    " << point.x << " "
        //           << point.y << " "
        //           << point.z << std::endl;



    //.... do something with the input and generate the output...
    pub_.publish(cloud_filtered);
  }

private:
  ros::NodeHandle n_; 
  ros::Publisher pub_;
  ros::Subscriber sub_;

};//End of class SubscribeAndPublish



// void callback(const PointCloud::ConstPtr &msg)

// {
//     pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZ>);

//     pcl::PassThrough<pcl::PointXYZ> pass;
//     pass.setInputCloud(msg);

//     pass.setFilterFieldName("z");
//     pass.setFilterLimits(0.2, 0.25);

//     pass.filter(*cloud_filtered);

//     std::cerr << "Cloud after filtering: " << std::endl;
//     for (const auto &point : *cloud_filtered)
//         std::cerr << "    " << point.x << " "
//                   << point.y << " "
//                   << point.z << std::endl;

//     //     printf ("Cloud: width = %d, height = %d\n", msg->width, msg->height);
//     // BOOST_FOREACH (const pcl::PointXYZ& pt, msg->points)
//     //     printf ("\t(%f, %f, %f)\n", pt.x, pt.y, pt.z);
// }

int main(int argc, char **argv)
{
  //Initiate ROS
  ros::init(argc, argv, "filtered_cloud_publisher");

  //Create an object of class SubscribeAndPublish that will take care of everything
  SubscribeAndPublish SAPObject;

  ros::spin();

  return 0;
}