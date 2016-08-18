
// ETMFCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ETMFC.h"
#include "ETMFCDlg.h"
#include "afxdialogex.h"
#include "Helper.h"
#include <string>
#include <algorithm>
#include <cstdlib>
#include <fstream>

#include <CmLib.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const char windowName[] = "Video";

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

bool comparePoint(const cv::Point& pt1, const cv::Point& pt2)
{
	return (pt1.x <= pt2.x && pt1.y <= pt2.y);
}

int mymin(int x, int y)
{
	return x < y ? x : y;
}

int mymax(int x, int y)
{
	return x > y ? x : y;
}

void drawRectangle()
{
	cv::Mat img = cv::imread(Helper::videoObject.allFrames.at(Helper::videoObject.currentFrame).filePath);

	if (Helper::drpt != cv::Point(-1, -1))
	{
		cv::Scalar color;
		
		if (comparePoint(Helper::tlpt, Helper::drpt))
			color = cv::Scalar(0, 0, 255);   // valid
		else
		{
			color = cv::Scalar(255, 0, 0);   // invalid

			std::string text = "Invalid";
			int fontFace = cv::FONT_HERSHEY_PLAIN;
			double fontScale = 2;
			int thickness = 3;

			cv::Point textOrg(mymin(Helper::tlpt.x, Helper::drpt.x), (Helper::tlpt.y + Helper::drpt.y)/2);
			cv::putText(img, text, textOrg, fontFace, fontScale,
				color, thickness, 8);
		}

		cv::rectangle(img, Helper::tlpt, Helper::drpt, color);
	}
	else if (Helper::tlpt != cv::Point(-1, -1))
	{
		cv::Scalar color = cv::Scalar(0, 0, 255);
		cv::circle(img, Helper::tlpt, 3, color);
	}
	
	cv::imshow(windowName, img);
	cv::waitKey(30);
}

void drawRectangle(cv::Mat img, cv::Point tlpt, cv::Point drpt, cv::Scalar color)
{
	if (drpt != cv::Point(-1, -1))
	{
		cv::rectangle(img, tlpt, drpt, color);
	}
	else if (tlpt != cv::Point(-1, -1))
	{
		cv::Scalar color = cv::Scalar(0, 0, 255);
		cv::circle(img, tlpt, 3, color);
	}
}


void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	HWND hWnd = *((HWND*)userdata);
	std::string labelStr = std::string("Tube num: ") + std::to_string(Helper::videoObject.allTubes.size());
	SetDlgItemText(hWnd, IDC_STATIC_TUBENUM, Helper::stringtoCString(labelStr));

	if (event == cv::EVENT_LBUTTONDOWN)
	{
		Helper::tlpt.x = x;
		Helper::tlpt.y = y;
		
		SetDlgItemText(hWnd, IDC_EDIT_TLPTX, Helper::stringtoCString(std::to_string(x)));
		SetDlgItemText(hWnd, IDC_EDIT_TLPTY, Helper::stringtoCString(std::to_string(y)));

		drawRectangle();
	}
	else if (event == cv::EVENT_RBUTTONDOWN)
	{
		//std::cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << std::endl;

		Helper::drpt.x = x;
		Helper::drpt.y = y;

		SetDlgItemText(hWnd, IDC_EDIT_DRPTX, Helper::stringtoCString(std::to_string(x)));
		SetDlgItemText(hWnd, IDC_EDIT_DRPTY, Helper::stringtoCString(std::to_string(y)));

		drawRectangle();
	}
	else if (event == cv::EVENT_RBUTTONUP)
	{
		
	}
	else if (event == cv::EVENT_MBUTTONDOWN)
	{
		//std::cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << std::endl;
	}
	else if (event == cv::EVENT_MOUSEMOVE)
	{
		//std::cout << "Mouse move over the window - position (" << x << ", " << y << ")" << std::endl;
	}
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CETMFCDlg dialog



CETMFCDlg::CETMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CETMFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CETMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_Slider);
}

BEGIN_MESSAGE_MAP(CETMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CETMFCDlg::OnBnClickedOpenImg)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDOK, &CETMFCDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CETMFCDlg::OnBnClickedCancel)
	ON_WM_EXITSIZEMOVE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_IMAGEFOLDER, &CETMFCDlg::OnBnClickedButtonImagefolder)
	ON_BN_CLICKED(IDC_BUTTON_GOTOFRAME, &CETMFCDlg::OnBnClickedButtonGotoframe)
	ON_BN_CLICKED(IDC_BUTTON_VIDEO_TO_IMAGES, &CETMFCDlg::OnBnClickedButtonVideoToImages)
	ON_BN_CLICKED(IDC_BUTTON_IMAGEFOLDER_ET, &CETMFCDlg::OnBnClickedButtonImagefolderEt)
	ON_BN_CLICKED(IDC_BUTTON_PREFRAME, &CETMFCDlg::OnBnClickedButtonPreframe)
	ON_BN_CLICKED(IDC_BUTTON_P10FRAMES, &CETMFCDlg::OnBnClickedButtonP10frames)
	ON_BN_CLICKED(IDC_BUTTON_P100FRAMES, &CETMFCDlg::OnBnClickedButtonP100frames)
	ON_BN_CLICKED(IDC_BUTTON_NEXTFRAME, &CETMFCDlg::OnBnClickedButtonNextframe)
	ON_BN_CLICKED(IDC_BUTTON_N10FRAMES, &CETMFCDlg::OnBnClickedButtonN10frames)
	ON_BN_CLICKED(IDC_BUTTON_N100FRAMES, &CETMFCDlg::OnBnClickedButtonN100frames)
	ON_BN_CLICKED(IDC_BUTTON_EDITTUBE, &CETMFCDlg::OnBnClickedButtonEdittube)
	ON_BN_CLICKED(IDC_BUTTON_SHOWSELECTTUBE, &CETMFCDlg::OnBnClickedButtonShowselecttube)
	ON_BN_CLICKED(IDC_BUTTON_SHOWCANDIDATE, &CETMFCDlg::OnBnClickedButtonShowcandidate)
	ON_EN_CHANGE(IDC_EDIT_WHICHTUBE, &CETMFCDlg::OnEnChangeEditWhichtube)
	ON_EN_CHANGE(IDC_EDIT_TLPTX, &CETMFCDlg::OnEnChangeEditTlptx)
	ON_EN_CHANGE(IDC_EDIT_TLPTY, &CETMFCDlg::OnEnChangeEditTlpty)
	ON_EN_CHANGE(IDC_EDIT_DRPTX, &CETMFCDlg::OnEnChangeEditDrptx)
	ON_EN_CHANGE(IDC_EDIT_DRPTY, &CETMFCDlg::OnEnChangeEditDrpty)
	ON_BN_CLICKED(IDC_BUTTON_Play, &CETMFCDlg::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_BGSUBTRACTION, &CETMFCDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_SHOWALLTUBES, &CETMFCDlg::OnBnClickedButtonShowalltubes)
	ON_BN_CLICKED(IDC_BUTTON_ADDTUBE, &CETMFCDlg::OnBnClickedButtonAddtube)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CETMFCDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_TLXP1, &CETMFCDlg::OnBnClickedButtonTlxp1)
	ON_BN_CLICKED(IDC_BUTTON_TLXM1, &CETMFCDlg::OnBnClickedButtonTlxm1)
	ON_BN_CLICKED(IDC_BUTTON_TLYP1, &CETMFCDlg::OnBnClickedButtonTlyp1)
	ON_BN_CLICKED(IDC_BUTTON_TLYM1, &CETMFCDlg::OnBnClickedButtonTlym1)
	ON_BN_CLICKED(IDC_BUTTON_DRXP1, &CETMFCDlg::OnBnClickedButtonDrxp1)
	ON_BN_CLICKED(IDC_BUTTON_DRXM1, &CETMFCDlg::OnBnClickedButtonDrxm1)
	ON_BN_CLICKED(IDC_BUTTON_DRYP1, &CETMFCDlg::OnBnClickedButtonDryp1)
	ON_BN_CLICKED(IDC_BUTTON_DRYM1, &CETMFCDlg::OnBnClickedButtonDrym1)
