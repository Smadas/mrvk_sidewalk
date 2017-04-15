//FANN libraries
#include "fann.h"
#include "floatfann.h"

//files libraries
#include <iostream>
#include <fstream>

//looking for home directory
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <string> 

//opencv
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <sensor_msgs/Image.h>

std::string get_directory(std::string file_name, std::string file_num, std::string file_type);
bool generateTrainData();

int main()
{
	int num_layers = 6;
	int num_input = 3;
	int num_neurons_hidden[4];
	num_neurons_hidden[0] = 10;
	num_neurons_hidden[1] = 5;
	num_neurons_hidden[2] = 4;
	num_neurons_hidden[3] = 3;
	int num_output = 3;
	struct fann *ann = fann_create_standard(num_layers, num_input, num_neurons_hidden[0], num_neurons_hidden[1], num_neurons_hidden[2], num_neurons_hidden[3], num_output);

	//START set training parameters
	fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
	fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);
	fann_set_learning_momentum(ann, 0.9);
	fann_set_learning_rate(ann, 0.6);
	//END set training parameters

	double desired_error = 0.0001;
	int epochs_between_reports = 1;
	int max_epochs = 100;
	//vsetky vstupy a vystupy by mali byt v rozsahu (1, -1)
	fann_train_on_file(ann, get_directory("trainData", "", "txt").c_str(), max_epochs, epochs_between_reports, desired_error);

	std::string nn_dir;
	nn_dir = get_directory("neural_network", "", "net");
	fann_save(ann, nn_dir.c_str());

	fann_destroy(ann);

	generateTrainData();

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

bool generateTrainData()
{
	cv::Mat img;
	std::ifstream trainData;
	//namedWindow("read img", WINDOW_AUTOSIZE);
	for (int numPic = 1; numPic < 11; numPic++)
	{
		img = cv::imread(get_directory("data/", std::to_string(numPic).c_str(), "jpg").c_str(), CV_LOAD_IMAGE_COLOR);
	}
	return true;
}

/*
tvar vstupenho suboru
pocetDatovychVektorov pocetVstupov pocetVystupov
vstupnyVektor1
vystupnyVektor1
.
.
.
*/
