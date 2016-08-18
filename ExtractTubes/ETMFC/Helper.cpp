#include "stdafx.h"
#include "Helper.h"

#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdlib>
//#include <ctime>


VideoObject Helper::videoObject = VideoObject();
cv::Point Helper::tlpt = cv::Point(-1,-1);
cv::Point Helper::drpt = cv::Point(-1,-1);
cv::Point Helper::tmppt = cv::Point(0,0);
bool Helper::rightmousedown = false;
bool Helper::bDoBackGroundSubtraction = false;
bool Helper::bShowSelectedTube = false;
bool Helper::bStartPlay = false;
bool Helper::bInteractive = false;

int Helper::imageHeight = -1;
int Helper::imageWidth = -1;

cv::Ptr<cv::bgsegm::BackgroundSubtractorMOG> Helper::pMOG = cv::bgsegm::createBackgroundSubtractorMOG();

Helper::Helper()
{
}


Helper::~Helper()
{
}

void Helper::MatToCImage(cv::Mat& mat, CImage& cimage)
{
	if (0 == mat.total())
	{
		return;
	}


	int nChannels = mat.channels();
	if ((1 != nChannels) && (3 != nChannels))
	{
		return;
	}
	int nWidth = mat.cols;
	int nHeight = mat.rows;


	//重建cimage
	cimage.Destroy();
	cimage.Create(nWidth, nHeight, 8 * nChannels);


	//拷贝数据


	uchar* pucRow;									//指向数据区的行指针
	uchar* pucImage = (uchar*)cimage.GetBits();		//指向数据区的指针
	int nStep = cimage.GetPitch();					//每行的字节数,注意这个返回值有正有负


	if (1 == nChannels)								//对于单通道的图像需要初始化调色板
	{
		RGBQUAD* rgbquadColorTable;
		int nMaxColors = 256;
		rgbquadColorTable = new RGBQUAD[nMaxColors];
		cimage.GetColorTable(0, nMaxColors, rgbquadColorTable);
		for (int nColor = 0; nColor < nMaxColors; nColor++)
		{
			rgbquadColorTable[nColor].rgbBlue = (uchar)nColor;
			rgbquadColorTable[nColor].rgbGreen = (uchar)nColor;
			rgbquadColorTable[nColor].rgbRed = (uchar)nColor;
		}
		cimage.SetColorTable(0, nMaxColors, rgbquadColorTable);
		delete[]rgbquadColorTable;
	}


	for (int nRow = 0; nRow < nHeight; nRow++)
	{
		pucRow = (mat.ptr<uchar>(nRow));
		for (int nCol = 0; nCol < nWidth; nCol++)
		{
			if (1 == nChannels)
			{
				*(pucImage + nRow * nStep + nCol) = pucRow[nCol];
			}
			else if (3 == nChannels)
			{
				for (int nCha = 0; nCha < 3; nCha++)
				{
					*(pucImage + nRow * nStep + nCol * 3 + nCha) = pucRow[nCol * 3 + nCha];
				}
			}
		}
	}
}

void Helper::CImageToMat(CImage& cimage, cv::Mat& mat)
{
	if (true == cimage.IsNull())
	{
		return;
	}


	int nChannels = cimage.GetBPP() / 8;
	if ((1 != nChannels) && (3 != nChannels))
	{
		return;
	}
	int nWidth = cimage.GetWidth();
	int nHeight = cimage.GetHeight();


	//重建mat
	if (1 == nChannels)
	{
		mat.create(nHeight, nWidth, CV_8UC1);
	}
	else if (3 == nChannels)
	{
		mat.create(nHeight, nWidth, CV_8UC3);
	}


	//拷贝数据


	uchar* pucRow;									//指向数据区的行指针
	uchar* pucImage = (uchar*)cimage.GetBits();		//指向数据区的指针
	int nStep = cimage.GetPitch();					//每行的字节数,注意这个返回值有正有负


	for (int nRow = 0; nRow < nHeight; nRow++)
	{
		pucRow = (mat.ptr<uchar>(nRow));
		for (int nCol = 0; nCol < nWidth; nCol++)
		{
			if (1 == nChannels)
			{
				pucRow[nCol] = *(pucImage + nRow * nStep + nCol);
			}
			else if (3 == nChannels)
			{
				for (int nCha = 0; nCha < 3; nCha++)
				{
					pucRow[nCol * 3 + nCha] = *(pucImage + nRow * nStep + nCol * 3 + nCha);
				}
			}
		}
	}
}