END_MESSAGE_MAP()


// CETMFCDlg message handlers

BOOL CETMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	((CButton*)GetDlgItem(IDC_BUTTON_VIDEO_TO_IMAGES))->EnableWindow(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CETMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CETMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CETMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CETMFCDlg::OnBnClickedOpenImg()
{
	// TODO: Add your control notification handler code here
	CFileDialog fDlg(TRUE);

	CString fileName;

	if (fDlg.DoModal() == IDOK)
	{
		// initialize
		Helper::videoObject.initialize();
		cv::destroyAllWindows();

		fileName = fDlg.GetPathName();

		std::string videoFilePath;
		Helper::CStringtostring(fileName, Helper::videoObject.videopath);
		
		SetDlgItemText(IDC_EDIT_VIDEOPATH, fileName);

		cv::VideoCapture videoCap;
		videoCap.open(Helper::videoObject.videopath);

		if (!videoCap.isOpened())
		{
			MessageBox(L"Invalid video!");
			return;
		}

		int videoLen = videoCap.get(CV_CAP_PROP_FRAME_COUNT);
		int videoWidth = videoCap.get(CV_CAP_PROP_FRAME_WIDTH);
		int videoHeight = videoCap.get(CV_CAP_PROP_FRAME_HEIGHT);
		int videoframerate = videoCap.get(CV_CAP_PROP_FPS);

		videoCap.release();

		std::string videoLenStr = std::string("Video Length : ") + std::to_string(videoLen);
		std::string videoFrameRateStr = std::string("Frame rate (fps): ") + std::to_string(videoframerate);
		std::string videoResolutionStr = std::string("Resolution: ") + std::to_string(videoWidth) + " * " + std::to_string(videoHeight);

		SetDlgItemText(IDC_STATIC_VIDEOLEN, Helper::stringtoCString(videoLenStr));
		SetDlgItemText(IDC_STATIC_VIDEOFR, Helper::stringtoCString(videoFrameRateStr));
		SetDlgItemText(IDC_STATIC_VIDEORESOLUTION, Helper::stringtoCString(videoResolutionStr));

		SetDlgItemText(IDC_EDIT_VIDEOSCALE, L"1");
	}
}

void CETMFCDlg::showFrame(int frameID)
{
	if (frameID >= Helper::videoObject.allFrames.size())
	{
		frameID = Helper::videoObject.allFrames.size() - 1;
	}

	if (frameID < 0)
	{
		frameID = 0;
	}

	if (!Helper::videoObject.imagefolder.empty())
	{
		Helper::bInteractive = false;

		cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);

		void* userdata = (void*)&m_hWnd;
		cv::setMouseCallback(windowName, CallBackFunc, userdata);

		Helper::videoObject.currentFrame = frameID;
		std::string labelStr = std::string("Current frame/Total frame:") + std::to_string(frameID) + "/" + std::to_string(Helper::videoObject.allFrames.size() - 1);

		CWnd *label = GetDlgItem(IDC_STATIC_POS);
		label->SetWindowTextW(Helper::stringtoCString(labelStr));

		CSliderCtrl   *pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1);
		pSlidCtrl->SetPos(frameID);

		SetDlgItemText(IDC_EDIT_CURRENTIMGPATH, Helper::stringtoCString(Helper::videoObject.allFrames.at(frameID).filePath));

		cv::Mat frame = cv::imread(Helper::videoObject.allFrames.at(frameID).filePath);

		if (Helper::bDoBackGroundSubtraction)
		{
			cv::Mat fgMaskMOG;
			Helper::pMOG->apply(frame, fgMaskMOG);
			frame = fgMaskMOG;
		}

		if (Helper::bShowSelectedTube)
		{
			std::vector<Tube>::iterator it = Helper::videoObject.allTubes.end();
			it = Helper::videoObject.findTube(Helper::videoObject.currentTubeID);


			if (it != Helper::videoObject.allTubes.end() && it->tube_id != -1 && Helper::videoObject.currentFrame >= it->start_frame && Helper::videoObject.currentFrame <= it->end_frame)
			{
				int tubeElemIdx = frameID - it->start_frame;
				

				std::string text = std::to_string(it->tube_id);
				int fontFace = cv::FONT_HERSHEY_PLAIN;
				double fontScale = 2;
				int thickness = 2;
				cv::Scalar color = cv::Scalar(0, 0, 255);

				if (it->tubeElems.at(tubeElemIdx).isKeyFrame)
				{
					color = cv::Scalar(0,255,0);

					std::string tmpStr = std::string("Is Keyframe? ") + std::to_string(it->tubeElems.at(tubeElemIdx).isKeyFrame);
					SetDlgItemText(IDC_STATIC_ISKEYFRAME, Helper::stringtoCString(tmpStr));
				}
				else
				{
					std::string tmpStr = std::string("Is Keyframe? ") + std::to_string(it->tubeElems.at(tubeElemIdx).isKeyFrame);
					SetDlgItemText(IDC_STATIC_ISKEYFRAME, Helper::stringtoCString(tmpStr));
				}

				drawRectangle(frame, it->tubeElems.at(tubeElemIdx).tlpt, it->tubeElems.at(tubeElemIdx).drpt, color);

				cv::Point textOrg(mymin(it->tubeElems.at(tubeElemIdx).tlpt.x, it->tubeElems.at(tubeElemIdx).drpt.x), (it->tubeElems.at(tubeElemIdx).tlpt.y + it->tubeElems.at(tubeElemIdx).drpt.y) / 2);
				cv::putText(frame, text, textOrg, fontFace, fontScale,
					color, thickness, 8);

				// set candidate bounding box location
				SetDlgItemText(IDC_EDIT_TLPTX, Helper::stringtoCString(std::to_string(it->tubeElems.at(tubeElemIdx).tlpt.x)));
				SetDlgItemText(IDC_EDIT_TLPTY, Helper::stringtoCString(std::to_string(it->tubeElems.at(tubeElemIdx).tlpt.y)));
				SetDlgItemText(IDC_EDIT_DRPTX, Helper::stringtoCString(std::to_string(it->tubeElems.at(tubeElemIdx).drpt.x)));
				SetDlgItemText(IDC_EDIT_DRPTY, Helper::stringtoCString(std::to_string(it->tubeElems.at(tubeElemIdx).drpt.y)));

			}
		}
		else
		{
			bool bOneKeyframe = false;

			for (std::vector<Tube>::iterator it = Helper::videoObject.allTubes.begin(); it != Helper::videoObject.allTubes.end(); it++)
			{
				if (frameID >= it->start_frame && frameID <= it->end_frame)
				{
					int tubeElemIdx = frameID - it->start_frame;
					
					std::string text = std::to_string(it->tube_id);
					int fontFace = cv::FONT_HERSHEY_PLAIN;
					double fontScale = 2;
					int thickness = 2;
					cv::Scalar color = cv::Scalar(0, 0, 255);

					if (it->tubeElems.at(tubeElemIdx).isKeyFrame)
					{
						color = cv::Scalar(0, 255, 0);

						std::string tmpStr = std::string("Is Keyframe? ") + std::to_string(it->tubeElems.at(tubeElemIdx).isKeyFrame);
						SetDlgItemText(IDC_STATIC_ISKEYFRAME, Helper::stringtoCString(tmpStr));
						bOneKeyframe = true;
					}


					drawRectangle(frame, it->tubeElems.at(tubeElemIdx).tlpt, it->tubeElems.at(tubeElemIdx).drpt, color);

					cv::Point textOrg(mymin(it->tubeElems.at(tubeElemIdx).tlpt.x, it->tubeElems.at(tubeElemIdx).drpt.x), (it->tubeElems.at(tubeElemIdx).tlpt.y + it->tubeElems.at(tubeElemIdx).drpt.y) / 2);
					cv::putText(frame, text, textOrg, fontFace, fontScale,
						color, thickness, 8);

					if (Helper::videoObject.currentTubeID == it->tube_id)
					{
						// set candidate bounding box location
						SetDlgItemText(IDC_EDIT_TLPTX, Helper::stringtoCString(std::to_string(it->tubeElems.at(tubeElemIdx).tlpt.x)));
						SetDlgItemText(IDC_EDIT_TLPTY, Helper::stringtoCString(std::to_string(it->tubeElems.at(tubeElemIdx).tlpt.y)));
						SetDlgItemText(IDC_EDIT_DRPTX, Helper::stringtoCString(std::to_string(it->tubeElems.at(tubeElemIdx).drpt.x)));
						SetDlgItemText(IDC_EDIT_DRPTY, Helper::stringtoCString(std::to_string(it->tubeElems.at(tubeElemIdx).drpt.y)));
					}

				}
			}

			if (!bOneKeyframe)
			{
				// 一个keyframe都没有
				std::string tmpStr = std::string("Is Keyframe? 0");
				SetDlgItemText(IDC_STATIC_ISKEYFRAME, Helper::stringtoCString(tmpStr));
			}
		}


		cv::imshow(windowName, frame);
		cv::waitKey(30);

		Helper::bInteractive = true;
	}
}

void CETMFCDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CSliderCtrl   *pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1);
	Helper::videoObject.currentFrame = pSlidCtrl->GetPos();      //取得当前位置值 

	showFrame(Helper::videoObject.currentFrame);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CETMFCDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//Helper::videoObject.initialize();
	std::string tubeFile = Helper::videoObject.imagefolder + "\\tubeFile.txt";
	//ofstream outfile(tubeFile, ios::binary);
	//boost::archive::binary_oarchive oa(outfile);
	//oa << Helper::videoObject;
	//outfile.close();

	Helper::videoObject.saveInfo(tubeFile);

	MessageBox(L"Save tube info.");

	//CDialogEx::OnOK();
}


void CETMFCDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here

	//Helper::videoObject.initialize();

	std::string tubeFile = Helper::videoObject.imagefolder + "\\tubeFile.txt";
	//ofstream outfile(tubeFile, ios::binary);
	//boost::archive::binary_oarchive oa(outfile);
	//oa << Helper::videoObject;
	//outfile.close();

	Helper::videoObject.saveInfo(tubeFile);

	CDialogEx::OnCancel();
}


void CETMFCDlg::OnExitSizeMove()
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnExitSizeMove();
}


void CETMFCDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	// Helper::videoObject.initialize();

	std::string tubeFile = Helper::videoObject.imagefolder + "\\tubeFile.txt";
	ofstream outfile(tubeFile, ios::binary);
	boost::archive::binary_oarchive oa(outfile);
	oa << Helper::videoObject;
	outfile.close();

	MessageBox(L"Tube info. saved");

	CDialogEx::OnClose();
}



