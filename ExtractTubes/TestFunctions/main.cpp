#include <iostream>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <opencv2/saliency.hpp>
#include <string>

#include "boost/serialization/serialization.hpp"  
#include "boost/archive/binary_oarchive.hpp"  
#include "boost/archive/binary_iarchive.hpp"  
#include <boost/serialization/export.hpp>  
#include "boost/foreach.hpp"  
#include "boost/any.hpp"  
#include <boost/serialization/vector.hpp>  

#include <Cmlib.h>

using namespace saliency;


struct testClass
{
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & a;
		ar & haha;
	}

	int a;
	std::string haha;
};


void onMouse(int event, int x, int y, int flags, void* param)
{
	cv::Mat *im = reinterpret_cast<cv::Mat*>(param);
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:     //鼠标左键按下响应：返回坐标和灰度  
		std::cout << "at(" << x << "," << y << ")value is:"
			<< static_cast<int>(im->at<uchar>(cv::Point(x, y))) << std::endl;
		break;
	case CV_EVENT_RBUTTONDOWN:    //鼠标右键按下响应：输入坐标并返回该坐标的灰度  
		std::cout << "input(x,y)" << std::endl;
		std::cout << "x =" << std::endl;
		std::cin >> x;
		std::cout << "y =" << std::endl;
		std::cin >> y;
		std::cout << "at(" << x << "," << y << ")value is:"
			<< static_cast<int>(im->at<uchar>(cv::Point(x, y))) << std::endl;
		break;
	}
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == cv::EVENT_LBUTTONDOWN)
	{
		std::cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << std::endl;
	}
	else if (event == cv::EVENT_RBUTTONDOWN)
	{
		std::cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << std::endl;
	}
	else if (event == cv::EVENT_MBUTTONDOWN)
	{
		std::cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << std::endl;
	}
	else if (event == cv::EVENT_MOUSEMOVE)
	{
		//std::cout << "Mouse move over the window - position (" << x << ", " << y << ")" << std::endl;

	}
}

int testmouse()
{
	std::string imgpath = "D:/Projects/VideoSynopsis/VideosSameScene2/image_02/data/3000.png";
	cv::Mat image = cv::imread(imgpath);

	if (image.empty())
	{
		std::cout << "fail to load the image" << std::endl;
		return -1;
	}

	const char* windowname = "This is a demo";
	cv::namedWindow(windowname, CV_WINDOW_AUTOSIZE);

	//cv::setMouseCallback(windowname, onMouse, reinterpret_cast<void*> (&image));
	cv::setMouseCallback(windowname, CallBackFunc, NULL);

	cv::imshow(windowname, image);

	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}

/// Global Variables
int alpha_slider_max = 100;
int alpha_slider;
double alpha;
double beta;

/// Matrices to store images
cv::Mat src1;
cv::Mat src2;
cv::Mat dst;

// http://docs.opencv.org/2.4/doc/tutorials/highgui/trackbar/trackbar.html
void on_trackbar(int, void*)
{
	alpha = (double)alpha_slider / alpha_slider_max;
	beta = (1.0 - alpha);

	cv::addWeighted(src1, alpha, src2, beta, 0.0, dst);

	cv::imshow("Linear Blend", dst);
}


int testtrackbar()
{

	/// Read image ( same size, same type )
	cv::Mat osrc1  = cv::imread("D:/Projects/VideoSynopsis/VideosSameScene2/image_02/data/3000.png");
	cv::Mat osrc2 = cv::imread("D:/Projects/VideoSynopsis/VideosSameScene2/image_02/data/3001.png");

	if (!osrc1.data) { printf("Error loading src1 \n"); return -1; }
	if (!osrc2.data) { printf("Error loading src2 \n"); return -1; }


	cv::Size S(osrc1.cols / 2, osrc1.rows / 2);

	cv::resize(osrc1, src1, S);
	cv::resize(osrc2, src2, S);

	/// Initialize values
	alpha_slider = 50;
	//alpha_slider_max = 200;

	/// Create Windows
	cv::namedWindow("Linear Blend", 1);

	/// Create Trackbars
	char TrackbarName[50];
	sprintf_s(TrackbarName, "Alpha: %d", alpha_slider_max);

	cv::createTrackbar(TrackbarName, "Linear Blend", &alpha_slider, alpha_slider_max, on_trackbar);

	/// Show some stuff
	on_trackbar(alpha_slider, 0);

	/// Wait until user press some key
	cv::waitKey(0);
	cv::destroyAllWindows();

	return 0;
}

