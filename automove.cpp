#include <iostream>
#include <ros/ros.h>
#include <stdio.h>
#include <sensor_msgs/LaserScan.h>
#include <nav_msgs/Odometry.h>

ros::Publisher operatorPublisher; //init publisher
ros::Subscriber scan_Subscriber; //init subscriber
ros::Subscriber odom_Subscriber; //init subscriber
float pos_z=0;
float wall=0;
float wall_rad=0;
int wall_ang=0;

void counterCallback(sensor_msgs::LaserScan scan_data){
	//ROS_INFO("(distance: %d)",wall_ang);
    ROS_INFO("(distance: %f)",scan_data.ranges[wall_ang]);
    wall = scan_data.ranges[wall_ang];
    geometry_msgs::Twist motor;
}
void odomCallback(nav_msgs::Odometry odom){
	//ROS_INFO("I received odom: %f", odom.pose.pose.orientation.z); 
    pos_z = odom.pose.pose.orientation.z;
	geometry_msgs::Twist motor;
}

int main(int argc, char** argv)
{
    ROS_INFO_STREAM("scanning...");
	ros::init(argc, argv, "automove");  //init node
	ros::NodeHandle nodeHandle;

	operatorPublisher = nodeHandle.advertise<geometry_msgs::Twist>("/cmd_vel",1);
	scan_Subscriber = nodeHandle.subscribe("/scan",1000,counterCallback);
	odom_Subscriber = nodeHandle.subscribe("/odom",1000,odomCallback);
    
    geometry_msgs::Twist motor;
    ros::Rate loopRate(10); 
    while(ros::ok()){
	        wall_rad = 1.5708-pos_z;
		    wall_ang = wall_rad*180/3.14;
        if(wall>0.15){
            motor.linear.x = 0.05;
            motor.angular.z = -0.4;
            operatorPublisher.publish(motor); 
        }
        else if(wall<0.15 && wall>0.14){
            motor.linear.x = 0.05;
            motor.angular.z = 0.0;
            operatorPublisher.publish(motor); 
        }
        else{
            motor.linear.x = 0.05;
            motor.angular.z = 0.4;
            operatorPublisher.publish(motor); 
        }
        ros::spinOnce();
        loopRate.sleep();
    }    
	return 0;
}