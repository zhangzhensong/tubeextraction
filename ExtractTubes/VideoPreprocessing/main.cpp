#include <iostream>
#include <cstdlib>
#include <opencv2\opencv.hpp>
#include <string>

using namespace std;

int keyboard; // input from keyboard


int scaleVideo()
{
	std::string videoPath = "D:/Projects/VideoSynopsis/VideosSameScene2/MVI_2075.MOV";
	cv::VideoCapture videoCap;
	videoCap.open(videoPath);

	if (!videoCap.isOpened())
	{
		std::cout << "Open video failure." << std::endl;
		system("pause");
		return 0;
	}

	cv::VideoWriter outputVideo;
	int ex = static_cast<int>(videoCap.get(CV_CAP_PROP_FOURCC));

	cv::Size S = cv::Size((int)videoCap.get(CV_CAP_PROP_FRAME_WIDTH) / 2,    // Acquire input size
		(int)videoCap.get(CV_CAP_PROP_FRAME_HEIGHT) / 2);

	std::string outVideoPath = "D:/Projects/VideoSynopsis/VideosSameScene2/MVI_2075_7000.mov";
	outputVideo.open(outVideoPath, ex, videoCap.get(CV_CAP_PROP_FPS), S, true);

	if (!outputVideo.isOpened())
	{
		cout << "Could not open the output video for write: " << outVideoPath << endl;
		return -1;
	}

	char windowName[] = "This is a demo";
	cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);

	cv::Mat frame, scaledownFrame;
	keyboard = 1;

	int numOfFrame = 0;

	const int startFrame = 0;
	const int targetVideoLen = 7000;

	videoCap.set(CV_CAP_PROP_POS_FRAMES, startFrame);

	while (videoCap.read(frame) && (char)keyboard != 'q' && (char)keyboard != 27 && numOfFrame < targetVideoLen)
	{
		numOfFrame++;

		cv::resize(frame, scaledownFrame, S);
		cv::imshow(windowName, scaledownFrame);
		std::cout << "Frame ID: " << numOfFrame << std::endl;
		keyboard = cv::waitKey(1);

		outputVideo << scaledownFrame;
	}

	videoCap.release();
	outputVideo.release();
	cv::destroyAllWindows();

	return 0;
}


int connectVideo(std::string inputVideoPath1, std::string inputVideoPath2, std::string outputVideoPath)
{
	cv::VideoCapture inputVideo1, inputVideo2;
	inputVideo1.open(inputVideoPath1);
	inputVideo2.open(inputVideoPath2);

	if (!inputVideo1.isOpened())
	{
		std::cout << "Open video 1 failure." << std::endl;
		system("pause");
		return 0;
	}

	if (!inputVideo2.isOpened())
	{
		std::cout << "Open video 2 failure." << std::endl;
		system("pause");
		return 0;
	}

	cv::VideoWriter outputVideo;
	int ex = static_cast<int>(inputVideo1.get(CV_CAP_PROP_FOURCC));

	cv::Size S = cv::Size((int)inputVideo1.get(CV_CAP_PROP_FRAME_WIDTH),    // Acquire input size
		(int)inputVideo1.get(CV_CAP_PROP_FRAME_HEIGHT));

	outputVideo.open(outputVideoPath, ex, inputVideo1.get(CV_CAP_PROP_FPS), S, true);

	if (!outputVideo.isOpened())
	{
		std::cout << "Open output video 2 failure." << std::endl;
		system("pause");
		return 0;
	}


	int numOfFrame = 0;

	const int startFrame = 0;

	inputVideo1.set(CV_CAP_PROP_POS_FRAMES, 0);
	inputVideo2.set(CV_CAP_PROP_POS_FRAMES, 0);

	cv::Mat frame;
	char windowName[] = "Connectting videos";
	cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);

	while (inputVideo1.read(frame))
	{
		numOfFrame++;

		cv::imshow(windowName, frame);
		std::cout << "Frame ID: " << numOfFrame << std::endl;
		keyboard = cv::waitKey(1);

		outputVideo << frame;
	}

	while (inputVideo2.read(frame))
	{
		numOfFrame++;

		cv::imshow(windowName, frame);
		std::cout << "Frame ID: " << numOfFrame << std::endl;
		keyboard = cv::waitKey(1);

		outputVideo << frame;
	}

	inputVideo1.release();
	inputVideo2.release();
	outputVideo.release();
	cv::destroyAllWindows();

	return 0;
}


int main()
{
	std::string video1 = "D:/Projects/VideoSynopsis/VideosSameScene3/Left_8000.mov";
	std::string video2 = "D:/Projects/VideoSynopsis/VideosSameScene3/Left_7000.mov";
	std::string video3 = "D:/Projects/VideoSynopsis/VideosSameScene3/Left.mov";

	connectVideo(video1, video2, video3);

	system("pause");
	return 0;
}

