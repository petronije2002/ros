#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Float64.h"
#include "std_msgs/Int32.h"
#include "std_msgs/String.h"

#include <iostream>

/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */

class SubPub
{
public:
  SubPub()
  {
    // Topic you want to publish
    pub_ = n_.advertise<std_msgs::Float64>("/cmd_pos", 1);

    // Topic you want to subscribe
    sub_ = n_.subscribe("/key", 1, &SubPub::callback, this);
  }

  void callback(const std_msgs::Int32& input)
  {
    std_msgs::Float64 output;

    if (input.data == 105)
    {
      output.data = 1.5;

      pub_.publish(output);
    }
    else if (input.data == 107)
    {
      output.data = -1.5;

      pub_.publish(output);
    }

    // std::cout << input;
    //.... do something with the input and generate the output...
  }

private:
  ros::NodeHandle n_;
  ros::Publisher pub_;
  ros::Subscriber sub_;
};

// void chatterCallback(const std_msgs::Int32 msg)
// {

//   // std::string msgs = std::to_string(msg.data );

//   // ROS_INFO("I heard: [%s]",msg );

//   std::cout << msg;

// }

int main(int argc, char** argv)
{
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command
   * line.
   * For programmatic remappings you can use a different version of init() which
   * takes
   * remappings directly, but for most command-line programs, passing argc and
   * argv is
   * the easiest way to do it.  The third argument to init() is the name of the
   * node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */

  ros::init(argc, argv, "keyboard_list_node");
  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the
   * last
   * NodeHandle destructed will close down the node.
   */

  /**
   * The subscribe() call is how you tell ROS that you want to receive messages
   * on a given topic.  This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing.  Messages are passed to a callback function, here
   * called chatterCallback.  subscribe() returns a Subscriber object that you
   * must hold on to until you want to unsubscribe.  When all copies of the
   * Subscriber
   * object go out of scope, this callback will automatically be unsubscribed
   * from
   * this topic.
   *
   * The second parameter to the subscribe() function is the size of the message
   * queue.  If messages are arriving faster than they are being processed, this
   * is the number of messages that will be buffered up before beginning to
   * throw
   * away the oldest ones.
   */
  SubPub subpub;

  // subpub.sub_();

  /**
   * ros::spin() will enter a loop, pumping callbacks.  With this version, all
   * callbacks will be called from within this thread (the main one).
   * ros::spin()
   * will exit when Ctrl-C is pressed, or the node is shutdown by the master.
   */
  ros::spin();

  return 0;
}