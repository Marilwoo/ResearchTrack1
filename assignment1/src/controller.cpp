#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"
#include <math.h>
#include "assignment1/target.h"
#include "assignment1/tar.h"

//This is a process that calls a Service to receive a random target for the 
//robot and makes the robot reach it. It includes two publishers, one subscriber
//and a client.

ros::Publisher pub;
ros::Publisher pub2;

ros::ServiceClient client;

ros::Subscriber sub;

double tar_x = 0, tar_y = 0;

//This function is the callback for the subscriber, when the robot has reached
//the target it calls the service for assigning the new target giving it the min
//and max values for the possible new coordinates. Then it sets the velocity,
//using the cmd_vel, in function of the distance between the target and the
// actual position of the robot, which is given by the odom topic.

void positionCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
	//ROS_INFO("Robot position [%f, %f]", msg->pose.pose.position.x, msg->pose.pose.position.y);
	
	float dist = sqrt(pow((msg->pose.pose.position.x - tar_x), 2)+ pow((msg->pose.pose.position.y - tar_y), 2));
	
	if (dist < 0.1)
	{ 		
		assignment1::target tar;
		tar.request.min=-6.0;
		tar.request.max=6.0;
		client.call(tar);
		tar_x = tar.response.x;
		tar_y = tar.response.y;
		
		assignment1::tar mymsg;
		mymsg.name = "target:";
		mymsg.tarx = tar_x;
		mymsg.tary = tar_y;
		pub2.publish(mymsg);
		
		ROS_INFO("Target Reached \nNew target: [%f, %f]", tar_x, tar_y);
	}
		geometry_msgs::Twist vel;
		vel.linear.x=2.0*(tar_x-msg->pose.pose.position.x);
		vel.linear.y=2.0*(tar_y-msg->pose.pose.position.y);	
		pub.publish(vel);
}



int main(int argc, char **argv) 
{
	ros::init(argc, argv, "controller");
	ros::NodeHandle n;

	client = n.serviceClient<assignment1::target>("/target");
	pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
	pub2 = n.advertise<assignment1::tar>("/target",1000);
	sub = n.subscribe("/odom", 1000, positionCallback);
	
	ros::spin();

  	return 0;
}