int testDraw()
{
	//line and rectangle
	cv::Mat osrc1 = cv::imread("D:/Projects/VideoSynopsis/VideosSameScene2/image_02/data/3000.png");
	if (!osrc1.data) { printf("Error loading src1 \n"); return -1; }
	cv::Size S(osrc1.cols / 2, osrc1.rows / 2);

	cv::resize(osrc1, src1, S);
	cv::Point pt1(50, 100);
	cv::Point pt2(200, 300);

	cv::Scalar color(0, 0, 255); // bgr
	cv::rectangle(osrc1, pt1, pt2, color);
	cv::line(osrc1, pt1, pt2, color);

	cv::namedWindow("test draw", CV_WINDOW_AUTOSIZE);
	cv::imshow("test draw", osrc1);
	cv::waitKey(0);

	cv::destroyAllWindows();

	return 0;
}

int testSaliency()
{
	CStr dirPath = "D:/Projects/VideoSynopsis/VideosSameScene4/Left/";
	CStr imgPath = dirPath + "0129.png";
	CStr salDir = "D:/";
	//CmSalCut::Demo(imgPath, "", salDir);
	//CmSaliencyGC::Demo(imgPath, salDir);

	CStr saliencyAlgorithm = "SPECTRAL_RESIDUAL";

	// https://github.com/opencv/opencv_contrib/blob/master/modules/saliency/samples/computeSaliency.cpp
	cv::Ptr<cv::saliency::Saliency> pSaliencyAlgo = cv::saliency::Saliency::create(saliencyAlgorithm);
	//"SPECTRAL_RESIDUAL" – StaticSaliencySpectralResidual
	//"BING" – ObjectnessBING

	if (saliencyAlgorithm.empty())
	{
		cout << "***Error in the instantiation of the saliency algorithm...***\n";
		return -1;
	}

	Mat binaryMap;
	Mat image = cv::imread(imgPath);

	if (saliencyAlgorithm.find("SPECTRAL_RESIDUAL") == 0)
	{
		Mat saliencyMap;
		if (pSaliencyAlgo->computeSaliency(image, saliencyMap))
		{
			StaticSaliencySpectralResidual spec;
			spec.computeBinaryMap(saliencyMap, binaryMap);

			imshow("Saliency Map", saliencyMap);
			imshow("Original Image", image);
			imshow("Binary Map", binaryMap);
			waitKey(0);
		}

	}


}


void testBoostSerilization()
{
	std::string content;

	testClass a;
	a.a = 10;
	a.haha = "Just a test";

	std::ostringstream os;
	boost::archive::binary_oarchive oa(os);

	oa << a;

	content = os.str();

	std::cout << "archive content: " << content << std::endl;

	testClass b;
	std::istringstream is(content);
	boost::archive::binary_iarchive ia(is);
	ia >> b;//从一个保存序列化数据的string里面反序列化，从而得到原来的对象。

	std::cout << "int: " << b.a << ", string: " << b.haha << std::endl;

	// a non-portable native binary archive
	//boost::archive::binary_oarchive(ostream &s) // saving
	//boost::archive::binary_iarchive(istream &s) // loading

	
}

int main()
{
	//testmouse();
	//testtrackbar();
	//testDraw();
	//testSaliency();

	testBoostSerilization();

	system("pause");
	return 0;
}