void Helper::CStringtostring(const CString& str1, std::string& str2)
{
	str2 = CW2A(str1.GetString());
}

std::string Helper::CStringtostring(const CString& str1)
{
	return std::string(CW2A(str1.GetString()));
}

void Helper::stringtoCString(const std::string& str1, CString& str2)
{
	str2 = str1.c_str();
}

CString Helper::stringtoCString(const std::string& str1)
{
	return CString(str1.c_str());
}


void Helper::playVideo(std::string videoPath)
{
	cv::VideoCapture videoCap;
	videoCap.open(videoPath);

	if (!videoCap.isOpened())
	{
		std::cout << "Open video failure." << std::endl;
		system("pause");
		return;
	}

	char windowName[] = "This is a demo";
	cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);

	int numOfFrame = 0;

	const int startFrame = 0;

	videoCap.set(CV_CAP_PROP_POS_FRAMES, startFrame);
	cv::Mat frame;
	int keyboard = 0;
	while (videoCap.read(frame) && (char)keyboard != 'q' && (char)keyboard != 27)
	{
		numOfFrame++;

		cv::imshow(windowName, frame);
		std::cout << "Frame ID: " << numOfFrame << std::endl;
		keyboard = cv::waitKey(1);

	}

	videoCap.release();
	cv::destroyAllWindows();
}

// callback function 
INT CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
	if (uMsg == BFFM_INITIALIZED) SendMessage(hwnd, BFFM_SETSELECTION, TRUE, pData);
	return 0;
}

// browseforfolder function
// returns the folder or an empty string if no folder was selected
// hwnd = handle to parent window
// title = text in dialog
// folder = selected (default) folder
CString Helper::BrowseForFolder(HWND hwnd, CString title, CString folder)
{
	CString ret;

	BROWSEINFO br;
	ZeroMemory(&br, sizeof(BROWSEINFO));
	br.lpfn = BrowseCallbackProc;
	br.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	br.hwndOwner = hwnd;
	br.lpszTitle = title;
	br.lParam = (LPARAM)folder.GetString();

	LPITEMIDLIST pidl = NULL;
	if ((pidl = SHBrowseForFolder(&br)) != NULL)
	{
		wchar_t buffer[MAX_PATH];
		if (SHGetPathFromIDList(pidl, buffer)) ret = buffer;
		
	}

	return ret;
}


// sample function call
//CString folder = BrowseForFolder(this->m_hWnd, L"Select Folder", L"C:\\");


/* Returns a list of files in a directory (except the ones that begin with a dot) */

void Helper::GetFilesInDirectory(std::vector<std::string> &out, const std::string &directory)
{
//#ifdef WINDOWS
#if 1
	HANDLE dir;
	WIN32_FIND_DATA file_data;

	if ((dir = FindFirstFile(CString((directory + "/*").c_str()), &file_data)) == INVALID_HANDLE_VALUE)
		return; /* No files found */

	do {
		std::wstring wstr(file_data.cFileName);
		std::string str(wstr.begin(), wstr.end());
		const std::string file_name = str;
		const std::string full_file_name = directory + "\\" + file_name;
		const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (file_name[0] == '.')
			continue;

		if (is_directory)
			continue;

		out.push_back(full_file_name);
	} while (FindNextFile(dir, &file_data));

	FindClose(dir);
#else
	DIR *dir;
	class dirent *ent;
	class stat st;

	dir = opendir(directory);
	while ((ent = readdir(dir)) != NULL) {
		const string file_name = ent->d_name;
		const string full_file_name = directory + "/" + file_name;

		if (file_name[0] == '.')
			continue;

		if (stat(full_file_name.c_str(), &st) == -1)
			continue;

		const bool is_directory = (st.st_mode & S_IFDIR) != 0;

		if (is_directory)
			continue;

		out.push_back(full_file_name);
	}
	closedir(dir);
#endif
} // GetFilesInDirectory


