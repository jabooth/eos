#include "eos/core/Landmark.hpp"
#include "eos/core/LandmarkMapper.hpp"
#include "eos/fitting/nonlinear_camera_estimation.hpp"
#include "eos/fitting/linear_shape_fitting.hpp"
#include "eos/render/utils.hpp"
#include "eos/render/texture_extraction.hpp"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "boost/program_options.hpp"
#include "boost/filesystem.hpp"

#include <vector>
#include <iostream>
#include <fstream>

using namespace eos;
namespace po = boost::program_options;
namespace fs = boost::filesystem;
using eos::core::Landmark;
using eos::core::LandmarkCollection;
using cv::Mat;
using cv::Vec2f;
using cv::Vec3f;
using cv::Vec4f;
using std::cout;
using std::endl;
using std::vector;
using std::string;


void writeMatToFile(cv::Mat& m, const char* filename)
{
    std::ofstream fout(filename);

    if(!fout)
    {
        std::cout << "File Not Opened" << std::endl;  return;
    }

    for(int i=0; i<m.rows; i++)
    {
        for(int j=0; j<m.cols; j++)
        {
            fout << m.at<float>(i,j) << "\t";
        }
        fout << std::endl;
    }

    fout.close();
}

void writeVectorToFile(std::vector<std::array<int, 3>> m, const char* filename)
{
    std::ofstream fout(filename);

    if(!fout)
    {
        std::cout << "File Not Opened" << std::endl;  return;
    }

	for (int i = 0; i < m.size(); ++i) {
		auto tris = m[i];
		fout << tris[0] << "\t" << tris[1] << "\t" << tris[2] << std::endl;
	}

    fout.close();
}

int main(int argc, char *argv[])
{

	morphablemodel::MorphableModel morphable_model;

	try {
		morphable_model = morphablemodel::load_model("/vol/construct3dmm/experiments/src/static/other_models/surrey/src/sfm_shape_29587.bin");
	}
	catch (const std::runtime_error& e) {
		cout << "Error loading the Morphable Model: " << e.what() << endl;
		return EXIT_FAILURE;
	}

	auto shape_model = morphable_model.get_shape_model();

	std::vector<std::array<int, 3>> tri_list = shape_model.get_triangle_list();
	cv::Mat shape_basis = shape_model.get_normalised_pca_basis();
	cv::Mat mean = shape_model.get_mean();
	writeMatToFile(shape_basis, "shape_basis.csv");
	writeMatToFile(mean, "shape_mean.csv");
	writeVectorToFile(tri_list, "trilist.csv");
	return EXIT_SUCCESS;
}
