#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include <kobuki_msgs/CliffEvent.h>

geometry_msgs::Twist vmsg;
ros::Publisher cmd_vel_pub;
double sensor;
double state;

void publishVel(){
  while(ros::ok()){
    cmd_vel_pub.publish(vmsg);
    ros::Duration(0.01).sleep();
    ros::spinOnce();
  }
}

void cliffCallback(const kobuki_msgs::CliffEvent::ConstPtr& msg){
  sensor = msg->sensor;
  state = msg->state;
  ROS_INFO("sensor: %f, state: %f", sensor, state);
  while(ros::ok()){
  if(sensor == 1 && state == 1){
    vmsg.linear.x = -0.17;
    vmsg.angular.z = 1.2;
    cmd_vel_pub.publish(vmsg);
      ros::Duration(1).sleep();
      ros::spinOnce();
    ROS_INFO("Cliff infront");
  }
  else if(sensor == 0 && state == 1){
    vmsg.linear.x = -0.13;
    vmsg.angular.z = -1.4;
    cmd_vel_pub.publish(vmsg);
      ros::Duration(1).sleep();
      ros::spinOnce();
    ROS_INFO("Cliff on left");
  }
  else if(sensor == 2 && state == 1){
    vmsg.linear.x = -0.13;
    vmsg.angular.z = 1.4;
    cmd_vel_pub.publish(vmsg);
      ros::Duration(1).sleep();
      ros::spinOnce();
    ROS_INFO("Cliff on right");
  }
  vmsg.linear.x = 0.17;
  vmsg.angular.z = 0.0;
  ROS_INFO("Driving forward");
  cmd_vel_pub.publish(vmsg);
      ros::Duration(0.1).sleep();
      ros::spinOnce();
}
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "cliff");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("/mobile_base/events/cliff", 1, cliffCallback);
  cmd_vel_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel_mux/input/navi", 1);

  vmsg.linear.x = 0.17;
  vmsg.angular.z = 0.0;
  publishVel();


  return 0;
}
