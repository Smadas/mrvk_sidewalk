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

//opencv
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <sensor_msgs/Image.h>

std::string get_directory(std::string file_name, std::string file_num, std::string file_type);
double normalize_input2(double minIn, double maxIn, double min, double max, double value);

int main(int argc, char **argv)
{
	//START ROS initialization
	ros::init(argc, argv, "nn_sidewalk");
	ros::NodeHandle n;

	//neural network init
	fann_type *NN_output;
	fann_type NN_input[100];
	std::string nn_dir;
	nn_dir = get_directory("neural_network", "", "net");
	struct fann *ann = fann_create_from_file(nn_dir.c_str());//declare fann NN

	///get input data
	cv::Mat img;
	img = cv::imread(get_directory("C", "", "jpg").c_str(), CV_LOAD_IMAGE_COLOR);
	cvtColor(img, img, CV_BGR2GRAY);
	for (int i = 0; i < img.size().width; i++)
	{
		for (int j = 0; j < img.size().height; j++)
		{
			NN_input[10*i + j] = normalize_input2(0, 255, 0, 1, img.at<uchar>(j, i));
		}
	}
	///compute NN
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
double normalize_input2(double minIn, double maxIn, double min, double max, double value)
{
	double k = 0;
	k = (max - min)/(maxIn - minIn);
	double q = 0;
	q = min - k*minIn;
	return (k*value + q);
}