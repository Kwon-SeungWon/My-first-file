#include <iostream>
#include <ros/ros.h>
#include <stdio.h>
#include <sensor_msgs/LaserScan.h>
#include <nav_msgs/Odometry.h>

ros::Publisher operatorPublisher; //init publisher
ros::Subscriber scan_Subscriber; //init subscriber
ros::Subscriber odom_Subscriber; //init subscriber
float pos_z;

void counterCallback(sensor_msgs::LaserScan scan_data){
    ROS_INFO("(Front: %f), (Back: %f), (Left: %f), (Right: %f)",scan_data.ranges[180],scan_data.ranges[0],scan_data.ranges[270],scan_data.ranges[90]);
    geometry_msgs::Twist motor;
}
void odomCallback(nav_msgs::Odometry odom){
		ROS_INFO("I received odom: %f", odom.pose.pose.orientation.z); 
    pos_z = odom.pose.pose.orientation.z;
		geometry_msgs::Twist motor;
    
}



int main(int argc, char** argv)
{
    ROS_INFO_STREAM("scanning...");
	  ros::init(argc, argv, "odom");  //init node
	  ros::NodeHandle nodeHandle;
	  operatorPublisher = nodeHandle.advertise<geometry_msgs::Twist>("/cmd_vel",1);
	  scan_Subscriber = nodeHandle.subscribe("/scan",1000,counterCallback);
		odom_Subscriber = nodeHandle.subscribe("/odom",1000,odomCallback);

    geometry_msgs::Twist motor;
    ros::Rate loopRate(10); 
    while(ros::ok()){
        if(pos_z>0.08){ 
            motor.linear.x = 0.0;
            motor.angular.z = -0.3;
            operatorPublisher.publish(motor);
        }
        else if(pos_z<-0.08){
            motor.linear.x = 0.0;
            motor.angular.z = 0.3;
            operatorPublisher.publish(motor);
        }
        else{
          motor.linear.x = 0.0;
          motor.angular.z = 0;
          operatorPublisher.publish(motor);
        }
        //init the ROS node
        ros::spinOnce();
        loopRate.sleep();
        }
	  return 0;
}