void CETMFCDlg::OnBnClickedButtonImagefolder()
{
	// TODO: Add your control notification handler code here
	if (!Helper::videoObject.videopath.empty())
	{
		std::string defaultDir = Helper::videoObject.videopath.substr(0, Helper::videoObject.videopath.find_last_of('\\'));
		CString dirPath = Helper::BrowseForFolder(this->m_hWnd, L"Select Folder", Helper::stringtoCString(defaultDir));

		SetDlgItemText(IDC_EDIT_CVTIMAGEFOLDER, dirPath);

		Helper::videoObject.imagefolder = Helper::CStringtostring(dirPath);

		((CButton*)GetDlgItem(IDC_BUTTON_VIDEO_TO_IMAGES))->EnableWindow(TRUE);
	}
	else
	{
		MessageBox(L"Please choose a video!");
	}
}


void CETMFCDlg::OnBnClickedButtonGotoframe()
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItemText(IDC_EDIT1, str);

	std::string str2;
	Helper::CStringtostring(str, str2);

	int frameID = std::stoi(str2);

	showFrame(frameID);
}


void CETMFCDlg::OnBnClickedButtonVideoToImages()
{
	// TODO: Add your control notification handler code here
	char windowName[] = "This is a demo";

	int numOfFrame = 0;

	CString startFramecstr;
	GetDlgItemText(IDC_EDIT_VIDEO_STARTFRAME, startFramecstr);
	std::string startFramestr;
	Helper::CStringtostring(startFramecstr, startFramestr);

	if (startFramestr.empty())
	{
		MessageBox(L"Please input start frame.");
		return;
	}

	CString videoLengthcstr;
	GetDlgItemText(IDC_EDIT_CVTVIDEOLENTH, videoLengthcstr);
	std::string videoLengthstr;
	Helper::CStringtostring(videoLengthcstr, videoLengthstr);

	if (videoLengthstr.empty())
	{
		MessageBox(L"Please input the expect video length.");
		return;
	}

	int startFrame = std::atoi(startFramestr.c_str());
	int videoLen = std::atoi(videoLengthstr.c_str());

	cv::VideoCapture videoCap;
	videoCap.open(Helper::videoObject.videopath);

	videoCap.set(CV_CAP_PROP_POS_FRAMES, startFrame);

	CString scaleCStr;
	GetDlgItemText(IDC_EDIT_VIDEOSCALE, scaleCStr);
	std::string scaleStr = Helper::CStringtostring(scaleCStr);
	double dScale = std::atof(scaleStr.c_str());

	if (dScale <= 0)
	{
		std::string err = std::string("scale error: ") + scaleStr;
		MessageBox(Helper::stringtoCString(err));
		return;
	}

	cv::Size S = cv::Size((int)videoCap.get(CV_CAP_PROP_FRAME_WIDTH) * dScale,    // Acquire input size
		(int)videoCap.get(CV_CAP_PROP_FRAME_HEIGHT) * dScale);

	std::string videoResolutionStr = std::string("Resolution: ") + std::to_string(S.width) + " * " + std::to_string(S.height);
	SetDlgItemText(IDC_STATIC_VIDEORESOLUTION, Helper::stringtoCString(videoResolutionStr));
	
	cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);

	int skipFrameNum = videoLen / 100;
	skipFrameNum = skipFrameNum > 1 ? skipFrameNum : 1;

	cv::Mat tmpFrame, frame;

	const float feps = 1e-3;
	while (videoCap.read(tmpFrame) && numOfFrame < videoLen)
	{
		char numOfFrameStr[10];
		sprintf_s(numOfFrameStr, "%08d", numOfFrame);

		std::string imagePath = Helper::videoObject.imagefolder + "/" + std::string(numOfFrameStr) + ".png";

		if (fabsf(dScale - 1) > feps)
		{
			cv::resize(tmpFrame, frame, S);
		}
		else
		{
			frame = tmpFrame;
		}

		cv::imwrite(imagePath, frame);

		if (numOfFrame % skipFrameNum == 0)

		{
			cv::imshow(windowName, frame);
			cv::waitKey(1);
		}

		numOfFrame++;
	}

	videoCap.release();
	cv::destroyWindow(windowName);

	MessageBox(L"Finish conversion!");
}

struct MyComparator
{
	const std::vector<std::string> & value_vector;

	MyComparator(const std::vector<std::string> & val_vec) :
		value_vector(val_vec) {}

	bool operator()(std::string str1, std::string str2)
	{
		std::string str1name = str1.substr(str1.find_last_of('\\') + 1, str1.find_last_of('.'));
		std::string str2name = str2.substr(str2.find_last_of('\\') + 1, str2.find_last_of('.'));

		int ti1 = std::atoi(str1name.c_str());
		int ti2 = std::atoi(str2name.c_str());

		return ti1 < ti2;
	}
};

