#include "ros/ros.h"
#include "assignment1/target.h"

//This process returns the x and y position of a new target for the robot given
//the max and min possible values choosing randomly in between them.

double randMToN(double M, double N)
{
	//calculate random number between max and min
 	return M + (rand() / ( RAND_MAX / (N-M) ) ) ; 
}



bool myrandom(assignment1::target::Request&req, assignment1::target::Response&res){
	//returns the two random values for x and y
	res.x= randMToN(req.min, req.max);	
	res.y= randMToN(req.min, req.max);
	return true;
}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "target_server");
	ros::NodeHandle n;
	ros::ServiceServer service= n.advertiseService("/target", myrandom);
	ros::spin();
	return 0;
}
