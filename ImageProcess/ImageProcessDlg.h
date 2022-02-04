
// ImageProcessDlg.h : ��� ����
//
#pragma once
///////Header Include
//Image Process
#include "cv.h"
#include "highgui.h"
#include "Parameter.h"
#include "CvvImage.h"
//Communication
#include "Comm.h"

// CImageProcessDlg ��ȭ ����
class CImageProcessDlg : public CDialogEx
{
	// �����Դϴ�.
public:
	CImageProcessDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	void CreateThread1(UINT _method);
	bool DestoryThread1(void);
	int Thread1Function(void);
	//Image Process
	void Init_ImageProcess(CvSize img_size);
	void ImageProcess();
	//labelling
	void Labelling();
	// Serial Comm
	TTYSTRUCT Int2TTY();		//Init Com fucntion
	TTYSTRUCT LoadSettings();	//Load Previous setting
	void SaveSettings();
	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_IMAGEPROCESS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

public:
	CWinThread* pThread1;
	static volatile bool isThread1Running;

	//Threshold Parameter
	int m_th_under_H;
	int m_th_under_S;
	int m_th_upper_H;
	int m_th_upper_S;
	////Ball
	int m_th_ball_H_L;
	int m_th_ball_H_H;
	int m_th_ball_S_L;
	int m_th_ball_S_H;
	////Color  (RGB)
	int m_th_red_H_L;
	int m_th_red_H_H;
	int m_th_red_S_L;
	int m_th_red_S_H;
	int m_th_blue_H_L;
	int m_th_blue_H_H;
	int m_th_blue_S_L;
	int m_th_blue_S_H;
	int m_th_green_H_L;
	int m_th_green_H_H;
	int m_th_green_S_L;
	int m_th_green_S_H;
	// �����Դϴ�.
protected:
	HICON m_hIcon;
	CComm m_Comm;

	//Add Here
	//Image Matrix
	IplImage* img_Cam = 0;
	IplImage* img_Process = 0;
	IplImage* img_HSV = 0;
	IplImage* img_Gray = 0;
	IplImage* img_Label = 0;
	IplImage* img_Show = 0;

	IplImage* img_Img = 0;
	IplImage* img_Img2 = 0;
	IplImage* img_Img3 = 0;
	IplImage* img_Img4 = 0;
	IplImage* img_Img5 = 0;
	// ������ �޽��� �� �Լ�

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	//Button Control
public:
	int m_index_color;

	//Button Click
	afx_msg void OnBnClickedThread1Start();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedThread1Stop();
	afx_msg void OnBnClickedSetParameter();

	//Combat Box
	afx_msg void OnCbnSelchangeComboColor();

	//Edit Control
	afx_msg void OnEnChangeThUpperH();
	afx_msg void OnEnChangeThUnderH();
	afx_msg void OnEnChangeThUpperS();
	afx_msg void OnEnChangeThUnderS();


	//etc
};