void CETMFCDlg::OnBnClickedButtonImagefolderEt()
{
	// TODO: Add your control notification handler code here

	if (!Helper::videoObject.imagefolder.empty())
	{
		std::string tubeFile = Helper::videoObject.imagefolder + "\\tubeFile.txt";
		Helper::videoObject.saveInfo(tubeFile);
		MessageBox(L"Save current tube info. before open new folder");
	}


	CString imagefolderDir;
	GetDlgItemText(IDC_EDIT_CVTIMAGEFOLDER, imagefolderDir);
	
	std::string defaultDir;
	if (imagefolderDir.GetLength() == 0)
	{
		defaultDir = "D:\\Projects\\VideoSynopsis\\VideosSameScene4\\left";
	}
	else
	{
		defaultDir = Helper::CStringtostring(imagefolderDir);
	}
	
	CString dirPath = Helper::BrowseForFolder(this->m_hWnd, L"Select Folder", Helper::stringtoCString(defaultDir));

	Helper::videoObject.initialize();

	Helper::videoObject.imagefolder = Helper::CStringtostring(dirPath);
	SetDlgItemText(IDC_EDIT_IMAGEFOLDER, dirPath);

	//cv::Mat fgMaskMOG, bgImg;
	//std::string bgImgPath = Helper::videoObject.imagefolder + "/BG.png";
	//bgImg = cv::imread(bgImgPath);
	//Helper::pMOG->apply(bgImg, fgMaskMOG);
	std::vector<std::string> allFiles;
	int frameID = 0;

	// load file
	std::string tubeFile = Helper::videoObject.imagefolder + "\\tubeFile.txt";
	if (CmFile::FileExist(tubeFile))
	{
		//ifstream infile(tubeFile, ios::binary);
		//boost::archive::binary_iarchive ia(infile);
		//ia >> Helper::videoObject;
		//infile.close();

		Helper::videoObject.loadInfo(tubeFile);

		SetDlgItemText(IDC_EDIT_WHICHTUBE, Helper::stringtoCString(std::to_string(Helper::videoObject.currentTubeID)));
		SetDlgItemText(IDC_BUTTON_ADDTUBE, L"Modify tube");

		std::string tubeIDs = "";
		for (size_t i = 0; i < Helper::videoObject.allTubes.size(); i++)
		{
			tubeIDs += (std::to_string(Helper::videoObject.allTubes.at(i).tube_id) + ", ");
		}
		SetDlgItemText(IDC_EDIT_TUBEIDS, Helper::stringtoCString(tubeIDs));
		
		MessageBox(L"Load tube info.");
	}
	else
	{
		Helper::videoObject.initialize();

		
		SetDlgItemText(IDC_EDIT_WHICHTUBE,L"");

		//		OnEnChangeEditWhichtube();

		allFiles.clear();
		Helper::GetFilesInDirectory(allFiles, Helper::videoObject.imagefolder);
		std::sort(allFiles.begin(), allFiles.end(), MyComparator(allFiles));

		

		// initialize frame information
		for (std::vector<std::string>::iterator it = allFiles.begin(); it != allFiles.end(); it++)
		{
			FrameElem frameElem;
			frameElem.frame_id = frameID;
			frameElem.filePath = *it;

			Helper::videoObject.allFrames.push_back(frameElem);
			frameID++;
		}
	}

	std::string labelStr = std::string("Tube num: ") + std::to_string(Helper::videoObject.allTubes.size());
	SetDlgItemText(IDC_STATIC_TUBENUM, Helper::stringtoCString(labelStr));

	

	cv::Mat tmpImg = cv::imread(Helper::videoObject.allFrames.at(0).filePath);
	std::string imageResolutionStr = std::string("Resolution: ") + std::to_string(tmpImg.cols) + " * " + std::to_string(tmpImg.rows);
	SetDlgItemText(IDC_STATIC_IMAGE_RESOLUTION, Helper::stringtoCString(imageResolutionStr));

	Helper::imageHeight = tmpImg.rows;
	Helper::imageWidth = tmpImg.cols;

	std::string imageNumStr = std::string("Image number: ") + std::to_string(frameID);
	SetDlgItemText(IDC_STATIC_IMAGESETSIZE, Helper::stringtoCString(imageNumStr));

	m_Slider.SetRangeMin(0);
	m_Slider.SetRangeMax(Helper::videoObject.allFrames.size() - 1);
	m_Slider.SetTicFreq(1);

	showFrame(Helper::videoObject.currentFrame);
}


void CETMFCDlg::OnBnClickedButtonPreframe()
{
	// TODO: Add your control notification handler code here

	int frameID = Helper::videoObject.currentFrame - 1;
	showFrame(frameID);
}


void CETMFCDlg::OnBnClickedButtonP10frames()
{
	// TODO: Add your control notification handler code here
	int frameID = Helper::videoObject.currentFrame - 10;
	showFrame(frameID);
}


void CETMFCDlg::OnBnClickedButtonP100frames()
{
	// TODO: Add your control notification handler code here
	int frameID = Helper::videoObject.currentFrame - 100;
	showFrame(frameID);
}


void CETMFCDlg::OnBnClickedButtonNextframe()
{
	// TODO: Add your control notification handler code here
	int frameID = Helper::videoObject.currentFrame + 1;
	showFrame(frameID);
}


void CETMFCDlg::OnBnClickedButtonN10frames()
{
	// TODO: Add your control notification handler code here
	int frameID = Helper::videoObject.currentFrame + 10;
	showFrame(frameID);
}


void CETMFCDlg::OnBnClickedButtonN100frames()
{
	// TODO: Add your control notification handler code here
	int frameID = Helper::videoObject.currentFrame + 100;
	showFrame(frameID);
}



void CETMFCDlg::OnBnClickedButtonEdittube()
{
	// TODO: Add your control notification handler code here

	CWnd *label = GetDlgItem(IDC_STATIC_TUBENUM);
	std::string labelStr = std::string("Tube num: ") + std::to_string(Helper::videoObject.allTubes.size());
	label->SetWindowTextW(Helper::stringtoCString(labelStr));

	CString tubeIDcstr;
	GetDlgItemText(IDC_EDIT_WHICHTUBE, tubeIDcstr);

	std::string tubeIDstr = Helper::CStringtostring(tubeIDcstr);

	if (tubeIDstr.empty())
	{
		MessageBox(L"Please specify a tube ID.");
		return;
	}

	int tubeID = std::atoi(tubeIDstr.c_str());
	std::vector<Tube>::iterator it = Helper::videoObject.allTubes.end();
	it = Helper::videoObject.findTube(tubeID);

	if (it == Helper::videoObject.allTubes.end())
	{
		MessageBox(L"This tube doesn't exist, please choose a valid tube ID.");
		return;
	}

	if (Helper::videoObject.currentFrame < it->start_frame || Helper::videoObject.currentFrame > it->end_frame)
	{
		MessageBox(L"The tube does appear in this frame.");
		return;
	}

	CString tmp1, tmp2, tmp3, tmp4;
	GetDlgItemText(IDC_EDIT_TLPTX, tmp1);
	GetDlgItemText(IDC_EDIT_TLPTY, tmp2);
	GetDlgItemText(IDC_EDIT_DRPTX, tmp3);
	GetDlgItemText(IDC_EDIT_DRPTY, tmp4);

	if (tmp1.GetLength() == 0 || tmp2.GetLength() == 0 || tmp3.GetLength() == 0 || tmp4.GetLength() == 0)
	{
		MessageBox(L"Top left point and down right point cannot be empty");
		return;
	}

	int tubeElemIdx = Helper::videoObject.currentFrame - it->start_frame;
	//for (tubeElemIdx = 0; tubeElemIdx < it->tubeElems.size(); tubeElemIdx++)
	//	if (Helper::videoObject.currentFrame == it->tubeElems.at(tubeElemIdx).current_frame_id)
	//		break;

	TubeElem& tubeElem = it->tubeElems.at(tubeElemIdx);
	
	if (!tubeElem.isKeyFrame)
	{
		tubeElem.isKeyFrame = true;
		it->keyframe_num++;
		tubeElem.tlpt = Helper::tlpt;
		tubeElem.drpt = Helper::drpt;
		MessageBox(L"Add one keyframe to tube.");
	}
	else
	{
		tubeElem.tlpt = Helper::tlpt;
		tubeElem.drpt = Helper::drpt;
		MessageBox(L"Modify current keyframe.");
	}

	OnEnChangeEditWhichtube();

	
	if (it->keyframe_num > 1)
	{
		// find all keyframes' indecis
		std::vector<int> keyframeIdxVec;
		keyframeIdxVec.clear();
		keyframeIdxVec.resize(it->keyframe_num);

		int vecIdx = 0;
		for (int tubeElemIdx = 0; tubeElemIdx < it->tubeElems.size(); tubeElemIdx++)
		{
			if (it->tubeElems.at(tubeElemIdx).isKeyFrame)
			{
				keyframeIdxVec.at(vecIdx++) = tubeElemIdx;
			}
		}

		for (int i = 0; i < it->keyframe_num - 1; i++)
		{
			int fromIdx = keyframeIdxVec.at(i);
			int toIdx = keyframeIdxVec.at(i + 1);

			cv::Point2f tlStep = (it->tubeElems.at(toIdx).tlpt - it->tubeElems.at(fromIdx).tlpt) / (1.0f*(toIdx - fromIdx));
			cv::Point2f drStep = (it->tubeElems.at(toIdx).drpt - it->tubeElems.at(fromIdx).drpt) / (1.0f*(toIdx - fromIdx));

			// update new position
			for (int j = fromIdx + 1; j < toIdx; j++)
			{
				it->tubeElems.at(j).tlpt = it->tubeElems.at(fromIdx).tlpt + (j - fromIdx) * tlStep;
				it->tubeElems.at(j).drpt = it->tubeElems.at(fromIdx).drpt + (j - fromIdx) * drStep;
			}
		}

	}


	showFrame(Helper::videoObject.currentFrame);
	

	//tube.tubeElems.push_back(tubeElem);


	//labelStr = std::string("Tube num: ") + std::to_string(Helper::videoObject.allTubes.size());
	//SetDlgItemText(IDC_STATIC_TUBENUM, Helper::stringtoCString(labelStr));
}


