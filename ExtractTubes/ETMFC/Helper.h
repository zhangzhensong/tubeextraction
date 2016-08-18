#pragma once
#include <opencv2\opencv.hpp>
#include <opencv2/bgsegm.hpp>



#include "VideoObject.h"

typedef std::basic_string<TCHAR> tstring;

class Helper
{
public:
	Helper();
	~Helper();

	/*MatToCImage
	*简介：
	*   OpenCV的Mat转ATL/MFC的CImage，仅支持单通道灰度或三通道彩色
	*参数：
	*   mat：OpenCV的Mat
	*   cimage：ATL/MFC的CImage
	*/
	static void MatToCImage(cv::Mat& mat, CImage& cimage);


	/*CImageToMat
	*简介：
	*   ATL/MFC的CImage转OpenCV的Mat，仅支持单通道灰度或三通道彩色
	*参数：
	*   cimage：ATL/MFC的CImage
	*   mat：OpenCV的Mat
	*/
	static void CImageToMat(CImage& cimage, cv::Mat& mat);

	// convert CString to std::string
	static void CStringtostring(const CString& str1, std::string& str2);
	static std::string CStringtostring(const CString& str1);
	
	static void stringtoCString(const std::string& str1, CString& str2);
	static CString stringtoCString(const std::string& str1);

	static void GetFilesInDirectory(std::vector<std::string> &out, const std::string &directory);

	// choose a directory
	static CString BrowseForFolder(HWND hwnd, CString title, CString folder);

	// Given a path to a video file, play the video
	static void playVideo(std::string videoPath);

public:
	static VideoObject videoObject;

	static int imageHeight;
	static int imageWidth;

	static cv::Point tlpt, drpt, tmppt;
	static bool rightmousedown;
	static bool bDoBackGroundSubtraction;
	static bool bShowSelectedTube;
	static bool bStartPlay;
	static bool bInteractive;

	static cv::Ptr<cv::bgsegm::BackgroundSubtractorMOG> pMOG;
};

