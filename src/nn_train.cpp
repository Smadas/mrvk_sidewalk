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
bool generateTrainData(int numPic, int numIn, int numOut);
double normalize_input2(double minIn, double maxIn, double min, double max, double value);

int main()
{
	int num_layers = 3;
	int num_input = 100;
	int num_neurons_hidden[4];
	num_neurons_hidden[0] = 10;
	num_neurons_hidden[1] = 5;
	num_neurons_hidden[2] = 4;
	num_neurons_hidden[3] = 3;
	int num_output = 1;
	struct fann *ann = fann_create_standard(num_layers, num_input, num_neurons_hidden[0], num_output);

	//START set training parameters
	fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
	fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);
	fann_set_learning_momentum(ann, 0.9);
	fann_set_learning_rate(ann, 0.6);
	//END set training parameters

	double desired_error = 0.001;
	int epochs_between_reports = 1;
	int max_epochs = 100;
	//vsetky vstupy a vystupy by mali byt v rozsahu (1, -1)
	generateTrainData(10, num_input, num_output);
	fann_train_on_file(ann, get_directory("trainData", "", "txt").c_str(), max_epochs, epochs_between_reports, desired_error);

	std::string nn_dir;
	nn_dir = get_directory("neural_network", "", "net");
	fann_save(ann, nn_dir.c_str());

	fann_destroy(ann);

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

bool generateTrainData(int numPic, int numIn, int numOut)
{
	cv::Mat img;
	std::ofstream trainData;
	trainData.open(get_directory("trainData", "", "txt").c_str());
	trainData << numPic << " " << numIn	<< " " << numOut << std::endl;//data header

	for (int numVec = 1; numVec < (numPic + 1); numVec++)
	{
		img = cv::imread(get_directory("data/", std::to_string(numVec).c_str(), "jpg").c_str(), CV_LOAD_IMAGE_COLOR);
		cvtColor(img, img, CV_BGR2GRAY);
		for (int i = 0; i < img.size().width; i++)
		{
			for (int j = 0; j < img.size().height; j++)
			{
				trainData << normalize_input2(0, 255, 0, 1, img.at<uchar>(j, i)) << " ";
			}
		}
		trainData << std::endl;
		if (numVec < 6)
		{
			trainData << 0;
		}
		else
		{
			trainData << 1;
		}
		trainData << std::endl;
	}

	trainData.close();
	return true;
}

double normalize_input2(double minIn, double maxIn, double min, double max, double value)
{
	double k = 0;
	k = (max - min)/(maxIn - minIn);
	double q = 0;
	q = min - k*minIn;
	return (k*value + q);
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