void CETMFCDlg::OnBnClickedButtonShowselecttube()
{
	// TODO: Add your control notification handler code here
	Helper::bShowSelectedTube = !Helper::bShowSelectedTube;

	if (Helper::bShowSelectedTube)
	{
		std::string tmp = "Show all tubes";
		SetDlgItemText(IDC_BUTTON_SHOWSELECTTUBE, Helper::stringtoCString(tmp));
	}
	else
	{
		std::string tmp = "Show selected tube";
		SetDlgItemText(IDC_BUTTON_SHOWSELECTTUBE, Helper::stringtoCString(tmp));
	}

	showFrame(Helper::videoObject.currentFrame);
}


void CETMFCDlg::OnBnClickedButtonShowcandidate()
{
	// TODO: Add your control notification handler code here

	CWnd *label = GetDlgItem(IDC_STATIC_TUBENUM);
	std::string labelStr = std::string("Tube num: ") + std::to_string(Helper::videoObject.allTubes.size());
	label->SetWindowTextW(Helper::stringtoCString(labelStr));


	cv::Mat img = cv::imread(Helper::videoObject.allFrames.at(Helper::videoObject.currentFrame).filePath);
	cv::Scalar color(0,0,255);
	
	CString tmpcstr;
	GetDlgItemText(IDC_EDIT_TLPTX, tmpcstr);
	Helper::tlpt.x = std::atoi(Helper::CStringtostring(tmpcstr).c_str());

	GetDlgItemText(IDC_EDIT_TLPTY, tmpcstr);
	Helper::tlpt.y = std::atoi(Helper::CStringtostring(tmpcstr).c_str());

	GetDlgItemText(IDC_EDIT_DRPTX, tmpcstr);
	Helper::drpt.x = std::atoi(Helper::CStringtostring(tmpcstr).c_str());

	GetDlgItemText(IDC_EDIT_DRPTY, tmpcstr);
	Helper::drpt.y = std::atoi(Helper::CStringtostring(tmpcstr).c_str());
	
	cv::rectangle(img, Helper::tlpt, Helper::drpt, color);

	cv::imshow(windowName, img);
}


void CETMFCDlg::OnEnChangeEditWhichtube()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	CString tubeIDcstr;
	GetDlgItemText(IDC_EDIT_WHICHTUBE, tubeIDcstr);

	std::string tubeIDstr = Helper::CStringtostring(tubeIDcstr);

	if (tubeIDstr.empty())
	{
		//MessageBox(L"Please specify a tube ID.");
		SetDlgItemText(IDC_STATIC_TUBEID, L"Tube ID:");
		SetDlgItemText(IDC_STATIC_STARTFRAME, L"Start frame:");
		SetDlgItemText(IDC_STATIC_ENDFRAME, L"End frame:");
		SetDlgItemText(IDC_STATIC_KEYFRAMENUM, L"Keyframe num:");
		SetDlgItemText(IDC_STATIC_ISKEYFRAME, L"Is Keyframe?");

		return;
	}

	int tubeID = std::atoi(tubeIDstr.c_str());
	std::vector<Tube>::iterator it = Helper::videoObject.allTubes.end();
	it = Helper::videoObject.findTube(tubeID);

	if (it != Helper::videoObject.allTubes.end())
	{
		Helper::videoObject.currentTubeID = tubeID;
		std::string tmpStr = std::string("Tube ID: ") + std::to_string(it->tube_id);
		SetDlgItemText(IDC_STATIC_TUBEID, Helper::stringtoCString(tmpStr));
		tmpStr = std::string("Start frame: ") + std::to_string(it->start_frame);
		SetDlgItemText(IDC_STATIC_STARTFRAME, Helper::stringtoCString(tmpStr));
		tmpStr = std::string("End frame: ") + std::to_string(it->end_frame);
		SetDlgItemText(IDC_STATIC_ENDFRAME, Helper::stringtoCString(tmpStr));
		tmpStr = std::string("Keyframe num: ") + std::to_string(it->keyframe_num);
		SetDlgItemText(IDC_STATIC_KEYFRAMENUM, Helper::stringtoCString(tmpStr));

		int tubeElemIdx = Helper::videoObject.currentFrame - it->start_frame;

		tmpStr = std::string("Is Keyframe? ") + std::to_string(it->tubeElems.at(tubeElemIdx).isKeyFrame);
		SetDlgItemText(IDC_STATIC_ISKEYFRAME, Helper::stringtoCString(tmpStr));

		SetDlgItemText(IDC_BUTTON_ADDTUBE, L"Modify tube");

	}
	else
	{
		SetDlgItemText(IDC_STATIC_TUBEID, L"Tube ID: no");
		SetDlgItemText(IDC_STATIC_STARTFRAME, L"Start frame: no");
		SetDlgItemText(IDC_STATIC_ENDFRAME, L"End frame: no");
		SetDlgItemText(IDC_STATIC_KEYFRAMENUM, L"Keyframe num: no");
		SetDlgItemText(IDC_STATIC_ISKEYFRAME, L"Is Keyframe? no");

		SetDlgItemText(IDC_BUTTON_ADDTUBE, L"Add tube");
	}
}


