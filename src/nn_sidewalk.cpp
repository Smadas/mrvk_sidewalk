//ROS libraries
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Int32.h"
#include "gazebo_msgs/GetModelState.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/Point.h"
#include "gazebo_msgs/ModelStates.h"

#include <sstream>
#include <cmath>

//random number
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

//looking for home directory
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <string> 

//file libraries
#include <iostream>
#include <fstream>

//FANN libraries
#include "floatfann.h"

std::string get_directory(std::string file_name, std::string file_num, std::string file_type);

int main(int argc, char **argv)
{
	//START ROS initialization
	ros::init(argc, argv, "nn_sidewalk");
	ros::NodeHandle n;

	//neural network init
	fann_type *NN_output;
	fann_type NN_input[4];
	std::string nn_dir;
	nn_dir = get_directory("neural_network", "", "net");
	struct fann *ann = fann_create_from_file(nn_dir.c_str());//declare fann NN

	///compute NN
	NN_input[0] = 0.5;
	NN_input[1] = 0.5;
	NN_input[2] = 0.5;
	NN_output = fann_run(ann, NN_input);
	std::cout << "output " << NN_output[0] << " " << NN_output[1] << " " << NN_output[2] << std::endl;
	return 0;
}

std::string get_directory(std::string file_name, std::string file_num, std::string file_type)
{
	struct passwd *pw = getpwuid(getuid());
	const char *homedir = pw->pw_dir;
	std::string file_directory;
	file_directory = homedir;
	file_directory = file_directory + "/catkin_cpp_test/src/mrvk_sidewalk/src/" + file_name + file_num + "." + file_type;
	return file_directory;
}
