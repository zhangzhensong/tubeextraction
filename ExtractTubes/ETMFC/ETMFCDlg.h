
// ETMFCDlg.h : header file
//

#pragma once

#include <opencv2\opencv.hpp>
#include "afxcmn.h"


// CETMFCDlg dialog
class CETMFCDlg : public CDialogEx
{
// Construction
public:
	CETMFCDlg(CWnd* pParent = NULL);	// standard constructor
	void showFrame(int frameID);

// Dialog Data
	enum { IDD = IDD_ETMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CSliderCtrl m_Slider;



	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpenImg();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnExitSizeMove();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonImagefolder();
	afx_msg void OnBnClickedButtonGotoframe();
	afx_msg void OnBnClickedButtonVideoToImages();
	afx_msg void OnBnClickedButtonImagefolderEt();
	afx_msg void OnBnClickedButtonPreframe();
	afx_msg void OnBnClickedButtonP10frames();
	afx_msg void OnBnClickedButtonP100frames();
	afx_msg void OnBnClickedButtonNextframe();
	afx_msg void OnBnClickedButtonN10frames();
	afx_msg void OnBnClickedButtonN100frames();
	afx_msg void OnBnClickedButtonEdittube();
	afx_msg void OnBnClickedButtonShowselecttube();
	afx_msg void OnBnClickedButtonShowcandidate();
	afx_msg void OnEnChangeEditWhichtube();
	afx_msg void OnEnChangeEditTlptx();
	afx_msg void OnEnChangeEditTlpty();
	afx_msg void OnEnChangeEditDrptx();
	afx_msg void OnEnChangeEditDrpty();
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButtonShowalltubes();
	afx_msg void OnBnClickedButtonAddtube();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonTlxp1();
	afx_msg void OnBnClickedButtonTlxm1();
	afx_msg void OnBnClickedButtonTlyp1();
	afx_msg void OnBnClickedButtonTlym1();
	afx_msg void OnBnClickedButtonDrxp1();
	afx_msg void OnBnClickedButtonDrxm1();
	afx_msg void OnBnClickedButtonDryp1();
	afx_msg void OnBnClickedButtonDrym1();
};