void CETMFCDlg::OnEnChangeEditTlptx()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString TLPTXCStr;
	GetDlgItemText(IDC_EDIT_TLPTX, TLPTXCStr);

	if (TLPTXCStr.GetLength() == 0)
	{
		return;
	}

	Helper::tlpt.x = std::atoi(Helper::CStringtostring(TLPTXCStr).c_str());
	if (Helper::bInteractive)
	{
		drawRectangle();
	}
	
}


void CETMFCDlg::OnEnChangeEditTlpty()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString TLPTYCStr;
	GetDlgItemText(IDC_EDIT_TLPTY, TLPTYCStr);

	if (TLPTYCStr.GetLength() == 0)
	{
		return;
	}

	Helper::tlpt.y = std::atoi(Helper::CStringtostring(TLPTYCStr).c_str());
	if (Helper::bInteractive)
	{
		drawRectangle();
	}
}


void CETMFCDlg::OnEnChangeEditDrptx()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString DRPTXCStr;
	GetDlgItemText(IDC_EDIT_DRPTX, DRPTXCStr);

	if (DRPTXCStr.GetLength() == 0)
	{
		return;
	}

	Helper::drpt.x = std::atoi(Helper::CStringtostring(DRPTXCStr).c_str());
	if (Helper::bInteractive)
	{
		drawRectangle();
	}
}


void CETMFCDlg::OnEnChangeEditDrpty()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString DRPTYCStr;
	GetDlgItemText(IDC_EDIT_DRPTY, DRPTYCStr);

	if (DRPTYCStr.GetLength() == 0)
	{
		return;
	}

	Helper::drpt.y = std::atoi(Helper::CStringtostring(DRPTYCStr).c_str());
	if (Helper::bInteractive)
	{
		drawRectangle();
	}
}


void CETMFCDlg::OnBnClickedButtonPlay()
{
	// TODO: Add your control notification handler code here
	const int num = Helper::videoObject.allFrames.size();
	Helper::bStartPlay = true;
	for (int i = Helper::videoObject.currentFrame; i < num; i++)
	{
		if (!Helper::bStartPlay)
		{
			break;
		}
		showFrame(i);
	}
		
}


void CETMFCDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	Helper::bDoBackGroundSubtraction = !Helper::bDoBackGroundSubtraction;
	if (Helper::bDoBackGroundSubtraction)
	{
		SetDlgItemText(IDC_BUTTON_BGSUBTRACTION, L"Show Color Image");
	}
	else
	{
		SetDlgItemText(IDC_BUTTON_BGSUBTRACTION, L"Background Subtraction");
	}
	showFrame(Helper::videoObject.currentFrame);
}

void CETMFCDlg::OnBnClickedButtonShowalltubes()
{
	// TODO: Add your control notification handler code here

	std::string tubeIDs = "";
	for (size_t i = 0; i < Helper::videoObject.allTubes.size(); i++)
	{
		tubeIDs += (std::to_string(Helper::videoObject.allTubes.at(i).tube_id) + ", ");
	}
	SetDlgItemText(IDC_EDIT_TUBEIDS, Helper::stringtoCString(tubeIDs));
}


void CETMFCDlg::OnBnClickedButtonAddtube()
{
	// TODO: Add your control notification handler code here
	CString tmpCStrID, tmpCStrStart, tmpCStrEnd;
	GetDlgItemText(IDC_EDIT_WHICHTUBE, tmpCStrID);
	GetDlgItemText(IDC_EDIT_TUBE_STARTFRAME, tmpCStrStart);
	GetDlgItemText(IDC_EDIT_TUBE_ENDFRAME, tmpCStrEnd);

	if (tmpCStrID.GetLength() == 0)
	{
		MessageBox(L"Please input tube ID!");
		return;
	}

	if (tmpCStrStart.GetLength() == 0)
	{
		MessageBox(L"Please input start frame id!");
		return;
	}

	if (tmpCStrEnd.GetLength() == 0)
	{
		MessageBox(L"Please input end frame id!");
		return;
	}

	std::string tubeIDstr = Helper::CStringtostring(tmpCStrID);

	int tubeID = std::atoi(tubeIDstr.c_str());
	std::vector<Tube>::iterator it = Helper::videoObject.allTubes.end();
	it = Helper::videoObject.findTube(tubeID);
	bool bModifyTube = false;
	if (it != Helper::videoObject.allTubes.end())
	{
		if (IDNO == MessageBox(L"The tube already exists! Do you want to modify the start and end frame of the tube?", L"", MB_YESNO))
			return;

		bModifyTube = true;
	}

	Tube tube;
	tube.tube_id = tubeID;

	CString tmpCStr;
	GetDlgItemText(IDC_EDIT_TUBE_STARTFRAME, tmpCStr);
	tube.start_frame = std::atoi(Helper::CStringtostring(tmpCStr).c_str());
	GetDlgItemText(IDC_EDIT_TUBE_ENDFRAME, tmpCStr);
	tube.end_frame = std::atoi(Helper::CStringtostring(tmpCStr).c_str());

	if (tube.start_frame > tube.end_frame || tube.start_frame < 0 || tube.end_frame > Helper::videoObject.allFrames.size() - 1)
	{
		MessageBox(L"Start/End frame id error!");
		return;
	}

	tube.tubeElems.resize(tube.end_frame - tube.start_frame + 1);	
	for (size_t i = tube.start_frame; i <= tube.end_frame; i++)
	{
		tube.tubeElems.at(i - tube.start_frame).current_frame_id = i;
		tube.tubeElems.at(i - tube.start_frame).current_frame_id = tube.tube_id;
	}

	GetDlgItemText(IDC_BUTTON_ADDTUBE, tmpCStr);
	if (bModifyTube)
	{
		// modify tube	
		int maxStartFrame = mymax(it->start_frame, tube.start_frame);
		int minEndFrame = mymin(it->end_frame, tube.end_frame);

		for (size_t i = maxStartFrame; i <= minEndFrame; i++)
		{
			tube.tubeElems.at(i - tube.start_frame).isKeyFrame = it->tubeElems.at(i - it->start_frame).isKeyFrame;
			tube.tubeElems.at(i - tube.start_frame).tlpt = it->tubeElems.at(i - it->start_frame).tlpt;
			tube.tubeElems.at(i - tube.start_frame).drpt = it->tubeElems.at(i - it->start_frame).drpt;
			if (tube.tubeElems.at(i - tube.start_frame).isKeyFrame)
			{
				tube.keyframe_num++;
			}
		}

		Helper::videoObject.allTubes.erase(it);
		Helper::videoObject.allTubes.push_back(tube);

		MessageBox(L"One object is modified!");
	}
	else
	{
		// add tube
		Helper::videoObject.allTubes.push_back(tube);
		MessageBox(L"One object is inserted!");
	}

	std::string labelStr = std::string("Tube num: ") + std::to_string(Helper::videoObject.allTubes.size());
	SetDlgItemText(IDC_STATIC_TUBENUM, Helper::stringtoCString(labelStr));	
	OnEnChangeEditWhichtube();

	std::string tubeIDs = "";
	for (size_t i = 0; i < Helper::videoObject.allTubes.size(); i++)
	{
		tubeIDs += (std::to_string(Helper::videoObject.allTubes.at(i).tube_id) + ", ");
	}
	SetDlgItemText(IDC_EDIT_TUBEIDS, Helper::stringtoCString(tubeIDs));
	
}


void CETMFCDlg::OnBnClickedButtonStop()
{
	// TODO: Add your control notification handler code here
	Helper::bStartPlay = false;
}


void CETMFCDlg::OnBnClickedButtonTlxp1()
{
	// TODO: Add your control notification handler code here

	CString tmpCStr;
	std::string tmpStr;
	GetDlgItemText(IDC_EDIT_TLPTX, tmpCStr);
	float x = std::atof(Helper::CStringtostring(tmpCStr).c_str());

	x = x + 1 < Helper::imageWidth - 1 ? x + 1 : Helper::imageWidth - 1;

	SetDlgItemText(IDC_EDIT_TLPTX, Helper::stringtoCString(std::to_string(x)));
}


void CETMFCDlg::OnBnClickedButtonTlxm1()
{
	// TODO: Add your control notification handler code here
	CString tmpCStr;
	std::string tmpStr;
	GetDlgItemText(IDC_EDIT_TLPTX, tmpCStr);
	float x = std::atof(Helper::CStringtostring(tmpCStr).c_str());

	x = x - 1 > 0 ? x - 1 : 0;
	SetDlgItemText(IDC_EDIT_TLPTX, Helper::stringtoCString(std::to_string(x)));
}


void CETMFCDlg::OnBnClickedButtonTlyp1()
{
	// TODO: Add your control notification handler code here
	CString tmpCStr;
	std::string tmpStr;
	GetDlgItemText(IDC_EDIT_TLPTY, tmpCStr);
	float x = std::atof(Helper::CStringtostring(tmpCStr).c_str());

	x = x + 1 < Helper::imageHeight - 1 ? x + 1 : Helper::imageHeight - 1;

	SetDlgItemText(IDC_EDIT_TLPTY, Helper::stringtoCString(std::to_string(x)));
}


void CETMFCDlg::OnBnClickedButtonTlym1()
{
	// TODO: Add your control notification handler code here
	CString tmpCStr;
	std::string tmpStr;
	GetDlgItemText(IDC_EDIT_TLPTY, tmpCStr);
	float x = std::atof(Helper::CStringtostring(tmpCStr).c_str());

	x = x - 1 > 0 ? x - 1 : 0;
	SetDlgItemText(IDC_EDIT_TLPTY, Helper::stringtoCString(std::to_string(x)));
}


void CETMFCDlg::OnBnClickedButtonDrxp1()
{
	// TODO: Add your control notification handler code here
	CString tmpCStr;
	std::string tmpStr;
	GetDlgItemText(IDC_EDIT_DRPTX, tmpCStr);
	float x = std::atof(Helper::CStringtostring(tmpCStr).c_str());

	x = x + 1 < Helper::imageWidth - 1 ? x + 1 : Helper::imageWidth - 1;

	SetDlgItemText(IDC_EDIT_DRPTX, Helper::stringtoCString(std::to_string(x)));
}


void CETMFCDlg::OnBnClickedButtonDrxm1()
{
	// TODO: Add your control notification handler code here
	CString tmpCStr;
	std::string tmpStr;
	GetDlgItemText(IDC_EDIT_DRPTX, tmpCStr);
	float x = std::atof(Helper::CStringtostring(tmpCStr).c_str());

	x = x - 1 > 0 ? x - 1 : 0;
	SetDlgItemText(IDC_EDIT_DRPTX, Helper::stringtoCString(std::to_string(x)));
}


void CETMFCDlg::OnBnClickedButtonDryp1()
{
	// TODO: Add your control notification handler code here
	CString tmpCStr;
	std::string tmpStr;
	GetDlgItemText(IDC_EDIT_DRPTY, tmpCStr);
	float x = std::atof(Helper::CStringtostring(tmpCStr).c_str());
	x = x + 1 < Helper::imageHeight - 1 ? x + 1 : Helper::imageHeight - 1;

	SetDlgItemText(IDC_EDIT_DRPTY, Helper::stringtoCString(std::to_string(x)));
}


void CETMFCDlg::OnBnClickedButtonDrym1()
{
	// TODO: Add your control notification handler code here
	CString tmpCStr;
	std::string tmpStr;
	GetDlgItemText(IDC_EDIT_DRPTY, tmpCStr);
	float x = std::atof(Helper::CStringtostring(tmpCStr).c_str());

	x = x - 1 > 0 ? x - 1 : 0;
	SetDlgItemText(IDC_EDIT_DRPTY, Helper::stringtoCString(std::to_string(x)));
}
